#include <Adafruit_Protomatter.h>
#include <Fonts/TomThumb.h>  // Small built-in font
#include <Servo.h>
int hapticLPin = 0;
int hapticRPin = 1;
Servo hapticL; // init servo objects
Servo hapticR;
int buzzerPin = 15;
uint8_t rgbPins[]  = {2, 10, 3, 4, 11, 5};
uint8_t addrPins[] = {6, 12, 7, 13};
int clockPin = 8;
int latchPin = 14;
int oePin    = 9;
Adafruit_Protomatter matrix(
  64, 6, 1, rgbPins,
  4, addrPins,
  clockPin, latchPin, oePin,
  false
);
void drawPreviousIcon(int16_t x, int16_t y, int16_t size) {
  uint16_t color = matrix.color565(0, 128, 128); // Medium teal/cyan
  int16_t triX1 = x + size;
  int16_t triY1 = y;
  int16_t triX2 = x;
  int16_t triY2 = y + size / 2;
  int16_t triX3 = x + size;
  int16_t triY3 = y + size;
  matrix.fillTriangle(triX1, triY1, triX2, triY2, triX3, triY3, color);
  matrix.fillRect(x + size, y, size / 4, size, color);
}
void drawPauseIcon(int16_t x, int16_t y, int16_t size) {
  uint16_t color = matrix.color565(255, 140, 0); // Bright orange
  int16_t barWidth = size / 4;
  int16_t gap = barWidth;
  matrix.fillRect(x, y, barWidth, size, color);
  matrix.fillRect(x + barWidth + gap, y, barWidth, size, color);
}
void drawNextIcon(int16_t x, int16_t y, int16_t size) {
  uint16_t color = matrix.color565(0, 128, 128); // Medium teal/cyan
  matrix.fillRect(x, y, size / 4, size, color);
  int16_t triX1 = x;
  int16_t triY1 = y;
  int16_t triX2 = x + size;
  int16_t triY2 = y + size / 2;
  int16_t triX3 = x;
  int16_t triY3 = y + size;
  matrix.fillTriangle(triX1, triY1, triX2, triY2, triX3, triY3, color);
}
String songTitle = "";
String songArtist = "";
String playerState = "stopped";
void updateSongInfo(String line) {
  if (line.startsWith("SONG:")) songTitle = line.substring(5);
  else if (line.startsWith("ARTIST:")) songArtist = line.substring(7);
  else if (line.startsWith("STATE:")) playerState = line.substring(6);
}
void drawUI() {
  matrix.fillScreen(0x0000);
  // Draw "Now Playing" title
  const char* title = "Now Playing";
  matrix.setFont(&TomThumb);
  matrix.setTextWrap(false);
  matrix.setTextColor(matrix.color565(50, 100, 200));
  int16_t x1, y1;
  uint16_t w, h;
  matrix.getTextBounds(title, 0, 0, &x1, &y1, &w, &h);
  int16_t titleX = (matrix.width() - w) / 2 - x1;
  int16_t titleY = h + 1;
  matrix.setCursor(titleX, titleY);
  matrix.print(title);
  int16_t size = 6;
  int16_t spacingLeft = 10;
  int16_t spacingRight = 10;
  int16_t totalWidth = size + spacingLeft + size + spacingRight + size;
  int16_t startX = (matrix.width() - totalWidth) / 2;
  int16_t yIcons = titleY + 6;
  drawPreviousIcon(startX, yIcons, size);
  // Draw pause icon with different color if paused
  if (playerState == "playing")
    drawPauseIcon(startX + size + spacingLeft, yIcons, size);
  else {
    uint16_t color = matrix.color565(200, 0, 0); // Red for paused
    int16_t x = startX + size + spacingLeft;
    int16_t y = yIcons;
    int16_t barWidth = size / 4;
    int16_t gap = barWidth;
    matrix.fillRect(x, y, barWidth, size, color);
    matrix.fillRect(x + barWidth + gap, y, barWidth, size, color);
  }
  drawNextIcon(startX + size + spacingLeft + size + spacingRight, yIcons, size);
  // Draw song title below controls
  matrix.setFont(&TomThumb);
  matrix.setTextColor(matrix.color565(0, 200, 0));
  matrix.getTextBounds(songTitle.c_str(), 0, 0, &x1, &y1, &w, &h);
  int16_t songX = (matrix.width() - w) / 2 - x1;
  int16_t songY = yIcons + size + h + 6;
  matrix.setCursor(songX, songY);
  matrix.print(songTitle.c_str());
  matrix.show();
}
void drawWireIcon() {

}
void drawTrackIcon() {

}
void drawPlaceIcon() {
  
}
void drawKiCadUI() { // for KiCad 9.0 or later
  /*
  POWERED BY HOTKEYS:
  W - WIRE
  A - SYMBOL/FOOTPRINT
  P - POWER SYMBOL/PINS (SYMBOL EDITOR)
  CTRL+L - FLAGS
  E - PROPERTIES
  X - TRACK
  V - VIA (DURING TRACK ROUTING)

  SYMBOL EDITOR - P,E
  FOOTPRINT EDITOR - E
  SCHEMATIC EDITOR - W,A,P,CTRL+L
  PCB EDITOR - A,E,X,V

  BOARD LAYOUT: W | X | A |
  */

  matrix.fillScreen(0x0000); // clear

  // title
  const char* title = "KiCad";
  matrix.setFont(&TomThumb);
  matrix.setTextWrap(false);
  matrix.setTextColor(matrix.color565(50, 100, 200));
  int16_t x1, y1;
  uint16_t w, h;
  matrix.getTextBounds(title, 0, 0, &x1, &y1, &w, &h);
  int16_t titleX = (matrix.width() - w) / 2 - x1;
  int16_t titleY = h + 1;
  matrix.setCursor(titleX, titleY);
  matrix.print(title);

  // button icons
  int16_t size = 6;
  int16_t spacingLeft = 10;
  int16_t spacingRight = 10;
  int16_t totalWidth = size + spacingLeft + size + spacingRight + size;
  int16_t startX = (matrix.width() - totalWidth) / 2;
  int16_t yIcons = titleY + 6;
  drawWireIcon(); // write  afunc for this
  drawTrackIcon();
  drawPlaceIcon();
}
// haptics
void hapticTap(Servo hap) { // (5/3 = 1.67) ms/deg servo speed
  hap.write(30);
  hap.write(0);
}
void playTone(int pin, int frequency, int duration) { // for buzzer when tapped
    long delayValue = 1000000 / frequency / 2;
    long cycles = frequency * duration / 1000;
    
    for (long i = 0; i < cycles; i++) {
        digitalWrite(pin, HIGH);
        delayMicroseconds(delayValue);
        digitalWrite(pin, LOW);
        delayMicroseconds(delayValue);
    }
}

