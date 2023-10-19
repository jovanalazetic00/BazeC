#include "operacije.h"

FILE *otvoriDatoteku(char *filename) {
	FILE *fajl = fopen(filename, "rb+");
	if (fajl == NULL) {
		printf("Doslo je do greske! Moguce da datoteka \"%s\" ne postoji.\n", filename);
	} else {
		printf("Datoteka \"%s\" otvorena.\n", filename);
	}
	return fajl;
}

void kreirajDatoteku(char *filename) {
	FILE *fajl = fopen(filename, "wb");
	if (fajl == NULL) {
		printf("Doslo je do greske prilikom kreiranja datoteke \"%s\"!\n", filename);
	} else {
		BLOK blok;
		blok.slogovi[0].sifraLeta =  OZNAKA_KRAJA_DATOTEKE;
		fwrite(&blok, sizeof(BLOK), 1, fajl);
		printf("Datoteka \"%s\" uspesno kreirana.\n", filename);
		fclose(fajl);
	}
}

void dodajSlog(FILE *fajl, SLOG *slog) {
        if (fajl == NULL) {
            printf("Datoteka nije otvorena!\n");
            return;
        }

        SLOG *slogStari = pronadjiSlog(fajl, slog->sifraLeta);
        if (slogStari != NULL) {
            printf("Vec postoji slog sa tim evid brojem!\n");
            return;
        }

        BLOK blok;
        fseek(fajl, -sizeof(BLOK), SEEK_END); //u posljednji blok upisujemo novi slog
        fread(&blok, sizeof(BLOK), 1, fajl);

        int i;
        for (i = 0; i < FBLOKIRANJA; i++) {
            if (blok.slogovi[i].sifraLeta == OZNAKA_KRAJA_DATOTEKE) {  //slog sa oznakom kraja datoteke, tu treba upisati novi slog
                memcpy(&blok.slogovi[i], slog, sizeof(SLOG));
                break;
            }
        }
        i++;  //kraj datoteke

        if (i < FBLOKIRANJA) {  //jos uvijek ima mjesta u bloku, mozemo tu smjestiti slog sa oznakom kraja datoteke
            blok.slogovi[i].sifraLeta = OZNAKA_KRAJA_DATOTEKE;
            fseek(fajl, -sizeof(BLOK), SEEK_CUR);  //-siyeof vraca se na pocetak da ga procitam
            fwrite(&blok, sizeof(BLOK), 1, fajl);
            fflush(fajl);
        } else {
            fseek(fajl, -sizeof(BLOK), SEEK_CUR);
            fwrite(&blok, sizeof(BLOK), 1, fajl);
            BLOK noviBlok;
            noviBlok.slogovi[0].sifraLeta = OZNAKA_KRAJA_DATOTEKE;
            fwrite(&noviBlok, sizeof(BLOK), 1, fajl);
        }

        if (ferror(fajl)) {
            printf("Greska pri upisu u fajl!\n");
        } else {
            printf("Upis novog sloga zavrsen.\n");
        }
}
void ispisiSlog(SLOG *slog) {
	printf("%d           %d-%d-%d %d:%d          %s   %d             %d       %s \n",
        slog->sifraLeta,
		slog->datumVremePolaska.dan,
		slog->datumVremePolaska.mesec,
		slog->datumVremePolaska.godina,
		slog->datumVremePolaska.sati,
		slog->datumVremePolaska.minuti,
		slog->tipAviona,
		slog->trajanjeLeta,
		slog->udaljenostLeta,
		slog->mestoDolaska
		);
}


SLOG *pronadjiSlog(FILE *fajl, int s) {
	if (fajl == NULL) {
        printf("Datoteka nije otvorena!\n");
        return;
    }

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;

	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
			if (blok.slogovi[i].sifraLeta == OZNAKA_KRAJA_DATOTEKE) {
				return NULL;
			}

			if ( blok.slogovi[i].sifraLeta == s ) {
				SLOG *slog = (SLOG *)malloc(sizeof(SLOG));  //zauzmemo slog, upisemo i vratimo u povratnu vr
				memcpy(slog, &blok.slogovi[i], sizeof(SLOG));
				return slog;
			}
		}
	}

	return NULL;
}

void ispisiSveSlogove(FILE *fajl) {
	if (fajl == NULL) {
        printf("Datoteka nije otvorena!\n");
        return;
    }

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	int rbBloka = 0;
    printf("Sifra Leta-----Datum--------Tip Aviona-----Trajanje(min)----Udaljenost-----Odrediste\n");
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
			if (blok.slogovi[i].sifraLeta == OZNAKA_KRAJA_DATOTEKE) {
				printf("B%d S%d *\n", rbBloka, i);
                return;
			} else
			{
                ispisiSlog(&blok.slogovi[i]);
            }
		}

		rbBloka++;
	}
}

