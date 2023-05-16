#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(D3);
DallasTemperature temp(&oneWire);

DeviceAddress insideThermometer;

void setup_ds18b20() {
  temp.begin();
  if (!temp.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0");
  temp.setResolution(insideThermometer, 12);
}

float getTemp() {
  temp.requestTemperatures();
  float tempC = temp.getTempCByIndex(0);

  if (tempC == DEVICE_DISCONNECTED_C) {
    tempC = -999;
  }
  return (tempC);
}
