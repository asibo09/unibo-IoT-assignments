#include "HardwareSerial.h"
#include "config.h"
#include "esp32-hal.h"
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

// OGGETTI DI RETE
WiFiClient espClient;
PubSubClient client(espClient);

// variabile per far comunicare i due task
volatile bool isNetworkConnected = false;

// STATI FSM
enum SystemState { CONNECTING, CONNECTED, MONITORING, ERROR };
SystemState currentState = CONNECTING;

// lettura sonar
float getWaterLevel() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  long duration = pulseIn(PIN_ECHO, HIGH);
  float distance = duration * 0.034 / 2.0;
  return distance;
}

// TASK 1: Gestione Rete
void networkTask(void *pvParameters) {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  client.setServer(MQTT_SERVER, MQTT_PORT);

  while (true) {
    if (WiFi.status() != WL_CONNECTED) {
      // se non connesso riprova ogni 5 secondi a connettersi
      isNetworkConnected = false;
      Serial.print("[RETE] In attesa del Wi-Fi ");
      Serial.println(WIFI_SSID);
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      vTaskDelay(5000 / portTICK_PERIOD_MS);
    } else {
      if (!client.connected()) {
        // se non connesso al broker riprova ogni 2 secondi
        isNetworkConnected = false;
        Serial.print("[RETE] Wi-Fi OK! Tentativo connessione MQTT a: ");
        Serial.println(MQTT_SERVER);
        // Genera un ID client univoco
        String clientId = "ESP32-TMS-";
        clientId += String(random(0xffff), HEX);

        if (client.connect(clientId.c_str())) {
          isNetworkConnected = true;
        } else {
          vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
      } else {
        // se connesso mantiene viva la connessione
        isNetworkConnected = true;
        client.loop();
      }
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// TASK 2: FSM
void sensorTask(void *pvParameters) {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);

  unsigned long lastSendTime = 0;

  while (true) {
    switch (currentState) {
    case CONNECTING:
      digitalWrite(PIN_LED_RED, HIGH);
      digitalWrite(PIN_LED_GREEN, LOW);

      if (isNetworkConnected) {
        currentState = CONNECTED;
      }
      break;

    case CONNECTED:
      digitalWrite(PIN_LED_GREEN, HIGH);
      digitalWrite(PIN_LED_RED, LOW);

      if (!isNetworkConnected) {
        currentState = ERROR;
      }
      // dopo tempo F passa allo stato di MONITORING per l'invio
      else if (millis() - lastSendTime >= FREQUENCY_F) {
        currentState = MONITORING;
      }
      break;

    case MONITORING: {
      float level = getWaterLevel();
      String payload = String(level);

      // Invia il dato a MQTT
      client.publish(MQTT_TOPIC_LEVEL, payload.c_str());

      lastSendTime = millis();
      currentState = CONNECTED;
    } break;

    case ERROR:
      digitalWrite(PIN_LED_RED, HIGH);
      digitalWrite(PIN_LED_GREEN, LOW);

      if (isNetworkConnected) {
        currentState = CONNECTED;
      }
      break;
    }
    // Tick della FSM ogni 50ms
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  // Inizializzazione dei due Task di FreeRTOS, assegnati al Core 1 dell'ESP32
  xTaskCreatePinnedToCore(networkTask, "NetworkTask", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(sensorTask, "SensorTask", 4096, NULL, 1, NULL, 1);
}

void loop() { vTaskDelete(NULL); }