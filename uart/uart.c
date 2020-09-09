/*Contains very simple test implementations of UART RX & TX, to enable 'mainunit_comms.c'. Probabilities can be tuned in 'uart.h'. uart_get_line randomly picks among the two appropriate responses to model RU failure (probability can be set). Queries and possible replies on SUCCESS and FAILURE are defined in example_remote_data.h*/

#include <string.h> //for 'strcpy','strlen'
#include <stdlib.h> //for 'rand'
#include "uart.h"
#include "example_remote_data.h"

int uart_send_cmd (uint8_t *data, int len){
        int p_tx = rand() % PACKET_TRANSMISSION_PERCENT;
    
    if(p_tx < LINK_QUALITY_PERCENT){ ;
        return SUCCESS; //success
    }
    else return FAILURE;
}

int uart_get_line(uint8_t *data, int max_len){
    /* If succesfully received, add characters to *data, and returns the length of line received, which will have been copied to the buffer pointed to by data or returns 0 if a complete line has not yet been received. */
    
    //holds all queries and appropriate replies (4x2)
    char ** queries_replies[4]; //container size depends on the number of commands. Query at [n][0], Correct Reply at [n][1].
    queries_replies[0]=initial_query;
    queries_replies[1]=synchronised_sent;
    queries_replies[2]=set_clock;
    queries_replies[3]=get_part_number;

    //test - always give correct reply
    static int count = 0;
    
    int p_remote_fail = rand()%LINK_QUALITY_PERCENT;
    
    int reply=1; //correct replies (column 1)
    
    if(p_remote_fail<RU_RECEPTION_PERCENT){ //Something may go wrong at Remote Unit (10% probability);
        reply=2; //something failed (column 2)
    }
    
    int str_len = (int)strlen(queries_replies[count][reply]);
    uint8_t formatted_rx_data [str_len+3]; //make space for adding characters
    strcpy(formatted_rx_data,(uint8_t *)queries_replies[count][reply]);
    
    //CR LF would be also coming from the remote node - emulate here.
    formatted_rx_data[str_len]='\r';
    formatted_rx_data[str_len+1]='\n';
    formatted_rx_data[str_len+2]='\0';
	strcpy(data,formatted_rx_data);
    
    count++;
    if (count>3) count=0; /*goes from 0-3 and reset. Always gives the next reply. This does not account for Main Unit TX failures, since the Remote Unit is not aware of them. Therefore, it will cause innapropriate responses to be sent for queries of the Main Unit (Error 2).*/
    return str_len+2;
}
