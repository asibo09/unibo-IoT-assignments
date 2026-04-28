package it.unibo.iot;

import org.eclipse.paho.client.mqttv3.*;

public class TankMqttCallback implements MqttCallback {
    @Override
    public void connectionLost(Throwable cause) {
        System.out.println("Connection MQTT lost! " + cause.getMessage());
    }

    @Override
    public void messageArrived(String topic, MqttMessage message) {
        try {
            double distance = Double.parseDouble(new String(message.getPayload()));
            CusMain.currentWaterLevel = CusMain.TANK_CAPACITY - distance;
            CusMain.currentWaterPercentage = (CusMain.currentWaterLevel / CusMain.TANK_CAPACITY) * 100;
            CusMain.lastMqttMsgTime = System.currentTimeMillis();
            System.out.println("MQTT-> Distance from water: "+ distance + ", level: " + " (" + String.format("%.2f", CusMain.currentWaterPercentage) + "%)");
        } catch (Exception e) {
            System.err.println("Error processing MQTT message: " + e.getMessage());
        }
    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken token) {
        // Not used for subscribers
    }
}
