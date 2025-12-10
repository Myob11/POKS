#include <stdio.h>
#include <unistd.h>

int main(){


    __pid_t pidOtrok = fork();

    if(pidOtrok == 0){
        printf("smOtrok moj PID je: %d, PPID je: %d\n", getpid(), getppid());
    }
    else{
        printf("sem stars moj PID je: %d in moj PPID je: %d\n", getpid(), getppid());
    }
    //printf("KONEC: mojPID = %d pidStarša = %d pidOtroka = %d\n", getpid(), getppid(), pidOtrok);

    sleep(1);

    int  PID = 10;
    fork();
    fork();
    fork();

    //dobimo 2^3 procesov torej 8 procesov!


    return 0;
}