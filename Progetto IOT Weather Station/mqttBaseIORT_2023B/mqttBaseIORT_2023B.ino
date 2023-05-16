String myName="Nove";

#include "_led.h"
#include "_wifi.h"
#include "_mqtt.h"
#include "_ds18b20.h"

int SENSOR_TIME=5000;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(D6, INPUT_PULLUP);
  pinMode(D8, OUTPUT); //buzzer

  strip.Begin();
  setLed0(0, 0, 32);

  Serial.begin(115200);
  setup_wifi();

  setLed0(0, 32, 0);

  randomSeed(micros());
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(mqttReceive);

  setup_ds18b20();

}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  unsigned long now = millis();
  if (now - lastMsg > SENSOR_TIME) {
    lastMsg = now;
    int light = analogRead(A0);

    DynamicJsonDocument doc(1024);
    doc["light"] = light;
    doc["temperature"] = getTemp();
    //doc["name"] = myName;

    String mqttPayload = "";
    serializeJson(doc, mqttPayload);

    Serial.println("Publish message: " + mqttTopicPubish_sensor + " " + mqttPayload);
    mqttClient.publish(mqttTopicPubish_sensor.c_str(), mqttPayload.c_str());
  }

  if (digitalRead(D6) == 0) {
    Serial.println("Publish message: " + mqttTopicPubish_button);
    mqttClient.publish(mqttTopicPubish_button.c_str(), "");
    delay(200);
  }

}
