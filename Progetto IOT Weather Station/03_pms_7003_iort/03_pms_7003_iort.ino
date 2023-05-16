#include "_PMS7003.h"


void setup()
{
  Serial.begin(115200);
  PMS7003_setup();
}

void loop()
{

  if (PMS_7003_loop()) {
    PMS_7003_makeCSV();
    Serial.println(sensorPM);
    Serial.println(sensorRAW);
  }

}

void publishSensor() {

  StaticJsonDocument<MQTT_BUFFER> jPayload;

  jPayload["name"] = name;

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

}
