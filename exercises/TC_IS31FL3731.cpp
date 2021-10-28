#include "TC_IS31FL3731.h"

// badge LEDs
uint8_t LEDRegAddr[9] = {0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x10};
uint8_t LEDRegUsed[9] = {0b10010011, 0b10010011, 0b10010010, 0b11101111, 0b11111111, 0b11110111, 0b01111111, 0b11111111, 0b10111111};

/**************************************************************************/
/*!
    @brief Initialize hardware and clear display
    @param addr The I2C address we expect to find the chip at
    @returns True on success, false if chip isnt found
*/
/**************************************************************************/
bool TC_IS31FL3731::begin(uint8_t sdaPin, uint8_t sclPin, uint8_t addr) {
  Wire.begin(sdaPin, sclPin, 400000);

  _i2caddr = addr;

  // A basic scanner, see if it ACK's
  Wire.beginTransmission(_i2caddr);
  if (Wire.endTransmission() != 0) {
    return false;
  }

  // shutdown
  writeRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_SHUTDOWN, 0x00);

  delay(10);

  // out of shutdown
  writeRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_SHUTDOWN, 0x01);

  // picture mode
  writeRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_CONFIG,
                 CONFIG_MODE_PICTURE);
  setPictureFrame(0);

  for (int i=0; i<8; i++)
  {  
    // set PWM value to 0 for each LED
    setAllLEDPWM(0, i);

    // turn on only the LEDs we have available
    for (uint8_t j=0; j<9; j++) {
      writeRegister8(i, LEDRegAddr[j], LEDRegUsed[j]);
    }
  }

  audioSync(false);

  return true;
}

/**************************************************************************/
/*!
    @brief Sets all LEDs on & 0 PWM for current frame.
    @note It's recommended to just adjust PWM values with setAllLEDPWM 
          instead of using this function to avoid messing with LED state
    @param bank The bank/frame we will set the data in
*/
/**************************************************************************/
void TC_IS31FL3731::clear(uint8_t bank) {
  for (uint8_t i = 0; i < 6; i++) {
    Wire.beginTransmission(_i2caddr);
    Wire.write((byte)0x24 + i * 24);
    // write 24 bytes at once
    for (uint8_t j = 0; j < 24; j++) {
      Wire.write((byte)0);
    }
    Wire.endTransmission();
  }
}

/**************************************************************************/
/*!
    @brief Low level accesssor - sets a 8-bit PWM pixel value to a bank location
    does not handle rotation, x/y or any rearrangements!
    @param lednum The offset into the bank that corresponds to the LED
    @param bank The bank/frame we will set the data in
    @param pwm brightnes, from 0 (off) to 255 (max on)
*/
/**************************************************************************/
void TC_IS31FL3731::setLEDPWM(uint8_t lednum, uint8_t pwm, uint8_t bank) {
  if (lednum >= 144)
    return;
  writeRegister8(bank, 0x24 + lednum, pwm);
}

/**************************************************************************/
/*!
    @brief Low level accesssor - sets a 8-bit PWM pixel value to a bank location
    does not handle rotation, x/y or any rearrangements!
    @param lednum The offset into the bank that corresponds to the LED
    @param bank The bank/frame we will set the data in
    @param pwm brightnes, from 0 (off) to 255 (max on)
*/
/**************************************************************************/
void TC_IS31FL3731::setAllLEDPWM(uint8_t pwm, uint8_t bank) {
  for (uint8_t i = 0; i < 6; i++) {
    Wire.beginTransmission(_i2caddr);
    Wire.write((byte)0x24 + i * 24);
    // write 24 bytes at once
    for (uint8_t j = 0; j < 24; j++) {
      Wire.write((byte)pwm);
    }
    Wire.endTransmission();
  }
}

/**************************************************************************/
/*!
    @brief Sets the LED color for LEDs in the top row of THOTCON
    @param leds 7 LSbits are used to determine which LEDs to chages
                1 to change, 0 to leave un affected
    @param redColor PWM for red channel of LED from 0 (off) to 255 (max on)
    @param greenColor PWM for green channel of LED from 0 (off) to 255 (max on)
    @param blueColor PWM for blue channel of LED from 0 (off) to 255 (max on)
    @param bank The bank/frame we will set the data in
*/
/**************************************************************************/
void TC_IS31FL3731::setTextTopColor(uint8_t leds, uint8_t redColor, uint8_t greenColor, uint8_t blueColor, uint8_t bank) {  
  for (uint8_t i=0; i<7; i++) {
    if (leds & (0x1 << i)) {
      if (i) {
        // set red
        writeRegister8(bank, 0xA3 + (7-i), redColor); // sets THOTCO
      
        // set green
        writeRegister8(bank, 0x83 + (7-i), greenColor); // sets THOTCO
      
        // set blue
        writeRegister8(bank, 0x93 + (7-i), blueColor); // sets THOTCO
      } else { // set N led
        // set red
        writeRegister8(bank, 0x4B, redColor); // sets N
        // set green
        writeRegister8(bank, 0x2B, greenColor); // sets N
        // set blue
        writeRegister8(bank, 0x3B, blueColor); // sets N
      }
    }
  }
}

