// shift regs
const int dataPin = 4;
const int latchPin = 5;
const int clockPin = 2;


const int COILS = 6;
int coils[COILS] = {0, 0, 0, 0, 0, 0};


// //////////////////////////////////////////////////////////////////////
void registerWrite(int whichPin, int whichState) {
// the bits you want to send
  byte bitsToSend = 0;

  // turn off the output so the pins don't light up
  // while you're shifting bits:
  digitalWrite(latchPin, LOW);

  // turn on the next highest bit in bitsToSend:
  bitWrite(bitsToSend, whichPin, whichState);

  // shift the bits out:
  shiftOut(dataPin, clockPin, MSBFIRST, bitsToSend);

    // turn on the output so the LEDs can light up:
  digitalWrite(latchPin, HIGH);
}


void update_coils() {
  for(int i = 0; i < COILS; i++) {
    switch(coils[i]) {
      case 0: // inactive
        break;
      case 1: // flip-out
        break;
      case 2: // kick-back
        break;
    } // switch..case
  } // for
}

void draw_coils() {
  Serial.print("[");
  for(int i = 0; i < COILS; i++) {
    switch(coils[i]) {
      case 0:
        Serial.print("_");
        break;
      case 1:
        Serial.print("O");
        break;
      case 2:
        Serial.print("=");
        break;
    }
  }
  Serial.println("]");
}

