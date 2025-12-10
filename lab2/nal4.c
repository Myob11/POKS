#include <stdio.h>

float izracun_bruto_neto(float bruto){
    float neto_znesek = bruto - bruto * 0.162;
    printf("Neto znesek je: %.2f€\n", neto_znesek);
    return neto_znesek;
}

int main(void){

    float bruto_znesek;
    float neto_znesek;
    float kraja;
    float dohodnina;

    printf("\n");
    printf("Vnesi bruto zaslužek: ");
    scanf("%f", &bruto_znesek);
    printf("\n------------------ IZRAČUN ZA LETO -----------------\n");

    neto_znesek = izracun_bruto_neto(bruto_znesek);
    
    if(neto_znesek > 78016.32){
        printf("Vsota je nad 78016.32€\n");
        float neki = neto_znesek - 78016.32;
        dohodnina = 24358.43 + 0.5*neki;
        printf("Dohodnina znaša: %.2f€\n", dohodnina);
        printf("Ni višjega dohodninskega razreda, čestitam kradejo te maksimalno!\n");
    }
    else if (neto_znesek > 54178.00){
        printf("Vsota je nad 54178.00€\n");
        float neki = neto_znesek - 54178.00;
        dohodnina = 15061.48 + 0.39*neki;
        float razlika = 78016.32 - neto_znesek;
        printf("Dohodnina znaša: %.2f€\n", dohodnina);
        printf("Razlika do naslednjega razreda znaša: %.2f€\n", razlika);
    }
    else if (neto_znesek > 27089.00){
        printf("Vsota je nad 27089.00€\n");
        float neki = neto_znesek - 27089.00;
        dohodnina = 6122.11 + 0.33*neki;
        float razlika = 54178.00 - neto_znesek;
        printf("Dohodnina znaša: %.2f€\n", dohodnina);
        printf("Razlika do naslednjega razreda znaša: %.2f€\n", razlika);
    }
    else if (neto_znesek > 9210.26){
        printf("Vsota je nad 9210.26€\n");
        float neki = neto_znesek - 9210.26;
        dohodnina = 1473.64 + 0.26*neki;
        float razlika = 27089.00 - neto_znesek;
        printf("Dohodnina znaša: %.2f€\n", dohodnina);
        printf("Razlika do naslednjega razreda znaša: %.2f€\n", razlika);
    }
    else{
        printf("Vsota je pod 9210.26€, dohodnine ni treba plačati!\n");
    }

    kraja = bruto_znesek*0.162 + dohodnina;

    printf("Študentski servis ti je ukradel %.2f€\n", kraja);
    printf("----------------------------------------------------\n");
    printf("\n");
    
    return 0;
}