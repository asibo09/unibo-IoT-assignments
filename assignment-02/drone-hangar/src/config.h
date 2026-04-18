#ifndef __CONFIG__
#define __CONFIG__

#define PIN_LED_L1 11 // verde drone inside
#define PIN_LED_L2 12 // verde blinking
#define PIN_LED_L3 13 // rosso alarm

#define PIN_SONAR_TRIG 7
#define PIN_SONAR_ECHO 8
#define PIN_PIR 2
#define PIN_TEMP_SENSOR A0
#define PIN_SERVO 9

#define PIN_BUTTON_RESET 4

// parametri distanze
#define DIST_D1 0.3 // 30cm drone è uscito
#define DIST_D2 0.2 // 20cm drone è rientrato

// parametri tempo
#define TIME_T1 20 // 2sec tempo conferma uscita
#define TIME_T2 30 // 3sec tempo conferma atteraggio

// parametri temperatura
#define TEMP_1 30.0 // pre-alarm
#define TEMP_2 40.0 // alarm

// parametri tempo allarmi
#define TIME_T3 30 // 2sec tempo conferma pre-alarm
#define TIME_T4 40 // 3sec tempo conferma alarm

#endif