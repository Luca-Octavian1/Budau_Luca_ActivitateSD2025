#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct CursaAeriana
{
	char* cod;
	char* destinatie;
	char* data_plecare;
	char* ora_plecare;
	char* status;
};
typedef struct CursaAeriana cursaAeriana;

struct Nod {
	cursaAeriana info;
	struct Nod* next;
};
typedef struct Nod Nod;

void afisareCursa(cursaAeriana c) {
	printf("Cod: %s\n", c.cod);
	printf("Destinatie: %s\n", c.destinatie);
	printf("Data plecare: %s\n", c.data_plecare);
	printf("Ora plecare: %s\n", c.ora_plecare);
	printf("Status: %s\n\n", c.status);
}

void afisareLista(Nod* lista) {
	while (lista) {
		afisareCursa(lista->info);
		lista = lista->next;
	}
}

cursaAeriana citireDinFisier(FILE* fisier) {
	cursaAeriana c;
	char buffer[100];
	char delim[3] = {",\n"};
	fgets(buffer, 100, fisier);
	char *aux = strtok(buffer, delim);
	c.cod = malloc(strlen(aux) + 1);
	strcpy_s(c.cod, strlen(aux) + 1, aux);

	aux = strtok(NULL, delim);
	c.destinatie = malloc(strlen(aux) + 1);
	strcpy_s(c.destinatie, strlen(aux) + 1, aux);

	aux = strtok(NULL, delim);
	c.data_plecare = malloc(strlen(aux) + 1);
	strcpy_s(c.data_plecare, strlen(aux) + 1, aux);

	aux = strtok(NULL, delim);
	c.ora_plecare = malloc(strlen(aux) + 1);
	strcpy_s(c.ora_plecare, strlen(aux) + 1, aux);

	aux = strtok(NULL, delim);
	c.status = malloc(strlen(aux)+1);
	strcpy_s(c.status, strlen(aux)+1, aux);
	return c;
}

void adaugareElementInLista(Nod** lista, cursaAeriana c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = c;
	nou->next = NULL;
	if (*lista == NULL)
	{
		(*lista) = nou;
	}
	else {
		Nod* p = (*lista);
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
}

Nod* citireTotDinFisier(const char* numefisier) {
	FILE* f;
	Nod* lista=NULL;
	f = fopen(numefisier, "r");
	while (!feof(f)) {
		cursaAeriana c;
		c = citireDinFisier(f);
		adaugareElementInLista(&lista, c);
	}
	fclose(f);
	return lista;
}

void dezalocareNod(Nod** nod) {
	if ((*nod)->info.cod) {
		free((*nod)->info.cod);
	}
	if ((*nod)->info.destinatie) {
		free((*nod)->info.destinatie);
	}
	if ((*nod)->info.data_plecare) {
		free((*nod)->info.data_plecare);
	}
	if ((*nod)->info.ora_plecare) {
		free((*nod)->info.ora_plecare);
	}
	if ((*nod)->info.status) {
		free((*nod)->info.status);
	}
	free(*nod);
}

void dezalocareLista(Nod** lista) {
	while (*lista) {
		Nod* p = (*lista);
		(*lista) = (*lista)->next;
		dezalocareNod(&p);
	}
}

Nod* eliminareDinLista(Nod** lista, char* status_cautat) {
	while ((*lista) && strcmp((*lista)->info.status, status_cautat) == 0) {
		Nod* p = (*lista);
		(*lista) = p->next;
		dezalocareNod(&p);
	}
	if (*lista) {
		Nod* p = (*lista);
		while (p) {
			while (p->next && strcmp(p->next->info.status, status_cautat) != 0) {
				p = p->next;
			}
			if (p->next) {
				Nod* aux = p->next;
				p->next = aux->next;
				dezalocareNod(&aux);
			}
			else {
			p = NULL;
			}
		}
	}
}

void sortareDupaCod(Nod** lista) {
	int schimbat;
	do {
		schimbat = 0;
		Nod* p1 = (*lista);
		Nod* pred_p1 = NULL;

		while (p1 && p1->next) {
			Nod* p2 = p1->next;

			if (strcmp(p1->info.cod, p2->info.cod) > 0) {
				// Swap p1 și p2
				p1->next = p2->next;
				p2->next = p1;

				if (pred_p1) {
					pred_p1->next = p2;
				}
				else {
					*lista = p2; // am schimbat capul listei
				}

				// Acum pred_p1 devine p2, p1 rămâne
				pred_p1 = p2;
				schimbat = 1;
				// p1 rămâne pe loc, cu noul p1->next
			}
			else {
				// Avansez normal
				pred_p1 = p1;
				p1 = p1->next;
			}
		}
	} while (schimbat);
}


int main()
{
	Nod* lista = NULL;
	lista=citireTotDinFisier("curse.txt");
	afisareLista(lista);
	/*eliminareDinLista(&lista, "efectuata");
	printf("Dupa eliminarea curselor efectuate\n");
	afisareLista(lista);*/
	sortareDupaCod(&lista);
	printf("Dupa sortare\n");
	afisareLista(lista);
	dezalocareLista(&lista);
	return 0;
}