#include<stdio.h>
#include<malloc.h>
#include<string.h>

struct Echipa_fotbal {
	int id;
	int trofee;
	char* nume;
	float buget;
	char initiala;
};

struct Echipa_fotbal initializare(int id, int trofee, const char* nume, float buget, char initiala)
{
	struct Echipa_fotbal e;
	e.id = id;
	e.trofee = trofee;
	e.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy_s(e.nume, strlen(nume) + 1, nume);
	e.buget = buget;
	e.initiala = initiala;
	return e;
}

void afisare(struct Echipa_fotbal e) {
	if (e.nume != NULL)
	{
		printf("Echipa cu id-ul %d si initiala %c se numeste %s, detine %d trofee, iar pentru sezonul viitor are un buget de %5.2f milioane de euro. \n",
			e.id, e.initiala, e.nume, e.trofee, e.buget);
	}
	else
		printf("Echipa cu id-ul %d si initiala %c detine %d trofee, iar pentru sezonul viitor are un buget de %5.2f milioane de euro. \n",
			e.id, e.initiala, e.trofee, e.buget);

}

void afisareVector(struct Echipa_fotbal* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++)
	{
		afisare(vector[i]);
		printf("\n");
	}
}

struct Echipa_fotbal* copiazaPrimeleNElemente(struct Echipa_fotbal* vector, int nrElemente, int nrElementeCopiate) {
	struct Echipa_fotbal *vectorNou=NULL;
	vectorNou = (struct Echipa_fotbal*)malloc(sizeof(struct Echipa_fotbal) * nrElementeCopiate);
	for (int i = 0; i < nrElementeCopiate; i++)
	{
		vectorNou[i] = vector[i];
		vectorNou[i].nume = (char*)malloc(sizeof(char) * strlen(vector[i].nume) + 1);
		strcpy_s(vectorNou[i].nume, strlen(vector[i].nume) + 1, vector[i].nume);
	}
	return vectorNou;
}

void dezalocare(struct Echipa_fotbal** vector, int* nrElemente) {
	for (int i = 0; i < (*nrElemente); i++)
		if ((*vector)[i].nume != NULL)
			free((*vector)[i].nume);
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaEchipeFaraBani(struct Echipa_fotbal* vector, char nrElemente, float bugetMaxim, struct Echipa_fotbal** vectorNou, int* dimensiune) 
{
	(*dimensiune) = 0;
	for (int i = 0; i < nrElemente; i++)
		if (vector[i].buget <= bugetMaxim)
			(*dimensiune)++;
	if ((*vectorNou) != NULL)
		free(*vectorNou);
	*vectorNou = (struct Echipa_fotbal*)malloc(sizeof(struct Echipa_fotbal) * (*dimensiune));
	int j = 0;
	for(int i=0; i<nrElemente; i++)
		if (vector[i].buget <= bugetMaxim)
		{
			(*vectorNou)[j] = vector[i];
			(*vectorNou)[j].nume = (char*)malloc(sizeof(char) * strlen(vector[i].nume) + 1);
			strcpy_s((*vectorNou)[j].nume, strlen(vector[i].nume) + 1, vector[i].nume);
			j++;	
		}
}

struct Echipa_fotbal getPrimaEchipaCuNumele(struct Echipa_fotbal* vector, int nrElemente, const char* nume_cautat) {
	struct Echipa_fotbal e;
	e.nume = NULL;
	for(int i=0; i<nrElemente; i++)
		if (strcmp(vector[i].nume, nume_cautat) == 0)
		{
			e = vector[i];
			e.nume = (char*)malloc(strlen(nume_cautat) + 1);
			strcpy_s(e.nume, strlen(nume_cautat) + 1, vector[i].nume);
			return e;
		}

	return e;
}
	


int main() {

	struct Echipa_fotbal* echipa = NULL;
	int nrEchipe = 3;
	echipa = (struct Echipa_fotbal*)malloc(sizeof(struct Echipa_fotbal) * nrEchipe);
	echipa[0] = initializare(1, 73, "Juventus", 62.77, 'J');
	echipa[1] = initializare(2, 0, "Tottenham", 122.9, 'T');
	echipa[2] = initializare(3, 34, "Atletico Madrid", 89.11, 'A');

	afisareVector(echipa, nrEchipe);

	struct Echipa_fotbal* primeleEchipe = NULL;
	int nrPrimeleEchipe = 2;
	primeleEchipe = copiazaPrimeleNElemente(echipa, nrEchipe, nrPrimeleEchipe);
	printf("\n\nPrimele %d echipe: \n", nrPrimeleEchipe);
	afisareVector(primeleEchipe, nrPrimeleEchipe);

	dezalocare(&primeleEchipe, &nrPrimeleEchipe);
	afisareVector(primeleEchipe, nrPrimeleEchipe); //nu mai afiseaza nimic datorita dezalocarii

	printf("\n\nAfisare echipe sarace:\n");
	struct Echipa_fotbal* echipeSarace = NULL;
	int nrEchipeSarace = 0;
	copiazaEchipeFaraBani(echipa, nrEchipe, 100, &echipeSarace, &nrEchipeSarace);
	afisareVector(echipeSarace, nrEchipeSarace);
	dezalocare(&echipeSarace, &nrEchipeSarace);

	struct Echipa_fotbal echipaCautata;
	echipaCautata = getPrimaEchipaCuNumele(echipa, nrEchipe, "Juventus");
	printf("\n\nEchipa cautata: \n");
	afisare(echipaCautata);

	if (echipaCautata.nume != NULL)
	{
		free(echipaCautata.nume);
		echipaCautata.nume = NULL;
	}

	dezalocare(&echipa, &nrEchipe);

	return 0;
}