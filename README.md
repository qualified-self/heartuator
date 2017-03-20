## Heartware
(cc) 2017 Luis Rodil-Fernandez & Jelle Reith

Firmware for the O/S wearable.

Made for the Adafruit Featherboard HUZZAH ESP8266 compiles on Arduino Nano and Pro too without the WiFi stuff.


### OSC message map

### sensing

Every time a heartbeat is detected this message gets sent to IP number 192.168.8.50

message | params | description
-------------|-------------|----------|
/oximeter/beat | i <- id<br/>i <- IBI (miliseconds since last beat) | heartbeat detected

### wearable

To see which animation runs in which scene see the dramaturgy document.

message       | format | param         |
-------------|-------------|----------|
/wearable/sleep | i <- millis | put ESP8266 in energy saving mode for XYZ millis
/wearable/beat | i <- millis | run the "heartbeat" animation on the haptics (diastole, systole)
/wearable/coil | i <- coild id | activate the given coil in the wearable. (coil id's go from 0 to 5)
/wearable/scene/1 | i <- millis per frame | run animation in scene 1 at XYZ millis per frame
/wearable/scene/2 | i <- millis per frame | run animation in scene 2 at XYZ millis per frame
/wearable/scene/3 | i <- millis per frame | run animation in scene 3 at XYZ millis per frame
/wearable/scene/4 | i <- millis per frame | run animation in scene 4 at XYZ millis per frame
