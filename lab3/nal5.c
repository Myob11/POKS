#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(){

    const int N = 3;
    __pid_t root = getpid();

    for(int i = 0; i < N; i++){
        if(getpid() == root){
            __pid_t r = fork();
            if(r==0){
                sleep(i+1);
                printf("OTROK #%d PID = %d PPID = %d")
            }
        }
    }

    
}