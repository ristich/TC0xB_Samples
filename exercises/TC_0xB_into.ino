#include <Wire.h>
#include "TC_IS31FL3731.h"
#include "startup.h"
#include "buzzer.h"
#include "touch.h"
#include <Tone32.h>

#define I2C_ADDR 0x74
#define I2C_SDA 23u
#define I2C_SCL 18u
#define LED_SDB 22u

TC_IS31FL3731 tcleds = TC_IS31FL3731();

uint8_t state;
uint16_t LEDRow;
uint16_t row1Nums;
uint16_t row2Nums;
uint16_t touchLEDs;

void setup() {
  Serial.begin(115200);
  Serial.println("hello world.");

  // This prevents LEDs from affecting buzzer for some reason
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
  ledcWriteTone(BUZZER_CHANNEL, 1);
  ledcDetachPin(BUZZER_PIN);
  ledcWrite(BUZZER_CHANNEL, 0);

  pinMode(I2C_SDA, OUTPUT);
  pinMode(I2C_SCL, OUTPUT);
  pinMode(LED_SDB, OUTPUT);
  digitalWrite(LED_SDB, HIGH);

  tcleds.begin(I2C_SDA, I2C_SCL);

  // startup/boot routine
  startup(tcleds);
}

void loop() {
  // turn on LED with touch
  
  uint8_t reg8Val = 0;
  uint8_t reg7Val = 0;
  
  if (checkTouch(DOWN_TOUCH, TOUCH_THRESH)) { // down
    reg8Val += 1;
    tcleds.setBadgeLED(DOWN_LED, 250, 0);
  } else {
    tcleds.setBadgeLED(DOWN_LED, 0, 0);
  }

  if (checkTouch(LEFT_TOUCH, TOUCH_THRESH)) { // left
    reg8Val += 2;
    tcleds.setBadgeLED(LEFT_LED, 250, 0);
  } else {
    tcleds.setBadgeLED(LEFT_LED, 0, 0);
  }

  if (checkTouch(UP_TOUCH, TOUCH_THRESH)) { // up
    reg8Val += 4;
    tcleds.setBadgeLED(UP_LED, 250, 0);
  } else {
    tcleds.setBadgeLED(UP_LED, 0, 0);
  }

  if (checkTouch(RIGHT_TOUCH, TOUCH_THRESH)) { // right
    reg8Val += 8;
    tcleds.setBadgeLED(RIGHT_LED, 250, 0);
  } else {
    tcleds.setBadgeLED(RIGHT_LED, 0, 0);
  }

  if (checkTouch(START_TOUCH, TOUCH_THRESH)) { // start
    reg7Val += 2;
    tcleds.setBadgeLED(START_LED, 250, 0);
  } else {
    tcleds.setBadgeLED(START_LED, 0, 0);
  }

  if (checkTouch(A_TOUCH, TOUCH_THRESH)) { // A
    reg7Val += 4;
    tcleds.setBadgeLED(A_LED, 250, 0);
  } else {
    tcleds.setBadgeLED(A_LED, 0, 0);
  }

  if (checkTouch(B_TOUCH, TOUCH_THRESH)) { // B
    reg7Val += 8;
    tcleds.setBadgeLED(B_LED, 250, 0);
  } else {
    tcleds.setBadgeLED(B_LED, 0, 0);
  }

  if (checkTouch(SELECT_TOUCH, TOUCH_THRESH)) { // select
    reg7Val += 16;
    tcleds.setBadgeLED(SELECT_LED, 250, 0);
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
