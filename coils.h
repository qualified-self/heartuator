// shift regs
const int dataPin = 12;
const int latchPin = 14;
const int clockPin = 13;

enum STATES {
  INACTIVE = 0,
  KICK_OUT = 1,
  KICK_IN = 2
};

const int COILS = 6;
int coils[COILS] = {0, 0, 0, 0, 0, 0};


const int REGISTER_COUNT = 2;
byte registerState[REGISTER_COUNT];


void regWrite(int p, bool state) {
  // write to shift register specific pin
  int reg = p >> 3; //  / 8;  1000 -> 0001
  int actualPin = p - (8 * reg);
  digitalWrite(latchPin, LOW);
  for (int i = 0; i < REGISTER_COUNT; i++) {
    //byte states = registerState[i];
    if (i == reg) {
      bitWrite(registerState[i], actualPin, state);
    }
    shiftOut(dataPin, clockPin, MSBFIRST, registerState[i]);
  }
  digitalWrite(latchPin, HIGH);
}


void init_flappy_board() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  // initialize our abstraction array
  //registerState = new byte[numOfRegisters];
  for (size_t i = 0; i < REGISTER_COUNT; i++) {
    registerState[i] = 0;
  }

  // set all SRs to LOW
  for(int j = 0; j < 8*REGISTER_COUNT; j++) {
      regWrite(j, LOW);
  }
}

// srwrite(int srn, int p, int state)
/*
void loop(void)           
{
  regWrite(3,HIGH); 
  regWrite(2, HIGH);
  regWrite(4,LOW);
  delay(5000);
  regWrite(2,LOW);
  regWrite(4,HIGH);
  delay(5000);
}
*/

void coil_write(int coil_id, int state){
  int dir_0_pin;
  int dir_1_pin;
  int onoff_pin;
  int dir;
  int onoff;
  
 if((coil_id % 2) == 0) { // even
    dir_0_pin = (coil_id*8)+2;
    dir_1_pin = (coil_id*8)+4;
    onoff_pin = (coil_id*8)+3;
 }else if((coil_id % 2) == 1){ // odd
    dir_0_pin = (coil_id*8)+5;
    dir_1_pin = (coil_id*8)+7;
    onoff_pin = (coil_id*8)+6;
 }
 if(state == 0) {
  onoff = 0;
 }
 else if(state == 1){
  onoff = 1;
  dir = 0;
 }
 else if(state == 2){
  onoff = 1;
  dir = 1;
 }
  regWrite(onoff_pin, onoff); 
  regWrite(dir_0_pin, dir);
  regWrite(dir_1_pin, !dir);
}


/*
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


void pumpcoil(int id, int state) {
  // TODO integrate Jelle's function
}
*/

void update_coils() {
  for(int i = 0; i < COILS; i++) {
    coil_write(i, coils[i]);
  } // for
}

void draw_coils() {
  Serial.print("[");
  for(int i = 0; i < COILS; i++) {
    switch(coils[i]) {
      case INACTIVE:
        Serial.print("_");
        break;
      case KICK_OUT:
        Serial.print("O");
        break;
      case KICK_IN:
        Serial.print("=");
        break;
    }
  }
  Serial.println("]");
}


