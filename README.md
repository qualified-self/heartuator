## Heartware
(cc) 2017 Luis Rodil-Fernandez & Jelle Reith

Firmware for the O/S wearable.

Made for the Adafruit Featherboard HUZZAH ESP8266 compiles on Arduino Nano and Pro too without the WiFi stuff.


### OSC message map

### sensing

Every time a heartbeat is detected this message gets sent to IP number 192.168.8.50

#### sends
message | params | description
-------------|-------------|----------|
/oximeter/beat | i <- id<br/>i <- IBI (miliseconds since last beat) | heartbeat detected

#### receives
message | params | description
-------------|-------------|----------|
/oximeter/reset | (none) | reset signal normalizer when a new person puts sensor on

### wearable

To see which animation runs in which scene see the dramaturgy document.

#### sends

The wearable sends an ACKNOWLEDGE message every 5-seconds or so, which is used to determine that it is alive and responding to messages. If the time since the last message is greater than 10 sec you can safely assume that the wearable isn't working well.

message | params | description
-------------|-------------|----------|
/heartware/ack | i <- wearable id | indicate that wearable is on

#### receives

message       | format | param         |
-------------|-------------|----------|
/heartware/sleep | i <- millis | put ESP8266 in energy saving mode for XYZ millis
/heartware/beat | i <- millis | run the "heartbeat" animation on the haptics (diastole, systole)
/heartware/coil | i <- coild id | activate the given coil in the wearable. (coil id's go from 0 to 5)
/heartware/scene/1 | i <- millis per frame | run animation in scene 1 at XYZ millis per frame
/heartware/scene/2 | i <- millis per frame | run animation in scene 2 at XYZ millis per frame
/heartware/scene/3 | i <- millis per frame | run animation in scene 3 at XYZ millis per frame
/heartware/scene/4 | i <- millis per frame | run animation in scene 4 at XYZ millis per frame
