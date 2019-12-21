/*
  RaggedPi Project
  Arduino "Saffron" - Cell signal booster
  Written by david durost <david.durost@gmail.com>
*/
/* Includes */
#include <SoftwareSerial.h>
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

/* Constants */
// Misc
#define SDA 2                               // sda
#define SDL 3                               // sdl
#define SS 10                               // select
#define LED 13                              // led
// TFT
#define TFT_DC 9                            // dc
#define TFT_CS 7                            // cs
#define TFT_RST 2                           // reset
#define TFT_MOSI 11                         // mosi
#define TFT_MISO 12                         // miso
#define TFT_CLK 13                          // 
// Pins
#define PENT_PIN A3                         // analog pin
// Settings
#define READ_FREQ 60000                     // ms

/* Variables */
int rssiValue = 0,                          // sensor value
    signalStrength = 0,                     // sensor value
    prevSignalStrength = 0;                 // sensor value
float pentVal = 1.00;                       // sensor value
unsigned long lastPentReadTime = 0;         // ms

/* Objects */
Adafruit_ILI9341 tft = Adafruit_ILI9341(    // tft screen
    TFT_CS, FTF_DC, TFT_MOSI, 
    TFT_CLK, TFT_RST, TFT_MISO
);

/**
 * Draw
 * @param  char* str
 * @param  bool serial
 */
void draw(char *str, bool serial=false) {
    if (serial) Serial.print(str);
    tft.print(str);
}

/**
 * Draw float
 * @param  float val
 * @param  bool  serial
 */
void drawFloat(float val, bool serial=false) {
    if (serial) Serial.print(val);
    tft.print(val);
}

/**
 * Draw line
 * @param  char* str
 * @param  bool serial
 */
void drawln(char *str=NULL, bool serial=false) {
    if (serial) Serial.println(str);
    tft.println(str);
}

/**
 * Read pententiometer
 */
void readPententiometer() {
    pentVal = analogRead(pentPin);
    lastPentReadTime = millis();

    prevSignalStrenth = signalStrength;
    signalStrength = pentVal * (63/1023);
}

/**
 * Read RSSI
 */
void readRSSI() {
    rssiValue = analogRead(0);
}

/**
 * Write to Variable Gain Amplifier
 */
void writeToVGA() {
    SPI.begin();
    digitalWrite(SS, LOW);
    SPI.transfer(signalStrength);
    digitalWrite(SS, HIGH);
    SPI.end();
}

/**
 * Setup
 */
void setup() {
    Serial.begin(9600);
    while (!Serial);
    Serial.println("RaggedPi Project Codename Saffron");

    pinMode(LED, OUTPUT);
    pinMode(SS, OUTPUT);

    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(ILI9341_BLACK);

    delay(600);
}

/**
 * Loop
 */
void loop() {
    if ((millis() - lastPentReadTime) >= READ_DELAY)    readPententiometer();

    delay(5000);
}