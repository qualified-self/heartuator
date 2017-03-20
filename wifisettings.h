const char* ssid = "otherself";
const char* pass = "theselfandtheother";

//byte myMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//byte myIp[]  = { 192, 168, 0, 177 };

WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
//const IPAddress outIp(192, 168, 0, 11);        // remote IP of your computer
const IPAddress outIp(10, 0, 0, 4);
const unsigned int outPort = 12345;          // remote port to receive OSC
const unsigned int localPort = 54321;        // local port to listen for OSC packets (actually not used for sending)
