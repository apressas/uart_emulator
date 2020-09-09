#include <stdio.h>
#include <ctype.h> //for 'isdigit'
#include "mainunit_comms.h"

/*Test. Tune probabilities of TX/RU error in 'uart.h'. Compile with '$gcc test.c mainunit_comms.c uart.c'*/

int main() {
    while(1){
        char line[4];         /* a line to be read from the terminal */
        int attempts=1;           /* ..converted to integer */
        printf("\n************************************************************");
        printf("\nMain Unit -> Remote Unit:\nPlease enter number of connection attempts and press Enter. (0-99, default=1)\n");
 
        fgets (line, sizeof(line), stdin);
        if(isdigit(line[0])) sscanf (line, "%d", &attempts);
        else attempts=1;
        
        //statistics
        int n_connected = 0;
        int n_failed = 0;
        int n_error1 = 0;
        int n_error2 = 0;
        int n_error3 = 0;

        for(int attempt=0; attempt<attempts; attempt++){
            int comms_stat = -1;

            comms_stat = start_comms();
    
            if(comms_stat==STATUS_CONNECTED){
                printf("STATUS: Connected\n\n");
                n_connected++;
            }
            else if(comms_stat==STATUS_ERROR1){
                printf("STATUS: Connection Attempt Failed (Error 1)\n\n");
                n_failed++;
                n_error1++;
            }
            else if(comms_stat==STATUS_ERROR2){
                printf("STATUS: Connection Attempt Failed (Error 2)\n\n");
                n_failed++;
                n_error2++;
            }
            else if(comms_stat==STATUS_ERROR3){
                printf("STATUS: Connection Attempt Failed (Error 3)\n\n");
                n_failed++;
                n_error3++;
            }
        }
        
        printf("\nNoOfAttempts: %d | Success Rate: %.3f | Error 1 (TX) Rate: %.3f |\n Error 2 (RU) Rate: %.3f | Error 3 (RX) Rate %.3f \n",attempts, (float)n_connected/(float)attempts,(float)n_error1/(float)attempts,(float)n_error2/(float)attempts,(float)n_error3/(float)attempts);
    }
    return 0;
}