void obrisiSlogFizicki(FILE *fajl, int s) {
    if (fajl == NULL) {
        printf("Datoteka nije otvorena!\n");
        return;
    }
    SLOG *slog = pronadjiSlog(fajl, s);
    if (slog == NULL) {
        printf("Slog koji zelite obrisati ne postoji!\n");
        return;
    }
    BLOK blok, naredniBlok;
    int brisanje = s;
    fseek(fajl, 0, SEEK_SET);
    while (fread(&blok, 1, sizeof(BLOK), fajl)) {
        for (int i = 0; i < FBLOKIRANJA; i++) {

            if (blok.slogovi[i].sifraLeta == OZNAKA_KRAJA_DATOTEKE) {

                if (i == 0) {
                    printf("(skracujem fajl...)\n");
                    fseek(fajl, -sizeof(BLOK), SEEK_END);
					long bytesToKeep = ftell(fajl);
					ftruncate(fileno(fajl), bytesToKeep);
					fflush(fajl);
                }

                printf("Slog je fizicki obrisan.\n");
                return;
            }
            if (blok.slogovi[i].sifraLeta == brisanje) {

                for (int j = i+1; j < FBLOKIRANJA; j++) {
                    memcpy(&(blok.slogovi[j-1]), &(blok.slogovi[j]), sizeof(SLOG));
                }
                int podatakaProcitano = fread(&naredniBlok, sizeof(BLOK), 1, fajl);
                if (podatakaProcitano) {
                    fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                    memcpy(&(blok.slogovi[FBLOKIRANJA-1]), &(naredniBlok.slogovi[0]), sizeof(SLOG));
                    brisanje= naredniBlok.slogovi[0].sifraLeta;
                }

                fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                fwrite(&blok, sizeof(BLOK), 1, fajl);
                fflush(fajl);

                if (!podatakaProcitano) {
                    printf("Slog je fizicki obrisan.\n");
                    return;
                }

                break;
            }

        }
    }
}

int PronadjiNajbrziAvion(FILE *fajl){
    if (fajl == NULL) {
        printf("Datoteka nije otvorena!\n");
        return;
    }
    int najbrzi = 0;
    float brzina = 0;
	fseek(fajl, 0, SEEK_SET);
	BLOK blok;

	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
			if (blok.slogovi[i].sifraLeta == OZNAKA_KRAJA_DATOTEKE) {
				return najbrzi;
			}
            if((float)(blok.slogovi[i].udaljenostLeta/blok.slogovi[i].trajanjeLeta) > brzina){
                najbrzi = blok.slogovi[i].sifraLeta;
                brzina = (float)(blok.slogovi[i].udaljenostLeta/blok.slogovi[i].trajanjeLeta);
            }

		}
	}

	return NULL;
}

void DodajZvijezdicuNaLet(FILE *fajl){
    if(fajl == NULL){
        printf("Datoteka nije otvorena \n ");
        return;
    }

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	while (fread(&blok, sizeof(BLOK),1,fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
			if (blok.slogovi[i].sifraLeta ==  OZNAKA_KRAJA_DATOTEKE) {
                return;
			}

                if(blok.slogovi[i].datumVremePolaska.godina == 2021 && blok.slogovi[i].datumVremePolaska.mesec == 12 &&
                    blok.slogovi[i].datumVremePolaska.dan == 31 && blok.slogovi[i].trajanjeLeta > 1440 ){
                            strcat(blok.slogovi[i].mestoDolaska,"*");
                }

                fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                fwrite(&blok, sizeof(BLOK), 1, fajl);
                fseek(fajl, 0, SEEK_CUR);
		}
	}

}


void PrikazRazlicitihTipova(FILE *fajl)
{
    if (fajl == NULL) {
        printf("Datoteka nije otvorena!\n");
        return;
    }

    char lista[20][21];
    for(int k = 0;k<19;k++){
        strcpy(lista[k], "0");
    }
    int brojac = 0;
	int fleg = 0;


	fseek(fajl, 0, SEEK_SET);
	BLOK blok;

	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
			if (blok.slogovi[i].sifraLeta == OZNAKA_KRAJA_DATOTEKE)
                {
                        for(int k = 0;k<brojac;k++){
                        printf("ISPIS ZA MJESTO SLETANJA : %s \n",lista[k]);
                        IspisiTipoveAviona(fajl,lista[k]);
                        }
                    return;
                }
			for(int j = 0; j<19;j++)
            {
                if(strcmp(lista[j],blok.slogovi[i].mestoDolaska)==0){
                    fleg = 1;
                    printf("postoji vec");
                }
            }
            if(fleg ==0){
                printf("ne postoji vec \n");
                strcpy(lista[brojac],blok.slogovi[i].mestoDolaska);
                printf("Dodat je %s \n",lista[brojac]);
                brojac++;
            }
		}

    }
	return NULL;

}
void IspisiTipoveAviona(FILE *fajl,char *mjesto){

if (fajl == NULL) {
        printf("Datoteka nije otvorena!\n");
        return;
    }
	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
			if (blok.slogovi[i].sifraLeta == OZNAKA_KRAJA_DATOTEKE) {
				return;
			}
            if(strcmp(blok.slogovi[i].mestoDolaska,mjesto)==0){
                printf("Tip aviona je : %s \n",blok.slogovi[i].tipAviona);
            }

		}
	}

	return NULL;
}

