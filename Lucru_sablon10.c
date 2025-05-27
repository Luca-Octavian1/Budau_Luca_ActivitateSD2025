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

void pushStack(Nod** stack , Masina masina) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masina;
	nou->next = (*stack);
	*stack = nou;
}

Masina popStack(Nod** stack) {
	Masina m;
	m.id = -1;
	m.numeSofer = "";
	m.model = "";
	if (*stack) {
		Nod* vechi = *stack;
		m = vechi->info;
		*stack = vechi->next;
		free(vechi);
		return m;
	}
	else return m;
}

int isEmptyStack(Nod* stack) {
	if (stack != NULL) return 0;
	else return 1;
}

Nod* citireStackMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* stack = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		pushStack(&stack, m);
	}
	fclose(f);
	return stack;
}

void dezalocareStivaDeMasini(Nod** stack) {
	while (*stack) {
		Masina m = popStack(stack);
		free(m.model);
		free(m.numeSofer);
	}
}

int size(Nod* stack) {
	int i = 0;
	while (stack) {
		stack = stack->next;
		i++;
	}
	return i;
}

//QUEUE

struct NodDublu {
	Masina info;
	struct NodDublu* next;
	struct NodDublu* prev;
};
typedef struct NodDublu NodDublu;

struct Coada {
	NodDublu* first;
	NodDublu* last;
};
typedef struct Coada Coada;

void enqueue(Coada *coada, Masina masina) {
	NodDublu* nou = (NodDublu*)malloc(sizeof(NodDublu));
	nou->info = masina;
	nou->prev = NULL;
	if (coada->first) {
		nou->next = coada->first;
		coada->first->prev = nou;
	}
	else {
		nou->next = NULL;
		coada->last = nou;
	}
	coada->first = nou;
}

Masina dequeue(Coada *coada) {
	Masina m;
	if (coada->last) {
		NodDublu* vechi = coada->last;
		m = vechi->info;
		coada->last = vechi->prev;
		free(vechi);
		vechi = NULL;
		return m;
	}
	else {
		coada->first = NULL;
		m.id = -1;
		m.pret = 0;
		m.model = "";
		m.numeSofer = "";
		return m;
	}
}

Coada citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Coada coada;
	coada.first = NULL;
	coada.last = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		enqueue(&coada, m);
	}
	fclose(f);
	return coada;
}

void dezalocareCoadaDeMasini(Coada *coada) {
	while (coada->last) {
		Masina m = dequeue(coada);
		free(m.numeSofer);
		free(m.model);
	}
	coada->first = NULL;
}


//metode de procesare
Masina getMasinaByIDCoada(Coada *coada, int id) {
	int gasit = 0;
	Masina m;
	while (coada->first && gasit == 0) {
		m = dequeue(coada);
		if (m.id == id) {
			return m;
			gasit = 1;
		}
	}
	m.id = -1;
	m.numeSofer = "";
	m.serie = "";
	m.pret = 0;
	return m;
}

float calculeazaPretTotalCoada(Coada *coada) {
	float total = 0;
	while (coada->first) {
		Masina m = dequeue(coada);
		total += m.pret;
	}
	return total;
}

Masina getMasinaByIDStiva(Nod** stiva, int id) {
	Masina m;
	while (*stiva) {
		m = popStack(stiva);
		if (m.id == id) return m;
	}
	m.id = -1;
	m.model = "";
	m.numeSofer = "";
	return m;
}

float calculeazaPretTotalStiva(Nod** stiva) {
	float total = 0;
	while (*stiva) {
		Masina m = popStack(stiva);
		total += m.pret;
	}
}

int main() {
	//Nod* stack = citireStackMasiniDinFisier("masini.txt");
	//Masina m = getMasinaByIDStiva(&stack, 7);
	//printf("Masina gasita in coada cu id-ul intrdus este:\n");
	//afisareMasina(m);
	//float total2 = calculeazaPretTotalStiva(&stack);
	//printf("Pretul total din coada este: %.2f", total2);

	//
	//dezalocareStivaDeMasini(&stack);

	//afisareMasina(popStack(&stack));
	//afisareMasina(popStack(&stack));
	//afisareMasina(popStack(&stack));



	Coada coada = citireCoadaDeMasiniDinFisier("masini.txt");
	//afisareMasina(dequeue(&coada));
	//afisareMasina(dequeue(&coada));
	//afisareMasina(dequeue(&coada));
	Masina m2 = getMasinaByIDCoada(&coada, 7);
	printf("Masina gasita in coada cu id-ul intrdus este:\n");
	afisareMasina(m2);
	float total2 = calculeazaPretTotalCoada(&coada);
	printf("Pretul total din coada este: %.2f", total2);
	dezalocareCoadaDeMasini(&coada);
	return 0;
}