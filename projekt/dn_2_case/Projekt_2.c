/*
Namen projekta je: 
• Postaviti socket strežnik za generator serijskih števik 
 
Implementirajte univerzalni unikatni identifikator 
Postavite strežnik po navodilih iz vaje socket. Strežnik postavite na port 5069. Strežnik naj 
sledi naslednjim navodilom. 
• Na  prejeto  sporočilo  »GET«  naj  odgovori  s  zaporedno  številko  in  serijsko  številko. 
Serijski  številki  dodajte  presledek  in  CRC32  kodo  zaporedne  in  serijske  številke 
(uporabite  kodo  iz  1.  dela  reševanja  problema). 
Na primer: 00000001-xxxxxxxx-xxxx-Mxxx-Nxxx-xxxxxxxxxxxx 86E0E903 
• Odjemalec bo na to sporočilo odgovoril s nekim tekstom, na primer: 
»PREJETO  D5A07B2F«,  torej  TEXT  in  CRC32  koda  teksta.  Vaš  strežnik  mora 
obravnavati dve možnosti: 
o CRC32 koda spročila je pravilna: v tem primeru, si povečate števec serijskih 
številk. 
o CRC32  koda  sporočila  ni  pravilna:  odgovorite  s  sporočilom  »NAPAKA 
4900B4DB«. Števca serijskih številk v tem primeru ne povečate. 
• Na  kakršnokoli  drugo  sporočilo  odjemalca  naj  strežnik  odgovori  s  sporočilom 
»NEPREPOZNAVEN UKAZ«. 
*/

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

