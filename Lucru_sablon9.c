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
	m1.pret = atof(strtok(NULL, sep));
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

int calculeazaInaltimeArbore(Nod* arbore) {
	if (arbore) {
		int st = calculeazaInaltimeArbore(arbore->st);
		int dr = calculeazaInaltimeArbore(arbore->dr);
		if (st > dr) return 1 + st;
		else return 1 + dr;
	}
	else return 0;
}

int GradEchilibru(Nod* arbore) {
	return calculeazaInaltimeArbore(arbore->st) - calculeazaInaltimeArbore(arbore->dr);
}

void rotire2(Nod** arbore) {
	Nod* rad = (*arbore)->st;
	(*arbore)->st = rad->dr;
	rad->dr = (*arbore);
	(*arbore) = rad;
}

void rotire_2(Nod** arbore) {
	Nod* rad = (*arbore)->dr;
	(*arbore)->dr = rad->st;
	rad->st = (*arbore);
	(*arbore) = rad;
}

//void rotire2_1(Nod** arbore) {
//	rotire_2(&((*arbore)->st));
//	rotire2(arbore);
//}
//
//void rotire_21(Nod** arbore) {
//	rotire2(&((*arbore)->dr));
//	rotire_2(arbore);
//}

void adaugaMasinaInArboreEchilibrat(Nod**arbore,  Masina masinaNoua) {
	if (*arbore) {
		if (masinaNoua.id < (*arbore)->info.id){
			adaugaMasinaInArboreEchilibrat(&((*arbore)->st), masinaNoua);
		}
		else if (masinaNoua.id > (*arbore)->info.id) {
			adaugaMasinaInArboreEchilibrat(&((*arbore)->dr), masinaNoua);
		}
		if (GradEchilibru(*arbore) == 2) {
			if (GradEchilibru((*arbore)->st) == -1) {
				rotire_2(&((*arbore)->st));
			}
			rotire2(arbore);
		}
		else if (GradEchilibru(*arbore) == -2) {
			if (GradEchilibru(((*arbore)->dr)) == 1) {
				rotire2(&((*arbore)->dr));
			}
			rotire_2(arbore);
		}
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->st = NULL;
		nou->dr = NULL;
		(*arbore) = nou;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	Nod* arbore = NULL;
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		adaugaMasinaInArboreEchilibrat(&arbore, m);
	}
	fclose(f);
	return arbore;
}

void afisarePreOrder(Nod* arbore) {
	if (arbore) {
		afisareMasina(arbore->info);
		afisarePreOrder(arbore->st);
		afisarePreOrder(arbore->dr);
	}
}

void dezalocareArboreDeMasini(Nod** arbore) {
	if (*arbore) {
		dezalocareArboreDeMasini(&((*arbore)->st));
		dezalocareArboreDeMasini(&((*arbore)->dr));
		free((*arbore)->info.model);
		free((*arbore)->info.numeSofer);
		free(*arbore);
		(*arbore) = NULL;
	}
}

Masina getMasinaByID(Nod* arbore, int id) {
	Masina m;
	m.id = -1;
	m.numeSofer = "";
	m.model = "";
	if (arbore) {
		if (arbore->info.id > id) getMasinaByID(arbore->st, id);
		else if (arbore->info.id < id) getMasinaByID(arbore->dr, id);
		else return arbore->info;
	}
	else return m;
}

int determinaNumarNoduri(Nod* arbore) {
	if (arbore) {
		int st = determinaNumarNoduri(arbore->st);
		int dr = determinaNumarNoduri(arbore->dr);
		return st + dr + 1;
	}
	else return 0;
}

float calculeazaPretTotal(Nod* arbore) {
	if (arbore) {
		float st = calculeazaPretTotal(arbore->st);
		float dr = calculeazaPretTotal(arbore->dr);
		return st + dr + arbore->info.pret;
	}
	else return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* arbore, const char* numeSofer) {
	if (arbore) {
		float st = calculeazaPretulMasinilorUnuiSofer(arbore->st, numeSofer);
		float dr = calculeazaPretulMasinilorUnuiSofer(arbore->dr, numeSofer);
		if (strcmp(arbore->info.numeSofer, numeSofer) == 0) return st + dr + arbore->info.pret;
		else return st + dr;
	}
	else return 0;
}

int main() {
	Nod* arbore;
	arbore = citireArboreDeMasiniDinFisier("masini.txt");
	afisarePreOrder(arbore);
	printf("______________\n\n");

	Masina m = getMasinaByID(arbore, 10);
	afisareMasina(m);

	int nr = determinaNumarNoduri(arbore);
	printf("Numar de noduri: %d", nr);

	float pret1 = calculeazaPretTotal(arbore);
	printf("\n\nPret total: %.2f", pret1);

	float pret2 = calculeazaPretulMasinilorUnuiSofer(arbore, "Gigel");
	printf("\n\nPret sofer: %.2f\n\n", pret2);

	dezalocareArboreDeMasini(&arbore);
	return 0;
}