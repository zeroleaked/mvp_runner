// Import required libraries
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#include "gate_log.h"
#include "ext_AP.h"
#include "proximity.h"
#include "gates.h"

int restart_flag = 0;
int isCounting = 0;

#include "monitor.h"

void setup(){
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  // gate_log = default_gate_log;

  while (gate_init() != GATE_INIT_SUCCESS) {
      delay(1000);
      Serial.println("Initializing gate...");
    }

  connect_to_wifi();  

  init_udp();

  gate_log_reset();

  init_server();

  digitalWrite(LED_BUILTIN, HIGH); // Serving
}

void loop() {
  // 100 ms, Priority1
  ws.cleanupClients();

  // T < 10 ms, Priority3
  if (isCounting == 0) {
    if (gate_detect() == GATE_PASSED) {
      gate_command(START_MESSAGE);
      update_monitor();
      isCounting = 1;
    }
  }

  // 100 ms, Priority1
  if (restart_flag == 1) {
    gate_log_reset();
    restart_monitor();
    gate_command(STOP_MESSAGE);
    isCounting = 0;

    restart_flag = 0;
  }

  // 100 ms, Priority1
  uint16_t timing;
  int gate = udp_receive(&timing);
  if (gate != 0) {
    if (gate_log_entry(gate, timing) == LOG_ENTRY_OK) {
      update_monitor();
    }
  }

}
