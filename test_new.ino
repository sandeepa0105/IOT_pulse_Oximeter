#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_GFX.h>
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_SSD1306.h>
#define ENABLE_MAX30100 1
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32 

char auth[] = "y80-1oVnZdSoAvAaGq1Z8pA9s98L4vwz";             
char ssid[] = "Redmi Note 9 Pro";       
char pass[] = "12345678";       

#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3c 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#if ENABLE_MAX30100
#define REPORTING_PERIOD_MS     5000

PulseOximeter pox;
#endif
uint32_t tsLastReport = 0;
int xPos = 0;

void onBeatDetected()
{
  Serial.println("Beat!");
  heart_beat(&xPos);
}
void setup()
{
  Serial.begin(115200);
  Serial.println("SSD1306 128x64 OLED TEST");
 
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); 
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 18);
  display.print("Pulse OxiMeter");
  int temp1 = 0;
  int temp2 = 40;
  int temp3 = 80;
  heart_beat(&temp1);
  heart_beat(&temp2);
  heart_beat(&temp3);
  xPos = 0;
  display.display();
  delay(2000); 
  display.cp437(true);
  display.clearDisplay();
  Serial.print("Initializing pulse oximeter..");
  Blynk.begin(auth, ssid, pass);
#if ENABLE_MAX30100

  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);
  display_data(0, 0);
#endif
}
void loop()
{
#if ENABLE_MAX30100
  pox.update();
  Blynk.run();
  int bpm = 0;
  int spo2 = 0;
  
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    
    bpm = pox.getHeartRate();
    spo2 = pox.getSpO2();
    Serial.println(bpm);
    
    Serial.println(spo2);
    
    tsLastReport = millis();
    Blynk.virtualWrite(V0, bpm);
    Blynk.virtualWrite(V1, spo2);
    display_data(bpm, spo2);
  }
