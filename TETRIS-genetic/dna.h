#pragma once
#ifndef __DNA_H__
#define __DNA_H__

struct DNAW {
	int line = 0;
	double weight[8];
};

double GetW(int idx);

class DNA {
public:
	void GenerateNew(void);
	DNAW CrossOver(int parent1, int parent2);
	DNAW Recombination(int parent1, int parent2);
	DNAW MutationTransform(int parent);
	DNAW MutationExchange(int parent);
private:

};
#endif