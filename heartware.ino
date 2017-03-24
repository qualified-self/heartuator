#include <Metro.h>
#include "config.h"

// build settings change between featherboard and arduino nano
#define __BUILD_FEATHER__ // to switch to Arduino #undef this line...
#undef __BUILD_NANO__ // ... and #define this one

#ifdef __BUILD_NANO__
#include <Arduino.h>
#endif


#ifdef __BUILD_FEATHER__
  #include <ESP8266WiFi.h>
  #include <WiFiUdp.h>
  // this library is needed: https://github.com/CNMAT/OSC
  #include <OSCMessage.h>
#endif

//#include <Plaquette.h>
//#include <PqExtra.h>
//

#include "coils.h"
#include "animation.h"

#ifdef __BUILD_FEATHER__
  #include "wifisettings.h"
#endif

bool flapUp = false;
unsigned long lastFlapUp = 0;

int coil = 0;

const int beatTime = 20; // in ms
int beats[3] = {20, 80, 60};
int beatidx = 0;

int heartware_id = -1;

Animation *current = NULL;

Heartbeat heartbeat(coils);
LeftRight leftright(coils);
Flutter flutter(coils);
Random randombeat(coils);
Introvert introvert(coils);

Metro alive = Metro(ALIVE_ACK_MS);

// INIT /////////////////////////////////////////////////////////////
void init_animator() {
//  current = new Flutter(coils, 1000); //new LeftRight(coils, 120); //Heartbeat(coils, 120);
}

#ifdef __BUILD_FEATHER__
void init_wifi() {
  _LOG();
  _LOG();
  if(DEBUG) {
    Serial.print("Connecting to ");
    Serial.println(ssid);
  }
  
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  thisip = WiFi.localIP();

  if(DEBUG) {
    Serial.println("");
  
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println( thisip );
  }

  // sensor ID is the last byte in the IP quad
  heartware_id = thisip[3];

  Udp.begin(rxport);

  if(DEBUG) {
    Serial.println("Starting UDP");
    Serial.print("Local port: ");
    Serial.println(Udp.localPort());
  }
}
#endif

void setup() {
    Serial.begin(115200);
    while( !Serial ) {} // wait for serial init

    _LOG("initializing...");
//    lastreading = thisreading = 0;

    init_flappy_board();
    init_animator();

#ifdef __BUILD_FEATHER__
    init_wifi();
#endif
}

// ANIMATION ////////////////////////////////////////////////////////////
void animation_change(Animation *anim) {
  current = anim;
}

void animation_loop() {
  // no animation loaded, nothing to do here
  if (current == NULL) return;

  if( (current != NULL)
      && (current->ready()) ) {
    current->update();
    draw_coils();
  }

  if( current->finished() && current->must_loop() ) {
    current->reset();
  }
}

// MESSAGE HANDLERS /////////////////////////////////////////////////////
#ifdef __BUILD_FEATHER__
void on_sleep(OSCMessage &msg, int addrOffset) {
//  Serial.println("going to sleep to save battery...");
//  ESP.deepSleep(10 * 1000000);
}

void on_scene_1(OSCMessage &msg, int addrOffset) {
  if( msg.isInt(0) && msg.isInt(1) ) {
    int freq = msg.getInt(0);
    int repeat = msg.getInt(1);
    heartbeat.set_loop( repeat );
    heartbeat.frame( freq );
  }

  _LOG(">> scene 1");
  heartbeat.reset();
  animation_change((Animation *)&heartbeat);
}

void on_scene_2(OSCMessage &msg, int addrOffset) {
  if( msg.isInt(0) && msg.isInt(1) ) {
    int freq = msg.getInt(0);
    int repeat = msg.getInt(1);
    leftright.set_loop( repeat );
    leftright.frame( freq );
  }

  _LOG(">> scene 2");
  leftright.reset();
  animation_change((Animation *)&leftright);
}

