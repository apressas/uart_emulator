#include <stdio.h>
#include <string.h>
#include "mainunit_comms.h"

/*Package and send data to UART*/
int send_data(char * data){
    int str_len = (int)strlen(data);
    char formatted_data[str_len+3];
    //strcpy(formatted_data,data);
    memcpy(formatted_data, data, str_len);
    formatted_data[str_len]='\r'; //CR -> first remove '\0' perhaps?
    formatted_data[str_len+1]='\n'; //NL
    formatted_data[str_len+2]='\0'; //terminate
    printf("TX: %s",formatted_data); //debug

    /* send command, returns 0 on success */
    if (uart_send_cmd(formatted_data, strlen(formatted_data)) != SUCCESS){
        return FAILURE; //Error 1: Transmission failed, do not proceed.
    }
    return SUCCESS; //Successful Transmission.
}

/*Handle data from UART, throw appropriate error if expected line is not received*/
int get_response(uint8_t *received_data, char * exp_data[]){

        int msg_len = uart_get_line(received_data, 40);
    
        for(int col=0; col<2; col++){ //col MAX depends on how many responses are possible per request
            int same=1;
            int exp_len=(int)strlen(exp_data[col+1]); //exp_data from the 'replies' column
            if(msg_len==(exp_len+2)){ //some basic error checking. no further checks unless lengths match
                for (int element=0; element<exp_len; element++){
                    if (exp_data[col+1][element]==received_data[element]){ //byte-by-byte error checking - any response that may be corrupted will be detected here. If not; then rx packet is fine -> proceed.
                    }
                    else{
                        same=0;  //the packets are different
                        break;
                    }
                }
            }
            else{
                same=0; //the packets are different
            }
            
            if (same==1) { //if message found in expected replies
                return col; //0/1 -> success, depends if found in col=2 or col=3
            }
        }

        return CORRUPTION; //Error: Cannot understand incoming instruction (unexpected/corrupted)
}

int start_comms()
{
    //queries and expected responses from Remote Unit
    char * initial_query [] = {"?","Synchronized","FAILED"};
    char * synchronised_sent [] = {"Synchronized","OK","FAILED"};
    char * set_clock [] = {"3600","OK","NOT_SET"};
    char * get_part_number [] = {"J","910213163","UNKNOWN"};
    char ** queries_replies[4];
    queries_replies[0] = initial_query;
    queries_replies[1] = synchronised_sent;
    queries_replies[2] = set_clock;
    queries_replies[3] = get_part_number;
    
    uint8_t Rx_message [50];

    for (int i=0; i<4; i++){
        //send next command
        if (send_data(queries_replies[i][0])!=0){
            /* if transmission unsuccessful*/
            printf("ERROR 1: (TX Failed) \"send_data()\" failed.\n");
            return FAILURE;
        }
        
        //handle response
        int status = get_response(&Rx_message, queries_replies[i]) ;
        
        if ( status == SUCCESS){
            printf("RX: %s",Rx_message);
        }
        else if (status == FAILURE){ //found in failed reply column
            printf("RX: %s",Rx_message);
            printf("ERROR 2: (RU Failed) Remote Unit could not complete request.\n");
            return STATUS_ERROR2;
        }
        else if (status == CORRUPTION){ //not found anywhere
            printf("ERROR 3: (RX Failed) RX Data does not match with expected (corrupted).\n");
            return STATUS_ERROR3;
        }
    }
    //connection success!
    return STATUS_CONNECTED;
}


