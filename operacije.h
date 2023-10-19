#ifndef OPERACIJE_H
#define OPERACIJE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "slog.h"

FILE *otvoriDatoteku(char *filename);
void kreirajDatoteku(char *filename);
SLOG *pronadjiSlog(FILE *fajl, int sifraLeta);
void dodajSlog(FILE *fajl, SLOG *slog);
void ispisiSveSlogove(FILE *fajl);
void obrisiSlogFizicki(FILE *fajl, int sifraLeta);
int PronadjiNajbrziAvion(FILE *fajl);
void DodajZvijezdicuNaLet(FILE *fajl);
void IspisiTipoveAviona(FILE *fajl,char *mjesto);


#endif // OPERACIJE_H
