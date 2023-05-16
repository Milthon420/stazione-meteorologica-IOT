
/*********************************************************************
  This is an example which demonstrates use of the Plantower PMS7003
  laser particle counter. This should also work the the PMS5003 sensor,
  as they share the same protocol.

  For more information about the PMS7003, see README.md.

  Written by Jason Striegel.
  BSD license. See license.txt for details.
**********************************************************************/


#include <SoftwareSerial.h>
SoftwareSerial pmsSerial(D5, D6);

#include "src/PMS7003/Plantower_PMS7003.h"

Plantower_PMS7003 pms7003 = Plantower_PMS7003();

#define SEP ","

String sensorPM = "";
String sensorRAW = "";

void PMS7003_setup()
{
  pmsSerial.begin(9600);
  pms7003.init(&pmsSerial);
  //pms7003.debug = true;
}

bool PMS_7003_loop() {
  pms7003.updateFrame();

  if (pms7003.hasNewData()) {

    return (true);
  } else {
    return (false);
  }
}

void PMS_7003_makeCSV()  {
  sensorPM = String(pms7003.getPM_1_0()) + SEP + String(pms7003.getPM_1_0_atmos()) + SEP + String(pms7003.getPM_2_5()) + SEP + String(pms7003.getPM_2_5_atmos()) + SEP + String(pms7003.getPM_10_0()) + SEP + String(pms7003.getPM_10_0_atmos());
  sensorRAW = String(pms7003.getRawGreaterThan_0_3()) + SEP + String(pms7003.getRawGreaterThan_0_5()) + SEP + String(pms7003.getRawGreaterThan_1_0()) + SEP + String(pms7003.getRawGreaterThan_2_5()) + SEP + String(pms7003.getRawGreaterThan_5_0()) + SEP + String(pms7003.getRawGreaterThan_10_0());
}


void PMS7003_print() {
  char output[256];

  sprintf(output, "\nSensor Version: %d    Error Code: %d\n",
          pms7003.getHWVersion(),
          pms7003.getErrorCode());
  Serial.print(output);

  sprintf(output, "    PM1.0 (ug/m3): %2d     [atmos: %d]\n",
          pms7003.getPM_1_0(),
          pms7003.getPM_1_0_atmos());
  Serial.print(output);
  sprintf(output, "    PM2.5 (ug/m3): %2d     [atmos: %d]\n",
          pms7003.getPM_2_5(),
          pms7003.getPM_2_5_atmos());
  Serial.print(output);
  sprintf(output, "    PM10  (ug/m3): %2d     [atmos: %d]\n",
          pms7003.getPM_10_0(),
          pms7003.getPM_10_0_atmos());
  Serial.print(output);

  sprintf(output, "\n    RAW: %2d[>0.3] %2d[>0.5] %2d[>1.0] %2d[>2.5] %2d[>5.0] %2d[>10]\n",
          pms7003.getRawGreaterThan_0_3(),
          pms7003.getRawGreaterThan_0_5(),
          pms7003.getRawGreaterThan_1_0(),
          pms7003.getRawGreaterThan_2_5(),
          pms7003.getRawGreaterThan_5_0(),
          pms7003.getRawGreaterThan_10_0());
  Serial.print(output);

}
