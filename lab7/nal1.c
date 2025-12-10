

// nitni buffer

// zadnji element v bufferju nam pove ali je cakalna vrsta prazna ali polna, vedno moramo
// enega žrtvovati

#include <stdio.h> 
#include <pthread.h> 
#include <string.h> 
#include <unistd.h>

#define MAX_MSGS 10 
#define MAX_MSG_LEN 100 
void Fun1 (void); 
void Fun2 (void); // manjka2
 
char Vrsta[MAX_MSGS][MAX_MSG_LEN]; 
int Vpis = 0; 
int Branje = 0; 
 
int main () 
{ 
  pthread_t thread1, thread2; 
  pthread_create ( &thread1, NULL, (void *) Fun1, NULL); 
  pthread_create ( &thread2, NULL, (void *) Fun2, NULL); // manjka2
  pthread_join (thread1, NULL); 
  pthread_join (thread2, NULL); // manjka2
  printf("Konec  \n"); 
} 
 
void Fun1(void) 
// pisanje v vrsto
{ 
  char MESSAGE [MAX_MSG_LEN] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\0"; 
  char MESSAGEX [MAX_MSG_LEN] = "XXXXXXXXXXXXXXXXX\0"; 
  int i; 
 
  Vpis = 0; 
  Branje = 0; 
  printf("Thread1 started - sender \n"); 
  for (i=0; i<15; i++) 
  { 
    sleep(1); 
    while ( ( (Vpis + 1) % MAX_MSGS)  == Branje);  /* cakaj - vrsta polna*/ 
    // vpis + 1 nam pove ce je vrsta polna
    strcpy(Vrsta[Vpis], MESSAGE); 
    printf("Thread1 sending %d: %s \n",i, (char *) & MESSAGE); 
    Vpis = (Vpis + 1) % MAX_MSGS; // premaknemo kazalec za 1
    MESSAGE[0]++; // spremeni prvo crko v sporočilu a,b,c...
  } 
  printf("Thread1 sending %d: %s \n",i, (char *) & MESSAGEX); 
  strcpy(Vrsta[Vpis], MESSAGEX); 
  Vpis = (Vpis + 1) % MAX_MSGS; 
  printf("Thread1 ended \n"); 
  // samo 1x posljemo X-e
} 
 
void Fun2(void) 
// branje iz vrste
{ 
  char msgBuff[MAX_MSG_LEN];  /* local received message */ 
 
  printf("Thread2 started - receiver \n"); 
  sleep(5); 
  while(1) 
  { 
    while (Vpis == Branje);  /* cakaj - vrsta prazna*/ 
    strcpy(msgBuff, Vrsta[Branje]); // manjka3 strcpy
    printf("Thread2 received: %s Branje=%d Vpis=%d\n",msgBuff, Branje, Vpis); 
    Branje = (Branje + 1) % MAX_MSGS; // manjka 3 
    // premaknemo kazalec za 1
    if (msgBuff[0] == 'X') 
    { 
      printf("Thread2 ended \n"); 
      break; // manjka4
    }
    sleep(1); // manjka5
  } 
}
