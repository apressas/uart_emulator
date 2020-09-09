#include "uart.h"

int send_data(char * data);

int get_response(uint8_t * received_data, char * exp_data[]);

int start_comms(void);
