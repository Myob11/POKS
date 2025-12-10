
#include <stdio.h>
#include <unistd.h>

int main(){

    printf("START moj PID = %d\n", getpid());

    int level = 0;
    int id = 0; // binarni ID

    __pid_t a = fork();
    if(a == 0){level++;id=(id<<1)|1;}else{id=(id<<1)|0;}

    __pid_t b = fork();
    if(b == 0){level++;id=(id<<1)|1;}else{id=(id<<1)|0;}

    __pid_t c = fork();
    if(c == 0){level++;id=(id<<1)|1;}else{id=(id<<1)|0;}

    for(int i = 0; i < level; i++){
        printf("      ");
    }
    
    printf("NODE_ID=%d PID=%d PPID=%d level=%d\n" , id, getpid(), getppid(), level);

    sleep(1);
    
    return 0;

}