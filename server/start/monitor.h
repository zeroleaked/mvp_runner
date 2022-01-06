// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void restart_monitor() {
  ws.textAll("init");
}

void onecast_monitor(uint32_t id) {
  if (isCounting == 1) {
    char csv[32];
    gate_log_to_csv(csv);
    ws.text(id, csv);
  }
  else {
    ws.text(id, "init");
  }
}

void update_monitor() {
  char csv[32];
  gate_log_to_csv(csv);
  ws.textAll(csv);
}

void update_monitor(char * log) {
  ws.textAll(log);
}

void handle_websocket(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "restart") == 0) {
      restart_flag = 1;
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      onecast_monitor(client->id());
      break;
    case WS_EVT_DISCONNECT:
      break;
    case WS_EVT_DATA:
      handle_websocket(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void init_server() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
  
  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });

  // Start server
  server.begin();
}