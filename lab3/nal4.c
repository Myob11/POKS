#include <stdio.h>
#include <unistd.h>


int main(){

    int hefePID = getpid();
    printf("jsm HEFE in moj PID je: %d\n", hefePID);

    for(int i = 0; i <=2; i++){
        if(getpid() == hefePID){
            __pid_t a =  fork();
            if (a == 0){
                printf("sm otrok moj PID = %d moj PPID = %d\n", getpid(), getppid());
            }
            else{
                printf("sm stars moj PID je: %d PIDotroka je: %d\n", getpid(), a);
            }
        }
    }

    sleep(1);

    return 0;

}