#include <WiFi.h>
#include <WebSocketsClient.h>

#define GATE_NUM 2

#include "gates.h"

#define INPUT_PIN 15

volatile uint16_t csec;
 
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

int counting = 0;
 
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  csec++;
  portEXIT_CRITICAL_ISR(&timerMux);
 
}

const char* ssid = "SSID";
const char* password = "PW";

void setup() {

 
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println(WiFi.localIP());

  timer = timerBegin(0, 8, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 100000, true);
  timerAlarmEnable(timer);
 
  init_udp();

  digitalWrite(LED_BUILTIN, HIGH); // Standby
}
 
void loop() {
  // T < 10 ms
  if (counting == 1) {

    if (digitalRead(INPUT_PIN) == LOW) {
      uint16_t result;

      portENTER_CRITICAL(&timerMux);
      result = csec;
      portEXIT_CRITICAL(&timerMux);
      
      while (udp_send(result)) {
        delay(500);
        Serial.println("Resending timing...");
      }
      
      Serial.print("Sent ");Serial.println(result);
      counting = 0;
      digitalWrite(LED_BUILTIN, HIGH); // Standby
    }
  }

  // T < 10 ms
  int message = udp_receive();
  switch (message) {
    case START_MESSAGE:
      counting = 1;

      portENTER_CRITICAL(&timerMux);
      csec = 0;
      portEXIT_CRITICAL(&timerMux);

      timerWrite(timer, 0);

      digitalWrite(LED_BUILTIN, LOW); // Counting
      break;
    case STOP_MESSAGE:
      counting = 0;
      digitalWrite(LED_BUILTIN, HIGH); // Standby
      break;
  }
}
