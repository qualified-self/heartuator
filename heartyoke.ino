// build settings change between featherboard and arduino nano
#undef __BUILD_FEATHER__ // to switch to Arduino #undef this line...
#define __BUILD_NANO__ // ... and #define this one

#ifdef __BUILD_NANO__
#include <Arduino.h>
#endif

#ifdef __BUILD_FEATHER__
  #include <ESP8266WiFi.h>
  #include <WiFiUdp.h>
  // this library is needed: https://github.com/CNMAT/OSC
  #include <OSCMessage.h>
#endif

#include "coils.h"
#include "animation.h"

// TESTED OK

#ifdef __BUILD_FEATHER__
  #include "wifisettings.h"
#endif

bool flapUp = false;
unsigned long lastFlapUp = 0;

int coil = 0;

const int beatTime = 20; // in ms
int beats[3] = {20, 80, 60};
int beatidx = 0;

Animation *current = NULL;


// INIT /////////////////////////////////////////////////////////////
void init_flappy_board() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  flapUp = false;
  lastFlapUp = millis();
}

#ifdef __BUILD_FEATHER__
void init_wifi() {
/*  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
*/
}
#endif

void setup() {
    Serial.begin(115200);
    while( !Serial ) {} // wait for serial init

    init_flappy_board();

#ifdef __BUILD_FEATHER__
    init_wifi();
#endif

  Serial.println("Initializing...");
  current = new LeftRight(coils, 120); //Heartbeat(coils, 120);
}

// ANIMATION ////////////////////////////////////////////////////////////
void animation_change(Animation *anim) {
  current = anim;
}

void animation_loop() {
  if( (current != NULL)
      && (current->ready()) ) {
    current->update();
    draw_coils();
  }

  if(current->finished()) {
    Serial.println("FINISHED!");
  }
}


// MESSAGE HANDLERS /////////////////////////////////////////////////////
#ifdef __BUILD_FEATHER__
void on_sleep(OSCMessage &msg, int addrOffset) {
  Serial.println("going to sleep to save battery...");
  ESP.deepSleep(10 * 1000000);
}

void on_scene_1(OSCMessage &msg, int addrOffset) {
}

void on_scene_2(OSCMessage &msg, int addrOffset) {
}

void on_scene_3(OSCMessage &msg, int addrOffset) {
}

void on_scene_4(OSCMessage &msg, int addrOffset) {
}

void on_coil(OSCMessage &msg, int addrOffset) {
}

// beat the flap
void on_beat_single(OSCMessage &msg, int addrOffset) {
  if( msg.isInt(0) ) {
    coil = msg.getInt(0);
  }

  Serial.print("beating flap #");
  Serial.println(coil);

  registerWrite(coil, HIGH);
  flapUp = true;
  lastFlapUp = millis();
  delay( beats[beatidx % 3] ); //beatTime );
  beatidx++;
  registerWrite(coil, LOW);
}

void osc_message_pump() {
  OSCMessage in;
  int size;

  if( (size = Udp.parsePacket()) > 0)
  {
    // parse incoming OSC message
    while(size--) {
      in.fill( Udp.read() );
    }

    if(!in.hasError()) {
      in.route("/wearable/sleep", on_sleep);
      in.route("/wearable/beat", on_beat_single);
      in.route("/wearable/coil", on_coil);
      in.route("/wearable/scene/1", on_scene_1);
      in.route("/wearable/scene/2", on_scene_2);
      in.route("/wearable/scene/3", on_scene_3);
      in.route("/wearable/scene/4", on_scene_4);
    }

  } // if
}
#endif

void loop() {
// SEND
//    OSCMessage out("/test");
//    out.add(123); //"hello, osc.");
//
//    Udp.beginPacket(outIp, outPort);
//    out.send(Udp);
//    Udp.endPacket();
//
//    out.empty();
    //delay(5);

//    if( (flapUp == true)
//      && ((millis() - lastFlapUp) > beatTime) ) {
//      registerWrite(coil, LOW);
//      flapUp = false;
//    }

  // update animation sequence
  animation_loop();
  // update physical coils
  update_coils();

  if( current->finished() ) {
    delay(2000);
    current->reset();
  }

#ifdef __BUILD_FEATHER__
  // run message pump
  osc_message_pump();
#endif
} // loop()
