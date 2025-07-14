#include <Adafruit_Protomatter.h>

// Pin configuration
uint8_t rgbPins[] = {2, 10, 3, 4, 11, 5};   // R1, G1, B1, R2, G2, B2
uint8_t addrPins[] = {6, 12, 7, 13};      // A, B, C, D
int clockPin = 8;
int latchPin = 14;
int oePin = 9;

// Create matrix object
Adafruit_Protomatter matrix(
  64,  // Width
  4,   // Bit depth (brightness level: 1–6)
  1,   // Number of panels chained horizontally
  rgbPins,
  4, addrPins,
  clockPin,
  latchPin,
  oePin,
  false); // No double buffering

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10); // Wait for Serial to be ready
  Serial.println("Serial initiatied");

  ProtomatterStatus status = matrix.begin();
  Serial.print("Protomatter begin() status: ");
  Serial.println((int)status);

  if (!matrix.begin()) {
    Serial.println("Matrix failed to begin");
    while (1);
  }

  matrix.fillScreen(0x0000);  // Clear screen (black)
  Serial.println("filled for the first time");
  matrix.show();
  Serial.println("showing");

  // Test pattern
  matrix.drawPixel(5, 5, matrix.color565(255, 0, 0));   // Red
  matrix.drawPixel(10, 10, matrix.color565(0, 255, 0)); // Green
  matrix.drawPixel(20, 20, matrix.color565(0, 0, 255)); // Blue
  Serial.println("Drew");
  matrix.show();
  Serial.println("showing");
}

void loop() {
  // Add animation or drawing code here
  Serial.println("looping");
}
