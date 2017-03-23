#define PCB_L298

// shift regs
const int dataPin = 13;
const int latchPin = 14;
const int clockPin = 12;

#ifdef PCB_L298
enum STATES {
  INACTIVE = 0,
  KICK_OUT = 1,
  KICK_IN = 2
};
#endif

#ifdef PCB_ULN2003
enum STATES {
  INACTIVE = 0,
  KICK_OUT = 1,
  KICK_IN = 10
};
#endif

const int COILS = 6;
int coils[COILS] = {0, 0, 0, 0, 0, 0};

#ifdef PCB_L298
#define coil_write coil_write_l298
#else PCB_ULN2003
#define coil_write coil_write_uln2003
#endif

const int REGISTER_COUNT = 4;
byte registerState[REGISTER_COUNT];


void regWrite(int p, bool state) {
  int reg = p >> 3; /// 8;
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

void coil_write_uln2003(int coil_id, int state) {
  regWrite(coil_id,state);
}


/// @param coil_id coil number, starting at 0
void coil_write(int coil_id, int state){
  int dir_0_pin, dir_1_pin, onoff_pin, dir, onoff;

  int sreg = coil_id >> 1; // divide by 2 to get SR address
  if ((coil_id % 2) == 0) { // use coil nr. to find choose offsets
      dir_0_pin = (sreg*8)+2; // offsets for even coils
      dir_1_pin = (sreg*8)+4;
      onoff_pin = (sreg*8)+3;
  } else {
      dir_0_pin = (sreg*8)+5; // offsets for odd coils
      dir_1_pin = (sreg*8)+7;
      onoff_pin = (sreg*8)+6;
  }

 // get h-bridge pin values
 switch( state ) {
  case 0:
    onoff = 0;
    break;
  case 1:
    onoff = 1;
    dir = 0;
    break;
  case 2:
    onoff = 1;
    dir = 1;
    break;
 } // case state

  regWrite(onoff_pin, onoff);
  regWrite(dir_0_pin, dir);
  regWrite(dir_1_pin, !dir);
}

/*
// @BUGGY
void coil_write_l298(int coil_id, int state){
  int dir_0_pin, dir_1_pin, onoff_pin, dir, onoff;

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
*/

void reset_coils() {
  for(int i = 0; i < COILS; i++) {
    coil_write(i, 0);
  }
}

void update_coils() {
  for(int i = 0; i < COILS; i++) {
    coil_write(i, coils[i]);
  }
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

#ifdef PCB_L298
      case KICK_IN:
        Serial.print("=");
        break;
#endif

    } // switch
  } // for
  Serial.println("]");
}
