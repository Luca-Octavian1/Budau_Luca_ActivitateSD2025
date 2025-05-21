#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

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

//STACK
struct Nod {
	Masina info;
	struct Nod* next;
};
typedef struct Nod Nod;

//Alegeti prin ce veti reprezenta stiva si creati structura necesara acestei stive
//putem reprezenta o stiva prin LSI, LDI sau vector

void pushStack(Nod** stiva, Masina masina) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masina;
	nou->next = *stiva;
	*stiva = nou;
}

Masina popStack(Nod** stiva) {
	if (*stiva) {
		Nod* temp = *stiva;
		Masina m = temp->info;
		*stiva = temp->next;
		free(temp);
		return m;
	}
	else return (Masina) { -1, 0, 0, NULL, NULL, '-' };
}

unsigned char isEmptyStack(Nod* stiva) {
	return stiva == NULL;
}

Nod* citireStackMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* stiva = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		pushStack(&stiva, m);
	}
	fclose(f);
	return stiva;
}

void dezalocareStivaDeMasini(Nod** stiva) {
	while (*stiva) {
		Masina m = popStack(stiva); //se realizeaza si deplasarea
		free(m.numeSofer);
		free(m.model);
	}
}

int size(Nod* stiva) {
	int count = 0;
	while (stiva) {
		count++;
		stiva = stiva->next;
	}
	return count;
}

//QUEUE
//Alegeti prin ce veti reprezenta coada si creati structura necesara acestei cozi
//putem reprezenta o coada prin LSI, LDI sau vector

struct NodDublu 
{
	struct NodDublu* prev;
	struct NodDublu* next;
	Masina info;
};
typedef struct NodDublu NodDublu;

struct Coada
{
	NodDublu* front;
	NodDublu* rear;
};
typedef struct Coada Coada;

void enqueue(Coada* coada, Masina masina) {
	NodDublu* nou = (NodDublu*)malloc(sizeof(NodDublu));
	nou->info = masina;
	nou->next = NULL;
	nou->prev = coada->rear;
	if (coada->rear) {
		coada->rear->next = nou;
	}
	else {
		coada->front = nou;
	}
	coada->rear = nou;
}

Masina dequeue(Coada* coada) {
	if (coada->front) {
		Masina m = coada->front->info;
		NodDublu* temp = coada->front;
		coada->front = coada->front->next;
		free(temp);
		if (coada->front == NULL) {
			coada->rear = NULL;
		}
		else {
			coada->front->prev = NULL;
		}
		return m;
	}
	else {
		Masina m;
		m.id = 1;
		return m;
	}
}

Coada citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Coada coada;
	coada.front = NULL;
	coada.rear = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		enqueue(&coada, m);
	}
	fclose(f);
	return coada;
}

void dezalocareCoadaDeMasini(/*coada*/) {
	//sunt dezalocate toate masinile si coada de elemente
}


//metode de procesare
Masina getMasinaByID(/*stiva sau coada de masini*/int id);

float calculeazaPretTotal(Nod** stiva) {
	Nod* temp = NULL;
	float suma = 0;
	while (!isEmptyStack(&stiva)) {
		Masina m = popStack(stiva);
		suma += m.pret;
		pushStack(&temp, m);
	}
	while (!isEmptyStack(temp)) {
		pushStack(stiva, popStack(&temp));
	}
	return suma;
}

int main() {
	//Nod* stiva = citireStackMasiniDinFisier("masini.txt");
	//afisareMasina(popStack(&stiva));
	//afisareMasina(popStack(&stiva));
	//afisareMasina(popStack(&stiva));
	
	Coada coada = citireCoadaDeMasiniDinFisier("masini.txt");
	afisareMasina(dequeue(&coada));



	return 0;
}