/**************************************************************************/
/*!
    @brief Sets the LED color for LEDs in the bottom row of THOTCON
    @param leds 7 LSbits are used to determine which LEDs to chages
                1 to change, 0 to leave un affected
    @param redColor PWM for red channel of LED from 0 (off) to 255 (max on)
    @param greenColor PWM for green channel of LED from 0 (off) to 255 (max on)
    @param blueColor PWM for blue channel of LED from 0 (off) to 255 (max on)
    @param bank The bank/frame we will set the data in
*/
/**************************************************************************/
void TC_IS31FL3731::setTextBotColor(uint8_t leds, uint8_t redColor, uint8_t greenColor, uint8_t blueColor, uint8_t bank) {  
  for (uint8_t i=0; i<7; i++) {
    if (leds & (0x1 << i)) {
      if (i) {
        // set red
        if (i > 3) {
          writeRegister8(bank, 0x73 + (7-i), redColor); // sets THO
        } else {
          writeRegister8(bank, 0x75 + (7-i), redColor); // sets TCO
        }
      
        // set green
        if (i > 3) {
          writeRegister8(bank, 0x53 + (7-i), greenColor); // sets THO
        } else {
          writeRegister8(bank, 0x55 + (7-i), greenColor); // sets TCO
        }
      
        // set blue
        if (i > 3) {
          writeRegister8(bank, 0x63 + (7-i), blueColor); // sets THO
        } else {
          writeRegister8(bank, 0x65 + (7-i), blueColor); // sets TCO
        }
      } else {
        // set red
        writeRegister8(bank, 0x48, redColor); // sets N
        // set green
        writeRegister8(bank, 0x28, greenColor); // sets N
        // set blue
        writeRegister8(bank, 0x38, blueColor); // sets N
      }
    }
  }
}

/**************************************************************************/
/*!
    @brief Sets the LED intensity/PWM for the single color LEDs
    @param led LED to set
    @param pwm PWM value for LED from 0 (off) to 255 (max on)
    @param bank The bank/frame we will set the data in
*/
/**************************************************************************/
void TC_IS31FL3731::setBadgeLED(LED led, uint8_t pwm, uint8_t bank) {
  switch(led) {
    case UP_LED:
      writeRegister8(bank, 0x78, pwm);
      break;

    case RIGHT_LED:
      writeRegister8(bank, 0x68, pwm);
      break;

    case DOWN_LED:
      writeRegister8(bank, 0x67, pwm);
      break;

    case LEFT_LED:
      writeRegister8(bank, 0x57, pwm);
      break;

    case SELECT_LED:
      writeRegister8(bank, 0x9A, pwm);
      break;

    case START_LED:
      writeRegister8(bank, 0x8A, pwm);
      break;

    case B_LED:
      writeRegister8(bank, 0xAB, pwm);
      break;

    case A_LED:
      writeRegister8(bank, 0x9B, pwm);
      break;

    case SEAT2_LED:
      writeRegister8(bank, 0x35, pwm);
      break;

    case SEAT1_LED:
      writeRegister8(bank, 0x45, pwm);
      break;

    case SEAT4_LED:
      writeRegister8(bank, 0x24, pwm);
      break;

    case SEAT3_LED:
      writeRegister8(bank, 0x34, pwm);
      break;

    case SEAT5_LED:
      writeRegister8(bank, 0x25, pwm);
      break;

    default:
      break;
  }
}

/**************************************************************************/
/*!
    @brief Sets the display more of the LED driver
    @param mode Mode to change to; picture (single frame), auto (mutliple
                frames in progression), audio
*/
/**************************************************************************/
void TC_IS31FL3731::setDisplayMode(Display_Mode mode)
{
  uint8_t newMode = CONFIG_MODE_PICTURE;

  if (mode == Display_Mode_Auto_Play)
    newMode = CONFIG_MODE_AUTOPLAY;
  else if (mode == Display_Mode_Audio_Play)
    newMode = CONFIG_MODE_AUDIOPLAY;

  modifyRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_CONFIG, newMode,
                  CONFIG_MODE_MASK);
  
  return;
}

