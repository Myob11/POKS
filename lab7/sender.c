#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include "common.h"

int main(){
    mqd_t mq;
    char MESSAGE[MAX_SIZE] = "AAAAAAAAAAAAAAAAAAAAAAA";
    mq = mq_open(QUEUE_NAME, O_WRONLY);
    int i;
    for(i = 0; i < 15; i++){
        // sleep(1);
        mq_send(mq, MESSAGE, MAX_SIZE, 0);
        printf("Sent: %s\n", MESSAGE);
    }
    // zakljucimo z oddajo in posljemo msg_stop
    mq_send(mq, MSG_STOP, MAX_SIZE, 0);
    mq_close(mq);
    return 0;
}