void setup() {
  Serial.begin(115200);
  ProtomatterStatus status = matrix.begin();
  if (status != PROTOMATTER_OK) while(1);
  drawUI();

  // haptics
  hapticL.attach(hapticLPin);
  hapticR.attach(hapticRPin);
  pinMode(buzzerPin, OUTPUT);
}
void loop() {
  if (Serial.available()) {
    // play haptics
    playTone(buzzerPin, 250, 15); // buzzer tap sound
    hapticTap(hapticL); // servos
    hapticTap(hapticR);

    // button logic
    String line = Serial.readStringUntil('\n');
    line.trim();
    if (line == "SHOW_BUTTONS") {
      drawUI();
    } else if (line == "HIDE_BUTTONS") {
      matrix.fillScreen(0x0000);
      matrix.show();
    } else if (line.startsWith("SONG:") || line.startsWith("ARTIST:") || line.startsWith("STATE:")) {
      updateSongInfo(line);
      drawUI();
    }
    // Detect button presses on the matrix and send BTN:playpause/next/previous over Serial
    // (You must implement the button detection logic based on your touch input or hardware buttons)
    // Example:
    // if (pauseButtonPressed) Serial.println("BTN:playpause");
    // if (nextButtonPressed) Serial.println("BTN:next");
    // if (prevButtonPressed) Serial.println("BTN:previous");
  }
}