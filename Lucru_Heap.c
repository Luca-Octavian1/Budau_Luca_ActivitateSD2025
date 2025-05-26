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

//creare structura pentru Heap
//un vector de elemente, lungimea vectorului si numarul de elemente din vector
struct Heap {
	int lungime;
	Masina* vector;
	int nrMasini;
};
typedef struct Heap Heap;

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

Heap initializareHeap(int lungime) {
	Heap h;
	h.lungime = lungime;
	h.nrMasini = 0;
	h.vector = (Masina*)malloc(sizeof(Masina)*lungime);
	return h;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozSt = pozitieNod * 2 + 1;
	int pozDr = pozitieNod * 2 + 2;
	int pozMax = pozitieNod;
	if (pozSt < heap.nrMasini && heap.vector[pozMax].pret < heap.vector[pozSt].pret) {
		pozMax = pozSt;
	}
	if (pozDr < heap.nrMasini && heap.vector[pozMax].pret < heap.vector[pozDr].pret) {
		pozMax = pozDr;
	}
	if (pozMax != pozitieNod) {
		Masina aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pozMax <= (heap.nrMasini - 2) / 2) {
			filtreazaHeap(heap, pozMax);
		}
	}
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Heap h=initializareHeap(10);

	while (!feof(f)) {
		h.vector[h.nrMasini++] = citireMasinaDinFisier(f);
	}
	fclose(f);

	for (int i = (h.nrMasini - 2) / 2; i>=0 ; i--) {
		filtreazaHeap(h, i);
	}

	return h;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrMasini; i++) {
		afisareMasina(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i = heap.nrMasini; i < heap.lungime; i++) {
		afisareMasina(heap.vector[i]);
	}
}

Masina extrageMasina(Heap* h) {
	Masina m;
	if (h->nrMasini > 0) {
		m = h->vector[0];
		h->vector[0] = h->vector[h->nrMasini-1];
		h->vector[h->nrMasini-1] = m;
		h->nrMasini--;
		filtreazaHeap(*h, 0);
	}
	return m;
}


void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->vector[i].model);
		free(heap->vector[i].numeSofer);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrMasini = 0;
}

int main() {
	Heap h = citireHeapDeMasiniDinFisier("masini.txt");
	afisareHeap(h);
	printf("_________________________\n");
	extrageMasina(&h);
	extrageMasina(&h);
	extrageMasina(&h);
	extrageMasina(&h);
	extrageMasina(&h);
	extrageMasina(&h);
	extrageMasina(&h);
	extrageMasina(&h);
	extrageMasina(&h);
	extrageMasina(&h);
	afiseazaHeapAscuns(h);
	dezalocareHeap(&h);
	return 0;
}