#endif
  drawLine(&xPos);
}
void display_data(int bpm, int spo2) {
  display.fillRect(0, 18, 127, 15, SSD1306_BLACK);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 18);
  
  display.print("BPM ");
  display.print(bpm);
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(64, 18);
  
  display.print("Spo2 ");
  display.print(spo2);
  display.print("%");
  display.display();
}
void drawLine(int *x_pos) {
  // Draw a single pixel in white
  display.drawPixel(*x_pos, 8, SSD1306_WHITE);
  display.drawPixel((*x_pos)++, 8, SSD1306_WHITE);
  display.drawPixel((*x_pos)++, 8, SSD1306_WHITE);
  display.drawPixel((*x_pos)++, 8, SSD1306_WHITE);
  display.drawPixel((*x_pos), 8, BLACK);  // -----
  //Serial.println(*x_pos);
  display.fillRect(*x_pos, 0, 31, 16, SSD1306_BLACK);
  display.display();
  delay(1);
  if (*x_pos >= SCREEN_WIDTH) {
    *x_pos = 0;
  }
}
void heart_beat(int *x_pos) {
  /************************************************/
  //display.clearDisplay();
  display.fillRect(*x_pos, 0, 30, 15, SSD1306_BLACK);
  // Draw a single pixel in white
  display.drawPixel(*x_pos + 0, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 1, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 2, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 3, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 4, 8, BLACK); // -----
  //display.display();
  //delay(1);
  display.drawPixel(*x_pos + 5, 7, SSD1306_WHITE);
  display.drawPixel(*x_pos + 6, 6, SSD1306_WHITE);
  display.drawPixel(*x_pos + 7, 7, SSD1306_WHITE); // .~.
  //display.display();
  //delay(1);
  display.drawPixel(*x_pos + 8, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 9, 8, SSD1306_WHITE); // --
  //display.display();
  //delay(1);
  /******************************************/
  display.drawPixel(*x_pos + 10, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 10, 9, SSD1306_WHITE);
  display.drawPixel(*x_pos + 11, 10, SSD1306_WHITE);
  display.drawPixel(*x_pos + 11, 11, SSD1306_WHITE);
  //display.display();
  //delay(1);
  /******************************************/
  display.drawPixel(*x_pos + 12, 10, SSD1306_WHITE);
  display.drawPixel(*x_pos + 12, 9, SSD1306_WHITE);
  display.drawPixel(*x_pos + 12, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 12, 7, SSD1306_WHITE);
  //display.display();
  //delay(1);
  display.drawPixel(*x_pos + 13, 6, SSD1306_WHITE);
  display.drawPixel(*x_pos + 13, 5, SSD1306_WHITE);
  display.drawPixel(*x_pos + 13, 4, SSD1306_WHITE);
  display.drawPixel(*x_pos + 13, 3, SSD1306_WHITE);
  //display.display();
  //delay(1);
  display.drawPixel(*x_pos + 14, 2, SSD1306_WHITE);
  display.drawPixel(*x_pos + 14, 1, SSD1306_WHITE);
  display.drawPixel(*x_pos + 14, 0, SSD1306_WHITE);
  display.drawPixel(*x_pos + 14, 0, SSD1306_WHITE);
  //display.display();
  //delay(1);
  /******************************************/
  display.drawPixel(*x_pos + 15, 0, SSD1306_WHITE);
  display.drawPixel(*x_pos + 15, 1, SSD1306_WHITE);
  display.drawPixel(*x_pos + 15, 2, SSD1306_WHITE);
  display.drawPixel(*x_pos + 15, 3, SSD1306_WHITE);
  //display.display();
  //delay(1);
  display.drawPixel(*x_pos + 15, 4, SSD1306_WHITE);
  display.drawPixel(*x_pos + 15, 5, SSD1306_WHITE);
  display.drawPixel(*x_pos + 16, 6, SSD1306_WHITE);
  display.drawPixel(*x_pos + 16, 7, SSD1306_WHITE);
  //display.display();
  //delay(1);
  display.drawPixel(*x_pos + 16, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 16, 9, SSD1306_WHITE);
  display.drawPixel(*x_pos + 16, 10, SSD1306_WHITE);
  display.drawPixel(*x_pos + 16, 11, SSD1306_WHITE);
  //display.display();
  //delay(1);
  display.drawPixel(*x_pos + 17, 12, SSD1306_WHITE);
  display.drawPixel(*x_pos + 17, 13, SSD1306_WHITE);
  display.drawPixel(*x_pos + 17, 14, SSD1306_WHITE);
  display.drawPixel(*x_pos + 17, 15, SSD1306_WHITE);
  //display.display();
  //delay(1);
  display.drawPixel(*x_pos + 18, 15, SSD1306_WHITE);
  display.drawPixel(*x_pos + 18, 14, SSD1306_WHITE);
  display.drawPixel(*x_pos + 18, 13, SSD1306_WHITE);
  display.drawPixel(*x_pos + 18, 12, SSD1306_WHITE);
  //display.display();
  //delay(1);
  display.drawPixel(*x_pos + 19, 11, SSD1306_WHITE);
  display.drawPixel(*x_pos + 19, 10, SSD1306_WHITE);
  display.drawPixel(*x_pos + 19, 9, SSD1306_WHITE);
  display.drawPixel(*x_pos + 19, 8, SSD1306_WHITE);
  //display.display();
  //delay(1);
  /****************************************************/
  display.drawPixel(*x_pos + 20, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 21, 8, SSD1306_WHITE);
  //display.display();
  //delay(1);
  /****************************************************/
  display.drawPixel(*x_pos + 22, 7, SSD1306_WHITE);
  display.drawPixel(*x_pos + 23, 6, SSD1306_WHITE);
  display.drawPixel(*x_pos + 24, 6, SSD1306_WHITE);
  display.drawPixel(*x_pos + 25, 7, SSD1306_WHITE);
  //display.display();
  //delay(1);
  /************************************************/
  display.drawPixel(*x_pos + 26, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 27, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 28, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 29, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 30, 8, SSD1306_WHITE); // -----
  *x_pos = *x_pos + 30;
  display.display();
  delay(1);
}