/**************************************************************************/
/*!
    @brief Sets starting frame for auto play
    @param frame Frame (0-7)
*/
/**************************************************************************/
void TC_IS31FL3731::setAutoPlayStart(uint8_t frame)
{
  if (frame > 7) { frame = 0; }

  modifyRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_CONFIG, frame, 
                  CONFIG_FRAME_MASK);
  
  return;
}

/**************************************************************************/
/*!
    @brief Sets number of loops for auto play
    @param loops Number of loops to play through (1-7) or 0 for unlimited
*/
/**************************************************************************/
void TC_IS31FL3731::setAutoPlayLoops(uint8_t loops)
{
  if (loops > 7) { loops = 0; }
  uint8_t newVal = loops << AUTO_CONTROL_LOOPS_SHIFT;

  modifyRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_AUTO_CONTROL1, newVal,
                  AUTO_CONTROL_LOOPS_MASK);
  
  return;
}

/**************************************************************************/
/*!
    @brief Sets number of frames to play through per loop in auto play
    @param frames Number of frames to play through (1-7) or 0 for all
*/
/**************************************************************************/
void TC_IS31FL3731::setAutoPlayFrames(uint8_t frames)
{
  if (frames > 7) { frames = 0; }

  modifyRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_AUTO_CONTROL1, frames,
                  AUTO_CONTROL_FRAMES_MASK);
  
  return;
}

/**************************************************************************/
/*!
    @brief Sets time between frame transitions for auto play
    @param delay_ms Delay in milliseconds (max 693 ms)
*/
/**************************************************************************/
void TC_IS31FL3731::setAutoPlayDelay(uint16_t delay_ms)
{
  if (delay_ms > 693) { delay_ms = 693; }
  uint8_t newVal = (delay_ms / 11);

  writeRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_AUTO_CONTROL2, newVal);
  
  return;
}

/**************************************************************************/
/*!
    @brief Have the chip set the display to the contents of a frame
    @param frame Ranges from 0 - 7 for the 8 frames
*/
/**************************************************************************/
void TC_IS31FL3731::setPictureFrame(uint8_t frame) {
  if (frame > 7) { frame = 0; }

  writeRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_PICTURE_DISPLAY, frame);
}

/**************************************************************************/
/*!
    @brief Switch to a given bank in the chip memory for future reads
    @param bank The IS31 bank to switch to
*/
/**************************************************************************/
void TC_IS31FL3731::selectBank(uint8_t bank)
{
  Wire.beginTransmission(_i2caddr);
  Wire.write((byte)ISSI_COMMANDREGISTER);
  Wire.write(bank);
  Wire.endTransmission();
}

/**************************************************************************/
/*!
    @brief Enable the audio 'sync' for brightness pulsing (not really tested)
    @param sync True to enable, False to disable
*/
/**************************************************************************/
void TC_IS31FL3731::audioSync(bool sync)
{
  if (sync) {
    writeRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_AUDIOSYNC, 0x1);
  } else {
    writeRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_AUDIOSYNC, 0x0);
  }
}

/**************************************************************************/
/*!
    @brief Modify one byte to a register located in a given bank
    @param bank The IS31 bank to write the register location
    @param reg the offset into the bank to write
    @param val The byte value
    @param mask Modify only the bits that are set to one
*/
/**************************************************************************/
void TC_IS31FL3731::modifyRegister8(uint8_t bank, uint8_t reg, uint8_t val,
                                    uint8_t mask)
{
  uint8_t currReg = readRegister8(bank, reg);
  currReg &= ~(mask);
  val |= currReg;
  writeRegister8(bank, reg, val);
}

/**************************************************************************/
/*!
    @brief Write one byte to a register located in a given bank
    @param bank The IS31 bank to write the register location
    @param reg the offset into the bank to write
    @param val The byte value
*/
/**************************************************************************/
void TC_IS31FL3731::writeRegister8(uint8_t bank, uint8_t reg, uint8_t val)
{
  selectBank(bank);

  Wire.beginTransmission(_i2caddr);
  Wire.write((byte)reg);
  Wire.write((byte)val);
  Wire.endTransmission();
}

/**************************************************************************/
/*!
    @brief  Read one byte from a register located in a given bank
    @param   bank The IS31 bank to read the register location
    @param   reg the offset into the bank to read
    @return 1 byte value
*/
/**************************************************************************/
uint8_t TC_IS31FL3731::readRegister8(uint8_t bank, uint8_t reg)
{
  uint8_t x;

  selectBank(bank);

  Wire.beginTransmission(_i2caddr);
  Wire.write((byte)reg);
  Wire.endTransmission();

  Wire.requestFrom(_i2caddr, (size_t)1);
  x = Wire.read();

//  Serial.print("$"); Serial.print(reg, HEX);
//  Serial.print(": 0x"); Serial.println(x, HEX);

  return x;
}
