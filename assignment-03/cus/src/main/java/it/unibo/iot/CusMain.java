package it.unibo.iot;

import org.eclipse.paho.client.mqttv3.*;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

import io.vertx.core.Vertx;

public class CusMain {

    static final double TANK_CAPACITY = 100.0;
    static final double L1 = 40.0;
    static final double L2 = 80.0;

    static final long T1 = 5000; // 5 seconds
    static final long T2 = 10000; // 10 seconds

    static final String PORT_NAME = "COM3";
    static final String MQTT_BROKER = "tcp://broker.mqtt-dashboard.com";
    static final String MQTT_ID = "esiot-2025-siboni-" + System.currentTimeMillis();
    static final String MQTT_TOPIC = "it/unibo/iot/tank/level/siboni";

    enum WaterState {
        NORMAL, CHECK_L1, OVER_L1, OVER_L2
    }

    enum NetworkState {
        CONNECTED, UNCONNECTED
    }

    enum Mode {
        AUTOMATIC, MANUAL
    }

    static WaterState waterState = WaterState.NORMAL;
    static NetworkState netState = NetworkState.CONNECTED;
    static Mode currentMode = Mode.AUTOMATIC;

    static double currentWaterLevel = 0.0;
    static double currentWaterPercentage = 0.0;
    static int currentValvePerc = 0;
    static long lastMqttMsgTime = System.currentTimeMillis();
    static long timeL1Exceeded = 0;
    static CommChannel serialChannel;
    static MqttClient mqttClient;

    public static void main(String[] args) {
        System.out.println("Avvio CUS...");

        Vertx vertx = Vertx.vertx();
        vertx.deployVerticle(new DashboardService());

        // inizializzazione seriale
        try {
            serialChannel = new SerialCommChannel(PORT_NAME, 9600);
            System.out.println("Seriale connessa su " + PORT_NAME);
            Thread.sleep(2000);
        } catch (Exception e) {
            System.err.println("Errore Seriale: " + e.getMessage());
        }

        // inizializzazione MQTT
        try {
            mqttClient = new MqttClient(MQTT_BROKER, MQTT_ID, new MemoryPersistence());
            mqttClient.setCallback(new TankMqttCallback());
            mqttClient.connect();
            mqttClient.subscribe(MQTT_TOPIC);
            System.out.println("MQTT connesso e sottoscritto al topic " + MQTT_TOPIC);
        } catch (Exception e) {
            System.err.println("Errore MQTT: " + e.getMessage());
        }

        // Main loop
        while (true) {
            long currentTime = System.currentTimeMillis();

            // FSM Rete
            if (currentTime - lastMqttMsgTime > T2) {
                if (netState == NetworkState.CONNECTED) {
                    netState = NetworkState.UNCONNECTED;
                    System.out.println("ALLARME RETE: Rete UNCONNECTED");
                    sendSerialCmd("ERR");
                }
            } else {
                if (netState == NetworkState.UNCONNECTED) {
                    netState = NetworkState.CONNECTED;
                    waterState = WaterState.NORMAL;
                    sendSerialCmd("VALVE:0");
                    System.out.println("Rete Ripristinata: Rete CONNECTED e Ritorno a NORMAL");
                    sendSerialCmd("RESTORE");
                }

            }

            // FSM Acqua
            if (netState == NetworkState.CONNECTED) {
                if (currentMode == Mode.AUTOMATIC ) {   
                    switch (waterState) {
                        case NORMAL:
                            if (currentWaterPercentage >= L2) {
                                waterState = WaterState.OVER_L2;
                                System.out.println("STATE: OVER_L2");
                                sendSerialCmd("VALVE:100");
                            } else if (currentWaterPercentage >= L1) {
                                waterState = WaterState.CHECK_L1;
                                timeL1Exceeded = currentTime;
                                System.out.println("STATE: CHECK_L1");
                            }
                            break;

                        case CHECK_L1:
                            if (currentWaterPercentage >= L2) {
                                waterState = WaterState.OVER_L2;
                                System.out.println("STATE: OVER_L2");
                                sendSerialCmd("VALVE:100");
                            } else if (currentWaterPercentage < L1) {
                                waterState = WaterState.NORMAL;
                                System.out.println("STATE: NORMAL");
                            } else if (currentTime - timeL1Exceeded >= T1) {
                                waterState = WaterState.OVER_L1;
                                System.out.println("STATE: OVER_L1");
                                sendSerialCmd("VALVE:50");
                            }
                            break;

                        case OVER_L1:
                            if (currentWaterPercentage >= L2) {
                                waterState = WaterState.OVER_L2;
                                System.out.println("STATE: OVER_L2");
                                sendSerialCmd("VALVE:100");
                            } else if (currentWaterPercentage < L1) {
                                waterState = WaterState.NORMAL;
                                System.out.println("STATE: NORMAL");
                                sendSerialCmd("VALVE:0");
                            }
                            break;

                        case OVER_L2:
                            if (currentWaterPercentage < L2 && currentWaterPercentage >= L1) {
                                waterState = WaterState.CHECK_L1;
                                timeL1Exceeded = currentTime;
                                System.out.println("STATE: CHECK_L1");
                            } else if (currentWaterPercentage < L1) {
                                waterState = WaterState.NORMAL;
                                System.out.println("STATE: NORMAL");
                                sendSerialCmd("VALVE:0");
                            }
                            break;
                    }
                } else {
                    // modalità manuale, aggiorna solo lo stato senza inviare comandi
                    if (currentWaterPercentage >= L2) waterState = WaterState.OVER_L2;
                    else if (currentWaterPercentage >= L1) waterState = WaterState.OVER_L1;
                    else waterState = WaterState.NORMAL;
                }
            }
            try { Thread.sleep(100); } catch (Exception e) {}
        }
    }

    public static void sendSerialCmd(String cmd) {
        if (cmd.startsWith("VALVE:")) {
            currentValvePerc = Integer.parseInt(cmd.split(":")[1]);
        }
        
        if (serialChannel != null) {
            serialChannel.sendMsg(cmd);
        }
    }
}