#include <Udp.h>

#define UDP_BUFFER_LEN 16

#define START_MESSAGE 0
#define STOP_MESSAGE 1
#define NO_MESSAGE 2

#define SEND_ACKED 0
#define SEND_NOT_ACKED 1

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
    udp.begin(udpPort);
}

int udp_send(uint16_t data) {
  // udp.beginPacket(udpAddress, udpPort);
  udp.beginPacket("192.168.0.76", 50000);
  udp.write(GATE_NUM);
  udp.write((uint8_t) (data & 0xff));
  udp.write((uint8_t) (data >> 8));
  udp.endPacket();

  delay(100);
  if (udp.parsePacket() != 0) {
    if (udp.read() == 200) return SEND_ACKED;
  }
  return SEND_NOT_ACKED;
}

int udp_receive(){
  //processing incoming packet, must be called before  reading the buffer
  if (udp.parsePacket() != 0) {
    int message = udp.read();
    return message;
  }
  else return NO_MESSAGE;
}