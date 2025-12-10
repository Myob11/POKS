#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common.h"

int main(){
    mqd_t mq;
    struct mq_attr attr;
    char buffer[MAX_SIZE + 1];

    /* initialize the queue attributes */
    attr.mq_flags = 0; // damo v blocking stanje
    attr.mq_maxmsg = MAX_MSGS; // 10
    attr.mq_msgsize = MAX_SIZE; // 128
    attr.mq_curmsgs = 0; // čakalna vrsta je prazna

    /* create the message queue */
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
    sleep(2);
    while (1) {
        sleep(2);
        ssize_t bytes_read;
        bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL); /* receive message */
        printf("Received: %s\n", buffer);

        if(buffer[1] == MSG_STOP[1]){
            break;
        }
    }
    mq_close(mq); /* cleanup */
    mq_unlink(QUEUE_NAME);
    return 0;
    
}