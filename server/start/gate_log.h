#define MAX_GATE 5

#define LOG_ENTRY_OK 0
#define LOG_ENTRY_UNCHANGED 1

uint16_t _gate_log[MAX_GATE];

void gate_log_reset() {
    memset(_gate_log, 0, sizeof _gate_log);
}

int gate_log_entry(uint8_t gate, uint16_t timing) {
    if (gate >= 0 && gate < MAX_GATE) {
        if (_gate_log[gate-1] == 0) {
            _gate_log[gate-1] = timing;
            return LOG_ENTRY_OK;
        }
        else return LOG_ENTRY_UNCHANGED;
    }
    else LOG_ENTRY_UNCHANGED;
}

void gate_log_to_csv(char * csv) {
    for (int i = 0; i < MAX_GATE; i++){
        sprintf(csv+i*6, "%05u,", _gate_log[i]);
    }
}

// typedef struct gate_log_t {
//    uint16_t timings[6];
//    int currentGate;
// } gate_log_t;



// const gate_log_t default_gate_log = {
//     .timings = {0, 0, 0, 0, 0, 0}, 
//     .currentGate = -1
// };



// void init_server_data(* gate_log_t server_data) {
//     *server_data = default_server_data;
// }