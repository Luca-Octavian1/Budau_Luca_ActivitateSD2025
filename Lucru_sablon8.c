#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod {
	Masina info;
	struct Nod* st;
	struct Nod* dr;
};
typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}


void adaugaMasinaInArbore(Nod** radacina ,Masina masinaNoua) {
	if (*radacina) {
		if ((*radacina)->info.id > masinaNoua.id) {
			adaugaMasinaInArbore(&((*radacina)->st), masinaNoua);
		}
		else if ((*radacina)->info.id < masinaNoua.id) {
			adaugaMasinaInArbore(&((*radacina)->dr), masinaNoua);
		}
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->dr = NULL;
		nou->st = NULL;
		(*radacina) = nou;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		adaugaMasinaInArbore(&arbore, m);
	}
	fclose(f);
	return arbore;
}

void afisareMasiniDinArbore(/*arbore de masini*/) {
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere
}

void afisareInOrdine(Nod* arbore) {
	if (arbore) {
		afisareInOrdine(arbore->st);
		afisareMasina(arbore->info);
		afisareInOrdine(arbore->dr);
	}
}

void afisarePreOrdine(Nod* arbore) {
	if (arbore) {
		afisareMasina(arbore->info);
		afisarePreOrdine(arbore->st);
		afisarePreOrdine(arbore->dr);
	}
}

void afisarePostOrdine(Nod* arbore) {
	if (arbore) {
		afisarePostOrdine(arbore->st);
		afisarePostOrdine(arbore->dr);
		afisareMasina(arbore->info);
	}
}

void dezalocareArboreDeMasini(Nod** arbore) {
	if (*arbore) {
		dezalocareArboreDeMasini((&(*arbore)->st));
		dezalocareArboreDeMasini((&(*arbore)->dr));
		free((*arbore)->info.numeSofer);
		free((*arbore)->info.model);
		free(*arbore);
		*arbore = NULL;
	}
}

Masina getMasinaByID(Nod* arbore, int id) {
	Masina m;
	m.id = -1;
	if (arbore) {
		if (arbore->info.id == id) {
			return arbore->info;
		}
		else if (arbore->info.id > id) {
			getMasinaByID(arbore->st, id);
		}
		else {
			getMasinaByID(arbore->dr, id);
		}
	}
	else {
		return m;
	}
}

int determinaNumarNoduri(Nod* arbore) {
	if (arbore) {
		int st = determinaNumarNoduri(arbore->st);
		int dr = determinaNumarNoduri(arbore->dr);
		return 1 + st + dr;
	}
	else
	{
		return 0;
	}

}

int calculeazaInaltimeArbore(Nod* arbore) {
	if (arbore) {
		int st = calculeazaInaltimeArbore(arbore->st);
		int dr = calculeazaInaltimeArbore(arbore->dr);
		if (st > dr) return 1 + st;
		else return 1 + dr;
	}
	else return 0;
}

float calculeazaPretTotal(Nod* arbore) {
	if (arbore) {
		float s1=calculeazaPretTotal(arbore->st);
		float s2= calculeazaPretTotal(arbore->dr);
		return arbore->info.pret + s1 + s2;
	}
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* arbore, const char* numeSofer) {
	if (arbore) {
		float st = calculeazaPretulMasinilorUnuiSofer(arbore->st, numeSofer);
		float dr = calculeazaPretulMasinilorUnuiSofer(arbore->dr, numeSofer);
		if (strcmp(arbore->info.numeSofer, numeSofer) == 0) return arbore->info.pret + st + dr;
		else return st + dr;
	}
	else return 0;
}

int main() {
	Nod* arbore = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	float total = calculeazaPretTotal(arbore);
	printf("Totalul preturilor este: %.2f\n\n", total);

	int numar = determinaNumarNoduri(arbore);
	printf("Numarul de noduri este: %d\n\n", numar);

	int numarMax = calculeazaInaltimeArbore(arbore);
	printf("Inaltimea maxima este: %d\n\n", numarMax);

	float pretSofer = calculeazaPretulMasinilorUnuiSofer(arbore, "Gigel");
	printf("Pretul masinilor soferului ales este: %.2f\n\n", pretSofer);

	dezalocareArboreDeMasini(&arbore);
	return 0;
}