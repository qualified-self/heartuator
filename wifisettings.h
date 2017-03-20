const char* ssid = "otherself";
const char* pass = "theselfandtheother";

WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(192, 168, 8, 100);
const unsigned int outPort = 12345;          // remote port to receive OSC
const unsigned int localPort = 54321;        // local port to listen for OSC packets (actually not used for sending)
IPAddress thisip;

