#include <stdlib.h>
#include <Windows.h>
#include "dna.h"
#include <stdio.h>
#define POPULATION 50
#define NEW 5
#define DNACNT 8
#define MUTATION_RATE 0.0015
#define DIV 4

DNAW dna[POPULATION];
DNAW bestdna;
int generation = 1;
int bestdnaidx = 0;
int count = 1;

int GetIdx(void) {
	return bestdnaidx;
}

int GetP(void) {
	return POPULATION;
}

int GetG(void) {
	return generation;
}

double GetW(int idx) {
	return bestdna.weight[idx];
}

int GetL(void) {
	return bestdna.line / count;
}

int GetLT(int parent) {
	return dna[parent].line;
}

int GetBT(int parent) {
	return dna[parent].block;
}

int GetB(void) {
	return bestdna.block / count;
}

int GetC(void) {
	return count;
}

int GetIdxB(void) {
	int b = -1, idx = 0;
	for (int i = 0; i < POPULATION; i++) {
		if (dna[i].line > b) {
			b = dna[i].line;
			idx = i;
		}
	}
	return idx;
}

double GetWT(int parent, int idx) {
	return dna[parent].weight[idx];
}

void AddScore(int parent, int score) {
	dna[parent].line += score;
}

void AddBlock(int parent) {
	dna[parent].block++;
}

void DNA::GenerateNew(void) {
	for (int i = 0; i < POPULATION; i++) {
		for (int j = 0; j < DNACNT; j++) {
			dna[i].weight[j] = (double)(rand() % RAND_MAX - RAND_MAX / 2) / (RAND_MAX / 2);
			dna[i].line = 0;
			dna[i].block = 0;
		}
	}
}

int DNA::LoadFile(void) {
	FILE* fp = fopen("dna.txt", "r");
	if (fp == NULL) return 0;
	else {
		fscanf(fp, "%d\n", &generation);
		fscanf(fp, "%d\n", &bestdnaidx);
		for (int i = 0; i < POPULATION; i++) {
			for (int j = 0; j < DNACNT; j++)
				fscanf(fp, "%lf\n", &dna[i].weight[j]);
		}
		for (int i = 0; i < DNACNT; i++)
			fscanf(fp, "%lf\n", &bestdna.weight[i]);
	}
	fclose(fp);
	return 1;
}

void DNA::SaveFile(void) {
	FILE* fp = fopen("dna.txt", "w");
	fprintf(fp, "%d\n", generation);
	fprintf(fp, "%d\n", bestdnaidx);
	for (int i = 0; i < POPULATION; i++) {
		for (int j = 0; j < DNACNT; j++)
			fprintf(fp, "%lf\n", dna[i].weight[j]);
	}
	for (int i = 0; i < DNACNT; i++)
		fprintf(fp, "%lf\n", bestdna.weight[i]);
	fclose(fp);
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

DNAW DNA::CrossOver2(int parent1, int parent2) {
	DNAW child;
	int P = rand() % 100;
	for (int i = 0; i < DNACNT; i++) {
		if (P < 50) child.weight[i] = dna[parent1].weight[i];
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

DNAW DNA::MutationLittle(int parrent) {
	DNAW child = dna[parrent];
	for (int i = 0; i < DNACNT; i++) {
		double r = (double)rand() / RAND_MAX;
		if (r < MUTATION_RATE * 10) {
			double rn = 1 + (double)(rand() % RAND_MAX - RAND_MAX / 2) / (RAND_MAX / 2) / 10;
			child.weight[i] *= rn;
			if (child.weight[i] > 1) child.weight[i] = 1;
			else if (child.weight[i] < -1) child.weight[i] = -1;
		}
	}
	return child;
}

DNAW DNA::MutationReverse(int parent) {
	DNAW child = dna[parent];
	double r = (double)rand() / RAND_MAX;
	if (r < MUTATION_RATE) {
		int div1 = rand() % DNACNT;
		int div2 = rand() % DNACNT;
		if (div1 > div2) {
			int t = div1;
			div1 = div2;
			div2 = t;
		}
		for (int i = div1; i < div2; i++)
			child.weight[i] = dna[parent].weight[div2 - i];
	}
	return child;
}

DNAW DNA::MutationNew(int parent) {
	DNAW child = dna[parent];
	for (int i = 0; i < DNACNT; i++) {
		double r = (double)rand() / RAND_MAX;
		if(r < MUTATION_RATE) child.weight[i] = (double)(rand() % RAND_MAX - RAND_MAX / 2) / (RAND_MAX / 2);
	}
	return child;
}

void NextGeneration(void) {
	int idx[POPULATION];
	generation++;
	DNA _DNA = DNA();
	for (int i = 0; i < POPULATION; i++) idx[i] = i;

	for (int i = 0; i < POPULATION; i++) {
		for (int j = i + 1; j < POPULATION; j++) {
			if (i == j) continue;
			if (dna[idx[i]].line < dna[idx[j]].line) {
				int t = idx[i];
				idx[i] = idx[j];
				idx[j] = t;
			}
		}
	}
	for (int i = 0; i < POPULATION; i++) {
		for (int j = i + 1; j < POPULATION; j++) {
			if (i == j) continue;
			if (dna[idx[i]].block < dna[idx[j]].block) {
				int t = idx[i];
				idx[i] = idx[j];
				idx[j] = t;
			}
		}
	}
	for (int i = 0; i < DNACNT; i++) {
		bestdna.weight[i] = dna[idx[0]].weight[i];
		bestdna.line = dna[idx[0]].line;
		bestdna.block = dna[idx[0]].block;
	}
	int fitness[DIV] = { 0 };
	int fit = 0;
	for (int i = 0; i < DIV; i++) {
		if (dna[idx[0]].line == 0) fitness[i] = ((double)dna[idx[i]].block / dna[idx[0]].block) * 100;
		else fitness[i] = ((double)dna[idx[i]].line / dna[idx[0]].line + (double)dna[idx[i]].block / dna[idx[0]].block) * 50;
		fit += fitness[i];
	}
	for (int i = DIV; i < POPULATION; i++) {
		int r = rand() % 100;
		int r2 = rand() % 100;
		int p1 = rand() % fit;
		int p2 = rand() % fit;
		int s = 0;
		bool f1 = false, f2 = false;
		for (int j = 0; j < DIV; j++) {
			s += fitness[j];
			if (p1 < s && !f1) {
				p1 = j; 
				f1 = true;
			}
			if (p2 < s && !f2) {
				p2 = j; 
				f2 = true;
			}
		} if (p1 == p2) p2 = (p2 + 1) % DIV;
		if (r < 33) dna[idx[i]] = _DNA.Recombination(idx[p1], idx[p2]);
		else if (r < 66) dna[idx[i]] = _DNA.CrossOver2(idx[p1], idx[p2]);
		else if(r < 100) dna[idx[i]] = _DNA.CrossOver(idx[p1], idx[p2]);
	}
	for (int i = 0; i < POPULATION; i++) {
		dna[idx[i]].block = 0;
		dna[idx[i]].line = 0;
	}
	for (int i = DIV; i < POPULATION; i++) {
		int r = rand() % 3;
		if(r == -1) dna[idx[i]] = _DNA.MutationLittle(idx[i]);
		else if(r == 0) dna[idx[i]] = _DNA.MutationReverse(idx[i]);
		else if (r == 1) dna[idx[i]] = _DNA.MutationTransform(idx[i]);
		else if (r == 2) dna[idx[i]] = _DNA.MutationNew(idx[i]);
	}
	bestdnaidx = idx[0];
}
