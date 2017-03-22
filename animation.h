
class Animation {
public:
  int _metronome;
  int *_coils;
  bool _finished;
  bool _loop;

  Animation(int *coils) : _coils(coils) {}

  virtual void update() {}
  virtual bool ready() {}

  virtual void reset() { _finished = false; }

  bool finished() { return _finished; }

  virtual void clear() {
    for(int i = 0; i < COILS; i++) {
      _coils[i] = 0;
    }
  }

  void frame(int ms) {
    this->_metronome = ms;
    Serial.print("<- frame speed =  ");
    Serial.println(this->_metronome);
  }

  void set_loop(int binary) {
    _loop = binary;
  }

  int must_loop() {
    return _loop;
  }

  int push(int val) {
    val++;
    if( val > 2 ) val = 0;
    return val;
  }
};

// /////////////////////////////////////////////////////////////////////////////////
class Heartbeat : public Animation {
public:
  int _loops;
  int _metronome;
  unsigned long _lastUpdate;

  Heartbeat(int *coils) : Animation(coils), _metronome(1000) {
    _loops = 0;
  }

  void update() {
    _loops++;

    if( _loops == 1 ) {
      _coils[0] = 1;
      _coils[1] = 1;
      _coils[2] = 0;
      _coils[3] = 1;
      _coils[4] = 1;
      _coils[5] = 0;
//      Serial.print("AURICULAR  in ");
//      Serial.println( millis() );
    } else if ( _loops == 2 ) {
      _coils[0] = 2;
      _coils[1] = 2;
      _coils[2] = 1;
      _coils[3] = 2;
      _coils[4] = 2;
      _coils[5] = 1;
//      Serial.print("AURICULAR out VENTRICULAR in ");
//      Serial.println( millis() );
    } else if ( _loops == 3 ) {
      _coils[0] = 0;
      _coils[1] = 0;
      _coils[2] = 2;
      _coils[3] = 0;
      _coils[4] = 0;
      _coils[5] = 2;
//      Serial.print("VENTRICULAR  out ");
//      Serial.println( millis() );
    } else if( _loops == 4 ) {
      _coils[0] = _coils[1] = _coils[2] = _coils[3] = _coils[4] = _coils[5] = 0;
//      Serial.print("REST ");
//      Serial.println( millis() );
    }

    if(_loops >= 4) { _finished = true; }
    _lastUpdate = millis();
  }

  virtual void reset() { 
    _finished = false; 
    _loops = 0; 
  }

  bool ready() {
    return (!_finished) && (millis() > (_lastUpdate + _metronome));
  }
};

// /////////////////////////////////////////////////////////////////////////////////
class LeftRight : public Animation {
public:
  int _loops;
  int _metronome;
  unsigned long _lastUpdate;

  LeftRight(int *coils) : Animation(coils), _metronome(1000) {
    _loops = 0;
  }

  void update() {
    _loops++;

    if( _loops == 1 ) {
      _coils[0] = 1;
      _coils[1] = 1;
      _coils[2] = 0;
      _coils[3] = 0;
      _coils[4] = 0;
      _coils[5] = 0;
      Serial.print("RL A ");
      Serial.println( millis() );
    } else if ( _loops == 2 ) {
      _coils[0] = 2;
      _coils[1] = 2;
      _coils[2] = 1;
      _coils[3] = 0;
      _coils[4] = 0;
      _coils[5] = 0;
      Serial.print("RL B ");
      Serial.println( millis() );
    } else if ( _loops == 3 ) {
      _coils[0] = 0;
      _coils[1] = 0;
      _coils[2] = 2;
      _coils[3] = 0;
      _coils[4] = 0;
      _coils[5] = 0;
      Serial.print("RL C ");
      Serial.println( millis() );
    } else if ( _loops == 4 ) {
      _coils[0] = 0;
      _coils[1] = 0;
      _coils[2] = 0;
      _coils[3] = 1;
      _coils[4] = 1;
      _coils[5] = 0;
      Serial.print("RL D ");
      Serial.println( millis() );
    } else if( _loops == 5 ) {
      _coils[0] = 0;
      _coils[1] = 0;
      _coils[2] = 0;
      _coils[3] = 2;
      _coils[4] = 2;
      _coils[5] = 1;
      Serial.print("RL E ");
      Serial.println( millis() );
    } else if( _loops == 6 ) {
      _coils[0] = 0;
      _coils[1] = 0;
      _coils[2] = 0;
      _coils[3] = 0;
      _coils[4] = 0;
      _coils[5] = 2;
      Serial.print("RL C ");
      Serial.println( millis() );
    } else if( _loops == 7 ) {
      _coils[0] = _coils[1] = _coils[2] = _coils[3] = _coils[4] = _coils[5] = 0;
      Serial.print("REST ");
      Serial.println( millis() );
    }

    if(_loops >= 7) { _finished = true; }
    _lastUpdate = millis();
  }

  virtual void reset() { 
    _finished = false; 
    _loops = 0; 
  }

  bool ready() {
    return (!_finished) && (millis() > (_lastUpdate + _metronome));
  }
};

// /////////////////////////////////////////////////////////////////////////////////
class None : public Animation {
public:
  None(int *coils) : Animation(coils) {
  }

  void update() {
    clear();
  }

  virtual void reset() { 
    _finished = false; 
  }

  bool ready() {
    return true;
  }
};

// /////////////////////////////////////////////////////////////////////////////////
class Flutter : public Animation {
public:
  int _index;
  int _direction;
  int _metronome;
  unsigned long _lastUpdate;

  Flutter(int *coils) : Animation(coils), _metronome(1000) {
    //_loops = 0;
    _index = 0;
    _direction = 1;
  }

  void update() {
    clear();

    int _prev = 0; // the coil before the current one
    
    _coils[_index] = 1; // kick current flap out

    // kick previous flap back in
    if(_index == 0) {
      _prev = COILS;
    } else {
      _prev = _index - _direction;
    }

    // limit to range
    if( _prev >= COILS ) _prev = COILS;
    if( _prev < 0 ) _prev = 0;
    _coils[_prev] = 2; // kick current flap out

    _index += _direction;
    if( (_index < 0) || (_index >= COILS) ) {
      _direction *= -1; // change direction
    }

    _lastUpdate = millis();
  }

  virtual void reset() { 
    _finished = false; 
  }

  bool ready() {
    return (!_finished) && (millis() > (_lastUpdate + _metronome));
  }
};


// /////////////////////////////////////////////////////////////////////////////////
class Random : public Animation {
public:
  int _index;
  int _prev;
  int _metronome;
  unsigned long _lastUpdate;

  Random(int *coils) : Animation(coils), _metronome(1000) {
    _index = 0;
  }

  void update() {
    // sometimes this seems to happen: [__=___]
    // @TODO are we getting indexes out of range?
    clear();
    _prev = _index;
    _index = random(0, COILS);

    _coils[_index] = 1;
    _coils[_prev]  = 2;

    _lastUpdate = millis();
  }

  virtual void reset() { 
    _finished = false; 
  }

  bool ready() {
    return (!_finished) && (millis() > (_lastUpdate + _metronome));
  }
};


