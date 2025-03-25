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

void afisareMasina(Masina masina) {
	printf("ID: %d \n", masina.id);
	printf("Numar usi: %d \n", masina.nrUsi);
	printf("Pret: %.2f \n", masina.pret);
	printf("Model: %s \n", masina.model);
	printf("Nume sofer: %s \n", masina.numeSofer);
	printf("Serie: %c \n", masina.serie);
	printf("\n");
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	for (int i = 0; i < nrMasini; i++)
		afisareMasina(masini[i]);
}

void adaugaMasinaInVector(Masina** masini, int * nrMasini, Masina masinaNoua) {
	Masina* aux = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1));
	for (int i = 0; i < (*nrMasini); i++)
	{
		aux[i] = (*masini)[i];
	}
	aux[(*nrMasini)] = masinaNoua;
	free(*masini);
	(*masini) = aux;
	(*nrMasini)++;
}

Masina citireMasinaFisier(FILE* file) {
	Masina m;
	char buffer[100];
	char sep[3] = ", \n";

	fgets(buffer, 100, file); //punem in buffer toata linia

	m.id = atoi(strtok(buffer, sep)); //atoi de la ASCII la Int
	m.nrUsi = atoi(strtok(NULL, sep));
	m.pret = atof(strtok(NULL, sep));

	char* aux;
	aux = strtok(NULL, sep);
	m.model = (char*)malloc(strlen(aux) + 1);
	strcpy(m.model, aux);

	aux = strtok(NULL, sep);
	m.numeSofer = (char*)malloc(strlen(aux) + 1);
	strcpy(m.numeSofer, aux);

	m.serie = strtok(NULL, sep)[0]; //returneaza inclusiv \n, de aceea luam [0]
	return m;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	FILE* fisier = fopen(numeFisier, "r");
	Masina* m = NULL;
	(*nrMasiniCitite) = 0;
	while (!feof(fisier))
	{
		adaugaMasinaInVector(&m, nrMasiniCitite, citireMasinaFisier(fisier));
	}
	fclose(fisier);
	return m;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	for (int i = 0; i < (*nrMasini); i++)
	{
		if ((*vector)[i].model != NULL)
			free((*vector)[i].model);
		if ((*vector)[i].numeSofer != NULL)
			free((*vector)[i].numeSofer);
	}
	free(*vector);
	(*vector) = NULL;
	(*nrMasini) = 0;
}

int main() {
	Masina* m;
	int nrMasini = 0;
	m = citireVectorMasiniFisier("masini.txt", &nrMasini);
	afisareVectorMasini(m, nrMasini);
	dezalocareVectorMasini(&m, &nrMasini);

	return 0;
}