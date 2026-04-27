#ifndef __CONFIG_H__
#define __CONFIG_H__

// PIN
#define PIN_TRIG 5
#define PIN_ECHO 18
#define PIN_LED_GREEN 4
#define PIN_LED_RED 16

// NETWORK CREDENTIALS (da personalizzare secondo le proprie)
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

// MQTT SETTINGS
#define MQTT_SERVER "broker.mqtt-dashboard.com"
#define MQTT_PORT 1883
#define MQTT_TOPIC_LEVEL "it/unibo/iot/tank/level/siboni"

// Frequenza di campionamento F (in millisecondi)
#define FREQUENCY_F 2000

#endif