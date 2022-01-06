#define STATE_GATE_0 0
#define STATE_GATE_1 1
#define STATE_GATE_2 2
#define STATE_INIT 3

#define NO_COM 0
#define START_COUNT_COM 1
#define UPDATE_MONITOR_COM 2

void fsm(int * state, int in_gate, char * in_time, char * out_data, int * out_com) {
    *out_com = NO_COM;
    switch (*state) {
        case STATE_INIT:
            if (in_gate == 0) {
                *state = STATE_GATE_0;
                *out_com = START_COUNT_COM;
            }
            break;
        case STATE_GATE_0:
            switch (in_gate) {
                case 1:
                    *state = STATE_GATE_1;
                    *out_com = UPDATE_MONITOR_COM;
                    break;
                case 2:
                    *state = STATE_GATE_2;
                    strncat(out_data, ",", 1); // gate 1 skipped
                    *out_com = UPDATE_MONITOR_COM;
                    break;
            }
        case STATE_GATE_1:
            switch (in_gate) {
                case 2:
                    *state = STATE_GATE_2;
                    *out_com = UPDATE_MONITOR_COM;
                    break;
            }

    }

    // output
    switch (*state) {
        case STATE_INIT:
            strcpy(out_data, "init");
            break;
        case STATE_GATE_0:
            strcpy(out_data, "start");
            break;
        case STATE_GATE_1:
            strncpy(out_data, in_time, 5);
            strncat(out_data, ",", 1);
            break;
        case STATE_GATE_2:
            strncpy(out_data+5, in_time, 5);
            strncat(out_data, ",", 1);
            break;
    }

}