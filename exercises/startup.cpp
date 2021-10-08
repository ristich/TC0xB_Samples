#include "startup.h"
#include <Tone32.h>
#include "buzzer.h"
#include "touch.h"

/**************************************************************************/
/*!
    @brief Play badge boot/startup routine
    @param ledController LED controller instance
*/
/**************************************************************************/
void startup(TC_IS31FL3731 ledController)
{
  // setup auto play of LEDs during music
  configStartupLEDs(ledController);

  // cycle through song
  // stop and end if a button press
  uint16_t len = sizeof(circusTune)/sizeof(Tone);
  for (uint16_t i=0; i<len; i++)
  {
    tone(BUZZER_PIN, circusTune[i].note, circusTune[i].hold*60, BUZZER_CHANNEL);
    noTone(BUZZER_PIN, BUZZER_CHANNEL);

    // if any touch events, stop startup
    if (checkAnyTouch() == true)
    {
      ledController.setAutoPlayFrames(1);
      delay(500);
      break;
    }
  }

  // back to picture mode
  ledController.setDisplayMode(Display_Mode_Picture);
}

/**************************************************************************/
/*!
    @brief Set up LED controller and LEDs for auto play during startup
    @param ledController LED controller instance
*/
/**************************************************************************/
void configStartupLEDs(TC_IS31FL3731 ledController)
{
  ledController.setAutoPlayFrames(2);
  ledController.setAutoPlayLoops(0);
  ledController.setAutoPlayDelay(175);
  ledController.setAutoPlayStart(1);

  // these should already be 0'd if this is called on startup
  ledController.setAllLEDPWM(0, 1);
  ledController.setAllLEDPWM(0, 2);

  // prepare the show
  ledController.setTextTopColor(0b1010101, 0xFFFF, 0xFFFF, 0xFFFF, 1);
  ledController.setTextTopColor(0b0101010, 0xFFFF, 0xFFFF, 0xFFFF, 2);
  ledController.setTextBotColor(0b0101010, 0xFFFF, 0xFFFF, 0xFFFF, 1);
  ledController.setTextBotColor(0b1010101, 0xFFFF, 0xFFFF, 0xFFFF, 2);

  ledController.setBadgeLED(DOWN_LED, 250, 1);
  ledController.setBadgeLED(UP_LED, 250, 1);
  ledController.setBadgeLED(SELECT_LED, 250, 1);
  ledController.setBadgeLED(B_LED, 250, 1);
  ledController.setBadgeLED(SEAT1_LED, 250, 1);
  ledController.setBadgeLED(SEAT3_LED, 250, 1);
  ledController.setBadgeLED(SEAT5_LED, 250, 1);
  
  ledController.setBadgeLED(LEFT_LED, 250, 2);
  ledController.setBadgeLED(RIGHT_LED, 250, 2);
  ledController.setBadgeLED(START_LED, 250, 2);
  ledController.setBadgeLED(A_LED, 250, 2);
  ledController.setBadgeLED(SEAT2_LED, 250, 2);
  ledController.setBadgeLED(SEAT4_LED, 250, 2);

  // showtime!
  ledController.setDisplayMode(Display_Mode_Auto_Play);
}
