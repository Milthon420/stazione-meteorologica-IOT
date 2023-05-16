#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* mqtt_server = "172.16.206.85";

const char* mqtt_user = "mosquitto";
const char* mqtt_password = "dietpi";

String thingId = "DMO_" + String(WiFi.macAddress().c_str());

//String mqttTopicSubscibe_buzzer = "iort2023/" + thingId + "/buzzer";
//String mqttTopicSubscibe_led = "iort2023/" + thingId + "/led";
String mqttTopicSubscibe_buzzer_name = "iort2023/" + myName + "/buzzer";
String mqttTopicSubscibe_led_name = "iort2023/" + myName + "/led";


String mqttTopicPubish_sensor = "iort2023/" + myName + "/sensor";
String mqttTopicPubish_button = "iort2023/" + myName + "/button";

WiFiClient espClient;
PubSubClient mqttClient(espClient);
unsigned long lastMsg = 0;



void mqttReceive(char* topic, byte* payload, unsigned int length) {
  payload[length] = 0;
  String Payload = String((char*)payload);
  String Topic = String(topic);

  Serial.println("Message arrived " + Topic + " " + Payload );

  //iort2023/marco/buzzer
  // 1000

  if (Topic == mqttTopicSubscibe_buzzer_name) {
    int freq = Payload.toInt();
    tone(D8, freq, 500);
  }

  //iort2023/marco/led
  // { "r":0, "g":64, "b":128}

  if (Topic == mqttTopicSubscibe_led_name) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, Payload);

    int r = doc["r"];
    int g = doc["g"];
    int b = doc["b"];

    //    Serial.println(r);
    //    Serial.println(g);
    //    Serial.println(b);

    setLed0(r, g, b);
  }


}



void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqttClient.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
      // ... and resubscribe
//      mqttClient.subscribe(mqttTopicSubscibe_buzzer.c_str());
//      mqttClient.subscribe(mqttTopicSubscibe_led.c_str());
      mqttClient.subscribe(mqttTopicSubscibe_buzzer_name.c_str());
      mqttClient.subscribe(mqttTopicSubscibe_led_name.c_str());

    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
