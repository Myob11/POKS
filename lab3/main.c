// PID --> process identifier
// PPID --> parent process identifier

//PID je int, če presežemo int vrednost ne moremo več ustvarjati procesov

// stanja procesov:
// 1. running
// waiting
// 3. stopped
// 4. zombie

// UID --> user identifier
// GUID --> group user identifier

// systemd je prastars, nima starsa

// pstree -p izpise drevo procesov s PID-ji
// ps -ef izpise vse procese, kater uporabnik jih je zagnal, PID, PPID
// samo 2 imata 0 za ppid in nimata starsa

// s forkanjem vedno podvojimo vse uporabljene vire
// ne preveckrat forkat 



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    printf("Proces: PID = %d PPID = %d\n", getpid(), getppid());
    sleep(100);
    return 0;

}
    




