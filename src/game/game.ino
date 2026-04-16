#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <avr/sleep.h>

#define START_STATE   1
#define LAUNCH_STATE  2
#define DEEPSLEEP_STATE  3
#define ROUND_STATE   4 
#define SEQRIGHT_STATE   5 
#define SEQWRONG_STATE   6 

#define POT_PIN A0

#define L1 3
#define L2 4
#define L3 5
#define L4 6

#define LS 9

#define B1 2
#define B2 10
#define B3 11
#define B4 12

#define LCD_ADDR 0x27
LiquidCrystal_I2C lcd(LCD_ADDR, 30, 2);

float T1;
float F;
int level;
int score;
int fadeAmount;
int currIntensity;
int currentState;
int sequence[4];

void wakeUp(){}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lcd.init();
  lcd.backlight();

  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);
  pinMode(LS, OUTPUT);

  pinMode(B1, INPUT);
  pinMode(B2, INPUT);
  pinMode(B3, INPUT);
  pinMode(B4, INPUT);

  randomSeed(analogRead(A1));
  
  int potVal = analogRead(POT_PIN);
  score = 0;
  T1 = 10.00;
  currIntensity = 0;
  fadeAmount = 5;

  level = map(potVal, 0, 1023, 1, 4);
  
  switch(level){
    case 1: F = 0.50; break;
    case 2: F = 0.70; break;
    case 3: F = 1.00; break;
    case 4: F = 1.50; break;
    default: break;
  }

  changeState(START_STATE);
}

void loop() { 
  switch (getCurrentState()) { 
  case START_STATE:
    start();
    break;
  case LAUNCH_STATE:
    launchGame();
    break;
  case DEEPSLEEP_STATE:
    deepSleep();
    break;
  case ROUND_STATE:
    roundGame();
    break;
  case SEQRIGHT_STATE:
    seqRight();
    break;
  case SEQWRONG_STATE:
    seqWrong();
    break;
  }

}


int getCurrentState(){
  return currentState;
}

void changeState(int new_state){
  currentState = new_state;
}


void ledAllOff(){
  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
  digitalWrite(L3, LOW);
  digitalWrite(L4, LOW);
}

//led verdi spenti, led rosso lampeggia, scritta su LCD "Welcome to TOS! Press B1 to start", a seguito di B1 premuto inizia il gioco
void start(){
  score = 0;
  T1 = 10.00;
  ledAllOff();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Welcome to TOS!");
  lcd.setCursor(0, 1);
  lcd.print("Press B1 to Start");
  Serial.println("Welcome to TOS! Press B1 to Start");

  unsigned long startTime = millis();
  while(millis() - startTime < 10000){
    analogWrite(LS, currIntensity);
    currIntensity = currIntensity + fadeAmount;
    if (currIntensity <= 0 || currIntensity >= 255) {
      fadeAmount = -fadeAmount ;
    }

    //se cliccato B1 chiama launchGame
    if(digitalRead(B1) == HIGH){
      changeState(LAUNCH_STATE);
      return;
    }
    delay(20); 
  }
  //se entro 10 sec non premuto B1 entra in deep sleep
  changeState(DEEPSLEEP_STATE);
}

//si verifica nel caso in cui B1 non venga premuto entro 10 sec nella fase di start, si entra in deep sleep e non appena premuto torna a fase di start
void deepSleep() {
  Serial.println("Deep sleep");
  lcd.clear();
  ledAllOff();
  digitalWrite(LS, LOW);

  attachInterrupt(digitalPinToInterrupt(B1), wakeUp, RISING);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();

  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(B1));

  Serial.println("Wake up!");

  delay(100);
  changeState(START_STATE);
}



//si verifica dopo aver premuto B1 in fase di start --> tutti Led spenti, LCD scritta "Go!", punteggio = 0
void launchGame(){
  score = 0;
  ledAllOff();
  digitalWrite(LS, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Go!");
  Serial.println("Go!");

  delay(1000);
  changeState(ROUND_STATE);
}

void generateSeq(){

  int tempSeq[4] = {1, 2, 3, 4};

  for (int i = 3; i > 0; i--) {
    int j = random(0, i + 1);
    int temp = tempSeq[i];
    tempSeq[i] = tempSeq[j];
    tempSeq[j] = temp;
  }

  for (int k = 0; k < 4; k++) {
    sequence[k] = tempSeq[k];
  }

  //per vedere su serial la sequenza generata
  // Serial.print("Seq: ");
  // for (int i = 0; i < 4; i++) {
  //   Serial.print(sequence[i]);
  //   Serial.print(" ");
  // }
  // Serial.println();
}

void turnOnLed(int index){
  int LEDPIN;
  switch (index) {
    case 1: LEDPIN = L1; break;
    case 2: LEDPIN = L2; break;
    case 3: LEDPIN = L3; break;
    case 4: LEDPIN = L4; break;
  }
  
  digitalWrite(LEDPIN, HIGH);
  delay(500);
  digitalWrite(LEDPIN, LOW);
  delay(100);
}



//led tutti spenti, su LCD mostrata sequenza casuale da 1 a 4(4 cifre tutte diverse, es 1234, 4321, 4213, ecc), avvio timer T1 e lettura button in cui player preme con annessa accensione led nell'ordine
void roundGame(){
  ledAllOff();
  digitalWrite(LS, LOW);

  generateSeq();
  
  for (int i=0; i<4; i++) {
    turnOnLed(sequence[i]);
  }

  //controllo sequenza
  unsigned long startTime = millis();
  int index = 0;

  while (index<4) {
  
    if (millis() - startTime > (unsigned long)(T1 * 1000)) {
      changeState(SEQWRONG_STATE);
      return;
    }

    int pressed = 0;
    if (digitalRead(B1) ==  HIGH) pressed = 1;
    if (digitalRead(B2) == HIGH) pressed = 2;
    if (digitalRead(B3) == HIGH) pressed = 3;
    if (digitalRead(B4) == HIGH) pressed = 4;

    if(pressed!=0){
      turnOnLed(pressed);
    
      if (pressed == sequence[index]) {
        index++;
      } else {
        changeState(SEQWRONG_STATE);
        return;
      }

      /* FORSE non proprio necessario
      // aspetta che tutti i pulsanti vengano rilasciati
      while (digitalRead(B1) == HIGH || digitalRead(B2) == HIGH || 
           digitalRead(B3) == HIGH || digitalRead(B4) == HIGH) {
        delay(10);
      }
      */
    }
  }

  changeState(SEQRIGHT_STATE);
}

//seq corretta e in tempo --> punteggio += 1, LCD scrive "Good! Score XXX" (xxx=punteggio), new round, T1 -= F
void seqRight(){
  score++;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GOOD! Score: ");
  lcd.print(score);
  Serial.print("GOOD! Score: ");
  Serial.println(score);

  T1-=F;
  delay(2000);
  changeState(ROUND_STATE);
}

//seq non corretta o non in tempo --> LED rosso accesso per 2 sec, gioco termina, LCD scritta "Game Over-Final Score XXX" per 10 sec, gioco ritorna ad inizio
void seqWrong(){
  lcd.clear();
  ledAllOff();
  lcd.setCursor(0,0);
  lcd.print("Game Over -");
  lcd.setCursor(0, 1);
  lcd.print("Final Score ");
  lcd.print(score);
  Serial.print("Game Over-Final Score ");
  Serial.println(score);

  
  digitalWrite(LS, HIGH);
  delay(2000);
  digitalWrite(LS, LOW);

  delay(8000);
  changeState(START_STATE);
}

