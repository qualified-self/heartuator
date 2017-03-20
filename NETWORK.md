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

#### ip addresses

The network for the installation is called "otherself" and the password is "theselfandtheother" both without quotes.

All IPs are statically mapped, this is the list of IPs.

If you have been conntected to a network called "otherself" before, you may have to renew your IP if you see that you are not automatically assigned the one in this list, [see this article that explains how to do that](http://osxdaily.com/2013/02/11/renew-dhcp-lease-mac-os-x/).

ip | device
-------------|-------------
192.168.8.1 | wifi router
192.168.8.100 | show-control mac mini
192.168.8.101 | control iPad
192.168.8.110 | luis' macbook
192.168.8.120 | tez' macbook
192.168.8.130 | chris' macbook
192.168.8.140 | garret's macbook
192.168.8.151 | sensor (id: 151)
192.168.8.152 | sensor (id: 152)
192.168.8.153 | sensor (id: 153) [backup]
192.168.8.154 | sensor (id: 154) [backup]
192.168.8.181 | wearable (id: 181)
192.168.8.182 | wearable (id: 182)
192.168.8.183 | wearable (id: 183) [backup]
192.168.8.184 | wearable (id: 184) [backup]

The logic of this configuration is that sensors start at 150 and are number in correlative order. Wearables start at 180 and are correlatively numbered. And each control machine (laptops and mini) get a multiple of 10 with any new devices in the control spectrum

OSC receive port is `12345`, sensors and wearable accept OSC messages on `54321`.

To change this configuration connect yourself to the "otherself" network and visit http://http://192.168.8.1/ in your browser. Text Luis for the admin password.
