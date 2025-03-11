#include<stdio.h>
#include<stdlib.h>
#include <string.h>

struct Echipa_fotbal {
	int id;
	int trofee;
	char* nume;
	float buget;
	char initiala;
};

struct Echipa_fotbal initializare(int id, int trofee, const char *nume, float buget, char initiala ) 
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

void modificaBuget(struct Echipa_fotbal *e, float buget_nou) {
	if (buget_nou > 0)
		e->buget = buget_nou;
}

void modificaNume(struct Echipa_fotbal* e, char* nume_nou) {
	if(strlen(e->nume) >= strlen(nume_nou))
		strcpy_s(e->nume, strlen(nume_nou) + 1, nume_nou);
	else
	{
		e->nume = (char*)realloc(e->nume, sizeof(char) * strlen(nume_nou) + 1);
		strcpy_s(e->nume, strlen(nume_nou) + 1, nume_nou);
	}
}

void dezalocare(struct Echipa_fotbal* e) {
	if (e->nume != NULL)
	{
		free(e->nume);
		e->nume = NULL;
	}
}

int main() {
	struct Echipa_fotbal e;
	e = initializare(1, 38, "Juventus", 67.8, 'J');
	modificaBuget(&e, 89.91);
	modificaNume(&e, "Juventus1897");
	afisare(e);
	dezalocare(&e);
	afisare(e);
	return 0;
}