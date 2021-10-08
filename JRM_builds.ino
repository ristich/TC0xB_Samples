#include <Arduino.h>
#include <SparkFunLIS3DH.h>
#include <Wire.h>
#include "TC_IS31FL3731.h"
#include "startup.h"
#include "buzzer.h"
#include <Tone32.h>

#define I2C_ADDR 0x74
#define I2C_SDA 23u
#define I2C_SCL 18u
#define LED_SDB 22u

LIS3DH myIMU; // default i2c addr 0x19 (accelerometer)

TC_IS31FL3731 tcleds = TC_IS31FL3731();

uint8_t state;
uint16_t LEDRow;
uint16_t row1Nums;
uint16_t row2Nums;
uint16_t touchLEDs;
uint8_t regVal; // register value to set
float buzzerTone; //tone that will be repeatedly emitted by badge
float toneShiftRate; //rate at which the pitch is allowed to change
float maxThrow; //this is the maximum value of the users throw!
float lastAccelVal; //this will store the value of the accelerometer from the last loop
float currentAccelVal; //this will store the current accelerometer value
float accelDif; //Acceleration difference between two loops
float shakeVal; //value constantly decreases to 0, but increases when shaken (not stirred)
uint8_t decVals[7] = {64, 96, 112, 120, 124, 126, 127};

int level=0;

void setup() {
  shakeVal = 0.0;
  toneShiftRate = 50.0;
  maxThrow = 0.0;
  accelDif = 0.0;
  currentAccelVal = 0.0;
  regVal = 0.0;
  buzzerTone = 100.0;

  Serial.begin(115200);
  Serial.println("hello world.");

  pinMode(I2C_SDA, OUTPUT);
  pinMode(I2C_SCL, OUTPUT);
  pinMode(LED_SDB, OUTPUT);
  digitalWrite(LED_SDB, HIGH);

  tcleds.begin(I2C_SDA, I2C_SCL);

  myIMU.begin();
  lastAccelVal = myIMU.readFloatAccelX();

  startup(tcleds);
}

void loop() {

  uint8_t reg8Val = 0;
  uint8_t reg7Val = 0;
  
  ToneCode();

  Serial.print("Level: ");
  Serial.println(level);

  if (checkTouch(12, 25)) { // down
    reg8Val += 1;
    tcleds.setBadgeLED(DOWN_LED, 250, 0);
  } else {
    tcleds.setBadgeLED(DOWN_LED, 0, 0);
  }

  if (checkTouch(2, 25)) { // left
    reg8Val += 2;
    tcleds.setBadgeLED(LEFT_LED, 250, 0);
  } else {
    tcleds.setBadgeLED(LEFT_LED, 0, 0);
  }

  if (checkTouch(15, 25)) { // up
    reg8Val += 4;
    tcleds.setBadgeLED(UP_LED, 250, 0);
  } else {
    tcleds.setBadgeLED(UP_LED, 0, 0);
  }

  if (checkTouch(13, 25)) { // right
    reg8Val += 8;
    tcleds.setBadgeLED(RIGHT_LED, 250, 0);
  } else {
    tcleds.setBadgeLED(RIGHT_LED, 0, 0);
  }

  if (checkTouch(4, 25)) { // start
    reg7Val += 2;
    tcleds.setBadgeLED(START_LED, 250, 0);
  } else {
    tcleds.setBadgeLED(START_LED, 0, 0);
  }

  if (checkTouch(T7, 25)) { // A
    reg7Val += 4;
    tcleds.setBadgeLED(A_LED, 250, 0);
  } else {
    tcleds.setBadgeLED(A_LED, 0, 0);
  }

  if (checkTouch(0, 25)) { // B
    reg7Val += 8;
    tcleds.setBadgeLED(B_LED, 250, 0);
  } else {
    tcleds.setBadgeLED(B_LED, 0, 0);
  }

  if (checkTouch(14, 25)) { // select
    reg7Val += 16;
    tcleds.setBadgeLED(SELECT_LED, 250, 0);
    // if it's pressed down turn it true, then immediately make it false. once your finger lifts off, it can go back to checking if true
    
  } else {
    tcleds.setBadgeLED(SELECT_LED, 0, 0);
  }

  if ((reg7Val > 0) || (reg8Val > 0))
  {
    Serial.print(reg7Val);
    Serial.print(',');
    Serial.println(reg8Val);
  }

}

// simple debounce check for touch sensing
//   gpioPin: gpio or touch number
//   threshold: threshold for touchRead
uint8_t checkTouch(uint8_t gpioPin, uint8_t threshold) {
  if (touchRead(gpioPin) <= threshold) {
    if (touchRead(gpioPin) <= threshold) {
      if (touchRead(gpioPin) <= threshold) {
        return 1;
      }
    }
  }

  return 0;
}
