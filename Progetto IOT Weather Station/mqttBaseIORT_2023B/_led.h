#include <NeoPixelBus.h>

NeoPixelBus<NeoRgbFeature, NeoEsp8266BitBang800KbpsMethod> strip(2, D5);

void setLed0( int r, int g, int b) {
  strip.SetPixelColor(0, RgbColor(r, g , b));
  strip.Show();
}

void setLed1( int r, int g, int b) {
  strip.SetPixelColor(1, RgbColor(r, g , b));
  strip.Show();
}
