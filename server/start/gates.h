#include <Udp.h>

#define UDP_BUFFER_LEN 16

#define START_MESSAGE 0
#define STOP_MESSAGE 1

// IP address to send UDP data to.
// it can be ip address of the server or 
// a network broadcast address
// here is broadcast address
const char * udpAddress = "192.168.0.255";
const int udpPort = 44444;

//create UDP instance
WiFiUDP udp;

void init_udp() {
  //This initializes udp and transfer buffer
    udp.begin(50000);
}

void gate_command(uint8_t command) {
  udp.beginPacket(udpAddress, udpPort);
  udp.write(command);
  udp.endPacket();
}

uint8_t udp_receive(uint16_t * timing){
  //processing incoming packet, must be called before  reading the buffer
  if (udp.parsePacket() != 0) {
    uint8_t gate = udp.read();
    *timing = udp.read();
    *timing |= (udp.read() << 8);

    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.write(200);
    udp.endPacket(); //ack
    
    Serial.print("From gate"); Serial.print(gate); Serial.print(" : ");
    Serial.println(*timing);
    return gate;
  }
  return 0;
}