void on_scene_3(OSCMessage &msg, int addrOffset) {
  if( msg.isInt(0) ) {
    int freq = msg.getInt(0);
    int repeat = msg.getInt(1);
    flutter.set_loop( repeat );
    flutter.frame( freq );
  }

  _LOG(">> scene 3");
  flutter.reset();
  animation_change((Animation *)&flutter);
}

void on_scene_4(OSCMessage &msg, int addrOffset) {
  if( msg.isInt(0) ) {
    int freq = msg.getInt(0);
    int repeat = msg.getInt(1);
    randombeat.set_loop( repeat );
    randombeat.frame( freq );
  }

  _LOG(">> scene 4");
  randombeat.reset();
  animation_change((Animation *)&randombeat);
}

void on_scene_5(OSCMessage &msg, int addrOffset) {
  if( msg.isInt(0) ) {
    int freq = msg.getInt(0);
    int repeat = msg.getInt(1);
    introvert.set_loop( repeat );
    introvert.frame( freq );
  }

  _LOG(">> scene 5");
  introvert.reset();
  animation_change((Animation *)&introvert);
}

void on_stop(OSCMessage &msg, int addrOffset) {
  _LOG(">> stop");
  current = NULL;
  reset_coils();
  update_coils();
  draw_coils();
}


void on_beat_single(OSCMessage &msg, int addrOffset) {
  _LOG(">> BEAT!");
}

void on_coil(OSCMessage &msg, int addrOffset) {
  int ftime;
  if( msg.isInt(0) && (msg.isInt(1)) ) {
    coil  = msg.getInt(0);
    ftime = msg.getInt(1);

    if(DEBUG) {
      Serial.print("beating flap #");
      Serial.println(coil);
    }

    if( (coil >= 0) && (coil < COILS) ) {
      _LOG("kick-out");
      coil_write(coil, 1);
      delay(ftime);
      _LOG("kick-in");
      coil_write(coil, 2);
      delay(ftime*2);
      _LOG("rest");
      coil_write(coil, 0);
      delay(ftime);
    } else {
      _LOG("flap number out of range");
    }
  } // if
}

void on_test_pattern(OSCMessage &msg, int addrOffset) {

  // deactivate any animation
  current = NULL;

  _LOG(">> test pattern");
  
  int latency = 10;
  
  if( msg.isInt(0) ) {
    latency = msg.getInt(0);
  }
  
  for(int i = 0; i < COILS; i++) {
    _LOG("coil ");
    _LOG(i);
    coil_write(i, 1);
    delay( latency );
    _LOG("kick-in");
    coil_write(i, 2);
    delay( latency*2 ); // nice if kick-back is x2 longer than kick-out
    _LOG("rest");
    coil_write(i, 0);
    delay(latency);

    delay(50);
  }
}

// //////////////////////////////////////////////////////////////
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
      in.route("/heartware/sleep", on_sleep);
      in.route("/heartware/testpattern", on_test_pattern);
      in.route("/heartware/beat", on_beat_single);
      in.route("/heartware/coil", on_coil);
      in.route("/heartware/scene/1", on_scene_1);
      in.route("/heartware/scene/2", on_scene_2);
      in.route("/heartware/scene/3", on_scene_3);
      in.route("/heartware/scene/4", on_scene_4);
      in.route("/heartware/scene/5", on_scene_5);
      in.route("/heartware/stop", on_stop);
    }

  } // if
}
#endif

void state_loop() {

  // send alive ACK message to show-control
  if(alive.check()) {
    _LOG("-> ACK");
    OSCMessage out("/heartware/ack");
    out.add( heartware_id );
    Udp.beginPacket(dest, txport);
    out.send(Udp);
    Udp.endPacket();
    out.empty();
  }

}


void loop() {
  state_loop();

  // update animation sequence
  animation_loop();
  // update physical coils
  update_coils();
//  sensor_loop();

#ifdef __BUILD_FEATHER__
  // run message pump
  osc_message_pump();
#endif
} // loop()

