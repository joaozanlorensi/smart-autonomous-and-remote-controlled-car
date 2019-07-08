#include "Buzzer.h"

const int Buzzer::victorySongMelody[] = {
    NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4, NOTE_AS3, NOTE_AS4, 0, 0,
};
const int Buzzer::victorySongTempo[] = {
    12, 12, 12, 12, 12, 12, 6, 6,
};

Buzzer::Buzzer(short int outputPin) {
  this->melodyPin = outputPin;
  pinMode(outputPin, OUTPUT);
}

void Buzzer::sing(int song) {
  // iterate over the notes of the melody:
  if (song == THEME_VICTORY) {
    int size = sizeof(victorySongMelody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / victorySongTempo[thisNote];

      buzz(victorySongMelody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      buzz(0, noteDuration);
    }
  }
}

void Buzzer::buzz(long frequency, long length) {
  long delayValue =
      1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in
  /// half since / there are two phases to each cycle
  long numCycles = frequency * length /
                   1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of
  /// seconds to / get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(melodyPin,
                 HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(melodyPin,
                 LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
}