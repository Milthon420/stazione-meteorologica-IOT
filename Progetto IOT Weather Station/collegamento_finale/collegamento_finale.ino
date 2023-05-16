String mqttPayload = "";
String myName = "Meteor";
int SENSOR_TIME = 5000;
// Definisci il pin del sensore magnetico
const int magneticPin = D5;

// Dichiarazione di variabili globali
volatile unsigned long lastInterruptTime = 0;
volatile unsigned long revolutions = 0;


#include "_PMS7003.h"
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <BH1750.h>
#include "_wifi.h"
#include "_mqtt.h"


#define BH1750_CONTINUOUS_HIGH_RES_MODE BH1750::CONTINUOUS_HIGH_RES_MODE
#define SDA_PIN D2
#define SCL_PIN D1
#define WIND_SPEED_FACTOR 2.4

Adafruit_BME280 bme;
BH1750 bh1750 = BH1750();




// Funzione di interrupt per il sensore magnetico
void ICACHE_RAM_ATTR handleInterrupt() {
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 10) {
    revolutions++;
    lastInterruptTime = interruptTime;
  }
}


void setup() {
   // Imposta il pin del sensore magnetico come input
  pinMode(magneticPin, INPUT_PULLUP);

  // Imposta la funzione di interrupt per il pin del sensore magnetico
  attachInterrupt(digitalPinToInterrupt(magneticPin), handleInterrupt, RISING);
  

  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output

  Serial.begin(115200);
  setup_wifi();
  PMS7003_setup();

  randomSeed(micros());
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(mqttReceive);





  Wire.begin(SDA_PIN, SCL_PIN);
  if (!bme.begin(0x76)) {
    Serial.println("Could not find BME280 sensor!");
    while (1);
  }


  if (!bh1750.begin(BH1750_CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("Could not find BH1750 sensor!");
    while (1);
  }
}

/* void publishSensor() {

  StaticJsonDocument<MQTT_BUFFER> jPayload;

  jPayload["PMS"] = name;

  jPayload["PM_1.0_SP"] = data.PM_SP_UG_1_0;
  jPayload["PM_2.5_SP"] = data.PM_SP_UG_2_5;
  jPayload["PM_10_SP"] = data.PM_SP_UG_10_0;

  jPayload["PM_1.0_AE"] = data.PM_AE_UG_1_0;
  jPayload["PM_2.5_AE"] = data.PM_AE_UG_2_5;
  jPayload["PM_10_AE"] = data.PM_AE_UG_10_0;

  jPayload["PM_RAW_0.3"] = data.PM_RAW_0_3;
  jPayload["PM_RAW_.5"] = data.PM_RAW_0_5;
  jPayload["PM_RAW_.0"] = data.PM_RAW_1_0;
  jPayload["PM_RAW_2.5"] = data.PM_RAW_2_5;
  jPayload["PM_RAW_5.0"] = data.PM_RAW_5_0;
  jPayload["PM_RAW_10"] = data.PM_RAW_10_0;


  String   mqttTopic = mqttRoot + "/" + thingId + "/" + mqttTopicSensor;
  publishJSON(mqttTopic, jPayload, false);
}*/




void loop() {

  if (!mqttClient.connected()) {
    if (reconnect()) {
      Serial.println("MQTT connected");
    }
  }

  mqttClient.loop();

   if (PMS_7003_loop()) {
    PMS_7003_makeCSV();
    Serial.println(sensorPM);
    Serial.println(sensorRAW);
  }

  unsigned long now = millis();
  if (now - lastMsg > SENSOR_TIME) {
    lastMsg = now;

 bme.takeForcedMeasurement();
  float temperature = bme.readTemperature();
  float pressure = bme.readPressure() / 100.0F;
  float humidity = bme.readHumidity();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

 

  uint16_t lux = bh1750.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lux");



   // Leggi il numero di rivoluzioni del sensore magnetico
  unsigned long rev = revolutions;

  // Calcola la velocità del vento in mph
  float windSpeed = rev * WIND_SPEED_FACTOR;

  // Formatta la velocità del vento in modo che abbia solo due cifre decimali
  char windSpeedStr[6];
  dtostrf(windSpeed, 5, 2, windSpeedStr);

  // Stampa la velocità del vento sulla console seriale
  Serial.print("Wind speed: ");
  Serial.print(windSpeed);
  Serial.println(" kph");

  // Resetta il contatore delle rivoluzioni
  revolutions = 0;

  // Attendi 1 secondo prima di ripetere il ciclo
  delay(1000);

  
  DynamicJsonDocument doc(1024);
  doc["name"] = myName;
  doc["lux"] = lux;
  doc["Temperature"] = temperature;
  doc["Pressure"] = pressure;
  doc["Humidity"] = humidity;
  doc["Wind_Speed"] = windSpeed;
  doc["PM1:"] = pms7003.getPM_1_0();
  doc["PM2.5:"] = pms7003.getPM_2_5();
  doc["PM10:"] = pms7003.getPM_10_0();
  

  String mqttPayload = "";
  serializeJson(doc, mqttPayload);

  Serial.println("Publish message: " + mqttTopicPubish_sensor + " " + mqttPayload);
  mqttClient.publish(mqttTopicPubish_sensor.c_str(), mqttPayload.c_str());





    if (mqttClient.connected()) {
      Serial.println("Publish message: " + mqttTopicPubish_sensor + " " + mqttPayload);
      mqttClient.publish(mqttTopicPubish_sensor.c_str(), mqttPayload.c_str());
    } else {
      Serial.println("MQTT disconnected");
    }
  }
  delay(2000);
}
