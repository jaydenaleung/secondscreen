#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

// Change these to whatever suits

#define R1_PIN 2
#define G1_PIN 10
#define B1_PIN 3
#define R2_PIN 4
#define G2_PIN 11
#define B2_PIN 5
#define A_PIN 6
#define B_PIN 12
#define C_PIN 7
#define D_PIN 13
#define E_PIN -1  // required for 1/32 scan panels, like 64x64px. Any available pin would do, i.e. IO32
#define LAT_PIN 14
#define OE_PIN 9
#define CLK_PIN 8


HUB75_I2S_CFG::i2s_pins _pins = { R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN, A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, LAT_PIN, OE_PIN, CLK_PIN };
HUB75_I2S_CFG mxconfig(
  64,    // Module width
  32,    // Module height
  1,     // chain length
  _pins  // pin mapping
);
MatrixPanel_I2S_DMA* dma_display = new MatrixPanel_I2S_DMA(mxconfig);

void setup() {

  Serial.begin(115200);

  dma_display->begin();             // setup the LED matrix
  dma_display->setBrightness8(100);  //0-255
  dma_display->setTextWrap(false);  // Don't wrap at end of line - will do ourselves
  dma_display->clearScreen();
  dma_display->setTextColor(dma_display->color444(15, 15, 0));

  dma_display->clearScreen();

  uint16_t lineColor = dma_display->color565(100, 100, 100);
  dma_display->drawFastHLine(0, 7, 64, lineColor);
  dma_display->drawFastHLine(0, 23, 64, lineColor);
}

void loop() {
  delay(1000);
}