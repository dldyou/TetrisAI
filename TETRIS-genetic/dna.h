#pragma once
#ifndef __DNA_H__
#define __DNA_H__

struct DNAW {
	int line = 0;
	int block = 0;
	double weight[8];
};

int GetC(void);
int GetIdx(void);
int GetIdxB(void);
int GetG(void);
int GetL(void);
int GetB(void);
double GetW(int idx);
double GetWT(int parent, int idx);
void AddScore(int parent, int score);
void AddBlock(int parent);
void NextGeneration(void);
int GetP(void);
int GetLT(int parent);
int GetBT(int parent);

class DNA {
public:
	void GenerateNew(void);
	int LoadFile(void);
	void SaveFile(void);
	DNAW CrossOver(int parent1, int parent2);
	DNAW CrossOver2(int parent1, int parent2);
	DNAW Recombination(int parent1, int parent2);
	DNAW MutationTransform(int parent);
	DNAW MutationLittle(int parent);
	DNAW MutationReverse(int parent);
	DNAW MutationNew(int parent);
private:

};
#endif
