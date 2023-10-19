#include <stdio.h>
#include <stdlib.h>
#include "operacije.h"

FILE *otvaranje(){
    FILE *fajl = NULL;
    char filename[20];
    printf("Naziv datoteke : ");
    scanf("%s", &filename[0]);
    fajl = otvoriDatoteku(filename);
    return fajl;
}
void kreiranje(){
    char filename[20];
    printf("Naziv datoteke: ");
    scanf("%s", &filename[0]);
    kreirajDatoteku(filename);
}

void pretraga(FILE *fajl){
    int s;
    printf("Sifra leta : ");
    scanf("%d", &s);
    SLOG *slog = pronadjiSlog(fajl, s);
    if (slog == NULL) {
        printf("Trazeni slog ne postoji!\n");
    } else {
        printf("Sifra Leta-----Datum--------Tip Aviona-----Trajanje(min)----Udaljenost-----Odrediste\n");
        ispisiSlog(slog);
    }
}
void ispis_svih(FILE *fajl){
ispisiSveSlogove(fajl);
}
void dodavanje(FILE *fajl){
            SLOG slog;
            printf("Sifra  : ");
            scanf("%d", &slog.sifraLeta);
            printf("Tip aviona : ");
            scanf("%s", slog.tipAviona);
            printf("Trajanje leta (minute) : ");
            scanf("%d", &slog.trajanjeLeta);
            printf("Udaljenost leta : ");
            scanf("%d", &slog.udaljenostLeta);
            printf("Odrediste leta :");
            scanf("%s", slog.mestoDolaska);
            printf("Dan leta :");
            scanf("%d", &slog.datumVremePolaska.dan);
            printf("Mjesec leta :");
            scanf("%d", &slog.datumVremePolaska.mesec);
            printf("Godina leta :");
            scanf("%d", &slog.datumVremePolaska.godina);
            printf("Sati leta :");
            scanf("%d", &slog.datumVremePolaska.sati);
            printf("Minuti leta :");
            scanf("%d", &slog.datumVremePolaska.minuti);
            dodajSlog(fajl, &slog);
            printf("\n");
}

void brisanje(FILE *fajl){
    int sifra;
    printf("Unesi sifru: ");
    scanf("%d", &sifra);
    obrisiSlogFizicki(fajl, sifra);

}
void NajbrziAvion(FILE *fajl){
    int broj = PronadjiNajbrziAvion(fajl);
    SLOG *slog = pronadjiSlog(fajl, broj);
    printf("Sifra Leta-----Datum--------Tip Aviona-----Trajanje(min)----Udaljenost-----Odrediste\n");
    ispisiSlog(slog);
}

void DodajZvijezdicu(FILE *fajl){
    DodajZvijezdicuNaLet(fajl);
}


void KontrolnaTacka(FILE *fajl)
{
    PrikazRazlicitihTipova(fajl);
}

int main()
{
	int unos;
	FILE *fajl = NULL;

	while (1) {
		printf("Odaberite opciju:\n");
		printf("1 - Otvaranje datoteke\n");
		printf("2 - Formiranje datoteke\n");
		printf("3 - Pretraga datoteke\n");
		printf("4 - Unos sloga\n");
		printf("5 - Ispis svi slogova\n");
		printf("6 - Fizicko brisanje sloga\n");
		printf("7 - Najbrzi avion \n");
		printf("8 - Dodavanje zvijezdice \n");
		printf("9 - Kontrolna tacka \n");
		printf("0 - Izlaz\n");
		if (fajl == NULL) {
			printf("!!! PAZNJA: datoteka jos uvek nije otvorena !!!\n");
		}
		scanf("%d", &unos);
		getc(stdin);

		switch(unos) {
			case 1:
				{
					fajl = otvaranje();
					break;
				}
			case 2:
				{
					kreiranje();
					break;
				}
			case 3:
				{
					pretraga(fajl);
					printf("\n");
					break;
				}
			case 4:
				{
					dodavanje(fajl);
					break;
				}
			case 5:
				{
					ispisiSveSlogove(fajl);
					printf("\n");
					break;
				}
			case 6:
				{
					brisanje(fajl);
					break;
				}
			case 7:
				{
                    NajbrziAvion(fajl);
					break;
				}
			case 8:
				{
                    DodajZvijezdicu(fajl);
					break;
				}
            case 9:
				{
                    KontrolnaTacka(fajl);
					break;
				}
			case 0:
				{

					if (fajl != NULL) {
						fclose(fajl);
						return 0;
					}
				}
		}
	}

	return 0;

}
