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
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

struct ListaDubla {
	Nod* first;
	Nod* last;
	int nrNoduri;
};
typedef struct ListaDubla ListaDubla;
 

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

void afisareListaMasiniDeLaInceput(ListaDubla lis) {
	Nod* p = lis.first;
	while (p) {
		afisareMasina(p->info);
		p = p->next;
	}
}

void afisareListaMasiniDeLaSfarsit(ListaDubla lis) {
	Nod* p = lis.last;
	while (p) {
		afisareMasina(p->info);
		p = p->prev;
	}
}

void adaugaMasinaInLista(ListaDubla *lis, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->prev = lis->last;
	if (lis->last != NULL)
		lis->last->next = nou;
	else
		lis->first = nou;
	lis->last = nou;
	lis->nrNoduri++;
}

void adaugaLaInceputInLista(ListaDubla* lis, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->prev = NULL;
	nou->next = lis->first;
	if (lis->first != NULL)
	{
		lis->first->prev = nou;
	}
	else
		lis->last = nou;
	lis->first = nou;
	lis->nrNoduri++;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	ListaDubla lis;
	lis.first = NULL;
	lis.last = NULL;
	lis.nrNoduri = 0;
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		adaugaMasinaInLista(&lis, m);
	}
	fclose(f);
	return lis;
}

void dezalocareLDMasini(ListaDubla* lis) {
	Nod* p = lis->first;
	while (p) {
		Nod* aux = p;
		p = p->next;
		if (aux->info.numeSofer)
			free(aux->info.numeSofer);
		if (aux->info.model)
			free(aux->info.model);
		free(aux);
	}
	lis->first = NULL;
	lis->last = NULL;
	lis->nrNoduri = 0;
}

float calculeazaPretMediu(ListaDubla lis) {
	float suma = 0;
	Nod* p = lis.first;
	while (p) {
		suma += p->info.pret;
		p = p->next;
	}
	if (lis.nrNoduri == 0)
		return 0;
	else
		return suma / lis.nrNoduri;
}

void stergeMasinaDupaID(ListaDubla* lis,int id) {
	if (lis->nrNoduri == 0){
		printf("Lista nu are elemente. \n");
		return;
	}
	Nod* p = lis->first;
	while (p != NULL && p->info.id != id){
		p = p->next;
	}
	if (p == NULL) {
		printf("Nu exista id-ul cautat.\n");
		return;
	}
	if (p->prev == NULL){
		lis->first = p->next;
		if (lis->first) {
			lis->first->prev = NULL;
		}
	}
	else {
		p->prev->next = p->next;
	}
	if (p->next != NULL){
		p->next->prev = p->prev;
	}
	else{
		lis->last = p->prev;
	}
	if (p->info.numeSofer)
		free(p->info.numeSofer);
	if (p->info.model)
		free(p->info.model);
	free(p);
	lis->nrNoduri--;
}

char* getNumeSoferMasinaScumpa(ListaDubla lis) {
	if (lis.nrNoduri == 0) {
		printf("Lista nu are elemente. \n");
		return;
	}
	Nod* p = lis.first;
	Nod* maxim = lis.first;
	while (p) {
		if (maxim->info.pret < p->info.pret)
		{
			maxim = p;
		}
		p = p->next;
	}
	return maxim->info.numeSofer;
}

int main() {
	char *nume;
	ListaDubla lis = citireLDMasiniDinFisier("masini.txt");
	printf("Afisare lista de la inceput:\n");
	afisareListaMasiniDeLaInceput(lis);

	stergeMasinaDupaID(&lis, 5);
	
	printf("Afisare lista de la final:\n");
	afisareListaMasiniDeLaSfarsit(lis);

	nume=getNumeSoferMasinaScumpa(lis);
	printf("Soferul cu cea mai scumpa masina este: %s\n", nume);
	

	printf("Pretul mediu este: %.2f\n", calculeazaPretMediu(lis));

	dezalocareLDMasini(&lis);
	return 0;
}