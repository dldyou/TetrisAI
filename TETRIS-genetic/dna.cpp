#include <stdlib.h>
#include "dna.h"
#define POPULATION 50
#define DNACNT 8
#define MUTATION_RATE 0.002

DNAW dna[POPULATION];
DNAW bestdna;

double GetW(int idx) {
	return bestdna.weight[idx];
}

void DNA::GenerateNew(void) {
	for (int i = 0; i < POPULATION; i++)
		for (int j = 0; j < DNACNT; j++)
			dna[i].weight[j] = (double)(rand() % RAND_MAX - RAND_MAX / 2) / (RAND_MAX / 2);
}

void DNA::best(void) {
	for (int i = 0; i < POPULATION; i++)
		for (int j = 0; j < DNACNT; j++)
			dna[i].weight[j] = (double)(rand() % RAND_MAX - RAND_MAX / 2) / (RAND_MAX / 2);
}

DNAW DNA::CrossOver(int parent1, int parent2) {
	DNAW child;
	int divP = rand() % DNACNT;
	for (int i = 0; i < DNACNT; i++) {
		if (i < divP) child.weight[i] = dna[parent1].weight[i];
		else child.weight[i] = dna[parent2].weight[i];
	}
	return child;
}

DNAW DNA::Recombination(int parent1, int parent2) {
	DNAW child;
	for (int i = 0; i < DNACNT; i++)
		child.weight[i] = (dna[parent1].weight[i] + dna[parent2].weight[i]) / 2;
	return child;
}

DNAW DNA::MutationTransform(int parrent) {
	DNAW child = dna[parrent];
	for (int i = 0; i < DNACNT; i++) {
		double r = (double)rand() / RAND_MAX;
		if (r < MUTATION_RATE)
			child.weight[i] = (double)(rand() % RAND_MAX - RAND_MAX / 2) / (RAND_MAX / 2);
	}
	return child;
}

DNAW DNA::MutationExchange(int parrent) {
	DNAW child = dna[parrent];
	for (int i = 0; i < DNACNT; i++) {
		for (int j = i + 1; j < DNACNT; j++) {
			double r = (double)rand() / RAND_MAX;
			if (i == j) continue;
			if (r < MUTATION_RATE) {
				double temp = child.weight[i];
				child.weight[i] = child.weight[j];
				child.weight[j] = temp;
			}
		}
	}
	return child;
}