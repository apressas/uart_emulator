#define PACKET_TRANSMISSION_PERCENT  95  //if 100, transmission from Main Unit always succeeds (ideal link).
#define RU_RECEPTION_PERCENT         10  //probability of something going wrong at the remote unit (0 for ideal).
#define LINK_QUALITY_PERCENT         100  //transmission success ratio, models lossy channel (100 for ideal).

enum {
    SUCCESS,
    FAILURE,
    CORRUPTION
};

enum{
    STATUS_CONNECTED,
    STATUS_ERROR1,
    STATUS_ERROR2,
    STATUS_ERROR3
};

typedef unsigned char uint8_t;

int uart_send_cmd (uint8_t *data, int len);

int uart_get_line(uint8_t *data, int max_len);
