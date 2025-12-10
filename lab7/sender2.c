#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include "common.h"

#define QUEUE_MAX_PRIO ((int)(9))

int main(){
    mqd_t mq;
    char MESSAGE[MAX_SIZE] = "AAAAAAAAAAAAAAAAAAAAAAA";
    char MESSAGEB[MAX_SIZE] = "BBBBBBBBBBBBBBBBBBBBBBB";

    mq = mq_open(QUEUE_NAME, O_WRONLY);
    int i, prio;
    for(i = 0; i < 15; i++){
        sleep(1);
        if(i < 10){
            mq_send(mq, MESSAGE, MAX_SIZE, 0);
            printf("Sent: %s with prio %d\n", MESSAGE, 0);
            continue;
        }
        else{
            mq_send(mq, MESSAGEB, MAX_SIZE, QUEUE_MAX_PRIO);
            printf("Sent: %s with prio %d\n", MESSAGEB, QUEUE_MAX_PRIO);
            continue;
        }
    }
    // zakljucimo z oddajo in posljemo msg_stop
    mq_send(mq, MSG_STOP, MAX_SIZE, 0);
    mq_close(mq);
    printf("sent: %s with prio %d\n", MSG_STOP, 0);
    return 0;
}
