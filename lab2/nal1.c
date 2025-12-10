#include <stdio.h>
#define max 20

int main() {

    char str[max];
    int starost;
    
    printf("Vnesi svojo starost:\n");
    scanf("%d", &starost); //postavimo se na lokacijo v spominu
    printf("Star si: %d\n", starost);

    printf("Vnesi svoje ime: ");
    fgets(str, max, stdin); // dolžina, max, iz kje beremo
    printf("Pozdravljen %s", str);

    printf("Vnesi svoje ime: ");
    scanf("%s", str);
    printf("\nŽivjo: %s", str);

    


    return 0;

}  
