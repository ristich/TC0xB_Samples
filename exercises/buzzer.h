#ifndef _BUZZER_H_
#define _BUZZER_H_

#include <Tone32.h>

#define BUZZER_PIN      5
#define BUZZER_CHANNEL  0

typedef struct {
  uint16_t note;
  uint8_t hold;
} Tone;

static Tone circusTune[] = {
  {NOTE_C6,  4},
  {NOTE_B5,  4},
  {NOTE_AS5, 2},
  {NOTE_B5,  2},
  {NOTE_AS5, 2},
  {NOTE_A5,  2},
  {NOTE_GS5, 4},
  {NOTE_G5,  4},
  {NOTE_FS5, 4},
  {NOTE_G5,  4},
  {NOTE_A5,  4},
  {NOTE_GS5, 4},
  {NOTE_G5,  2},
  {NOTE_GS5, 2},
  {NOTE_G5,  2},
  {NOTE_FS5, 2},
  {NOTE_F5,  4},
  {NOTE_E5,  4},
  {NOTE_DS5, 4},
  {NOTE_E5,  4},
  {NOTE_G5,  4},
  {NOTE_F5,  2},
  {NOTE_F5,  2},
  {NOTE_CS5, 4},
  {NOTE_D5,  4},
  {NOTE_G5,  4},
  {NOTE_F5,  2},
  {NOTE_F5,  2},
  {NOTE_CS5, 4},
  {NOTE_D5,  4},
  {NOTE_B4,  2},
  {NOTE_C5,  2},
  {NOTE_CS5, 2},
  {NOTE_D5,  2},
  {NOTE_DS5, 2},
  {NOTE_E5,  2},
  {NOTE_F5,  2},
  {NOTE_FS5, 2},
  {NOTE_G5,  2},
  {NOTE_GS5, 2},
  {NOTE_A5,  2},
  {NOTE_B5,  2},
  {NOTE_A5,  4},
  {NOTE_G5,  4}};

#endif // _BUZZER_H_
