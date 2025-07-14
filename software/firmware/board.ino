#include <Adafruit_Protomatter.h>
#include <Fonts/TomThumb.h>  // Small built-in font
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

bool showWeather = false;
String previousSongTitle = "";
String previousSongArtist = "";
String previousPlayerState = "";

String weatherTemp = "--";
String weatherHumidity = "--";
String weatherWind = "--";
String weatherDesc = "";

void drawWeather() {
  matrix.fillScreen(0x0000);
  const char* title = "Weather";
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

  char buffer[64];
  snprintf(buffer, sizeof(buffer), "%s F", weatherTemp.c_str());
  matrix.setTextColor(matrix.color565(255, 255, 255));
  matrix.setCursor(10, titleY + 20);
  matrix.print(buffer);

  snprintf(buffer, sizeof(buffer), "Humidity: %s%%", weatherHumidity.c_str());
  matrix.setCursor(10, titleY + 35);
  matrix.print(buffer);

  snprintf(buffer, sizeof(buffer), "Wind: %s mph", weatherWind.c_str());
  matrix.setCursor(10, titleY + 50);
  matrix.print(buffer);

  matrix.setCursor(10, titleY + 65);
  matrix.print(weatherDesc.c_str());

  matrix.show();
}

void updateWeatherInfo(String line) {
  if (line.startsWith("WEATHER_TEMP:")) {
    weatherTemp = line.substring(13);
  } else if (line.startsWith("WEATHER_HUMIDITY:")) {
    weatherHumidity = line.substring(17);
  } else if (line.startsWith("WEATHER_WIND:")) {
    weatherWind = line.substring(13);
  } else if (line.startsWith("WEATHER_DESC:")) {
    weatherDesc = line.substring(13);
  }
}

void loop() {
  if (Serial.available()) {
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
    } else if (line == "BTN:weather") {
      toggleWeather();
    } else if (line.startsWith("WEATHER_TEMP:") || line.startsWith("WEATHER_HUMIDITY:") || line.startsWith("WEATHER_WIND:") || line.startsWith("WEATHER_DESC:")) {
      updateWeatherInfo(line);
    }
  }
}

void drawUI() {
  if (showWeather) {
    drawWeather();
    return;
  }

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

void toggleWeather() {
  if (!showWeather) {
    // Save current music info for undo
    previousSongTitle = songTitle;
    previousSongArtist = songArtist;
    previousPlayerState = playerState;
    showWeather = true;
  } else {
    // Undo to previous music info
    songTitle = previousSongTitle;
    songArtist = previousSongArtist;
    playerState = previousPlayerState;
    showWeather = false;
  }
  drawUI();
}

void setup() {
  Serial.begin(115200);
  ProtomatterStatus status = matrix.begin();
  if (status != PROTOMATTER_OK) while(1);
  drawUI();
}

void loop() {
  if (Serial.available()) {
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
    } else if (line == "BTN:weather") {
      toggleWeather();
    }
    // Detect button presses on the matrix and send BTN:playpause/next/previous over Serial
    // (You must implement the button detection logic based on your touch input or hardware buttons)
    // Example:
    // if (pauseButtonPressed) Serial.println("BTN:playpause");
    // if (nextButtonPressed) Serial.println("BTN:next");
    // if (prevButtonPressed) Serial.println("BTN:previous");
  }
}






