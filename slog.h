#define FBLOKIRANJA 3
#define OZNAKA_KRAJA_DATOTEKE -1
typedef struct DateTime {
	int dan;
	int mesec;
	int godina;
	int sati;
	int minuti;
} DATETIME;

typedef struct {
int sifraLeta;
DATETIME datumVremePolaska;
char tipAviona[7];
int trajanjeLeta;
int udaljenostLeta;
char mestoDolaska[21];

} SLOG;


typedef struct Blok {
SLOG slogovi[FBLOKIRANJA];
} BLOK;
