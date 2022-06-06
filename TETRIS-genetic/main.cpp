#include <stdlib.h>
#include <time.h>
#include "stage.h"
#include "key.h"
#include "training.h"
#include "dna.h"

int main(void) {
	srand((unsigned int)time(NULL));
	RemoveCursor();
	DrawBoard();
	P();
	ShowScore();
	DNA::best();
	while (1) {
		Reset();
		while (1) {
			RemoveLine();
			InitKeyDelayRate(4 + SetSpeed());
			InitNewBlockPos(4, 0);
			ChooseBlock();
			if (!BlockNext()) break;
			while (1) {
				if (!BlockDown()) {
					AddBlockInfo();
					break;
				}
				//PLAYER
				ProcessKeyInput();
				//AI
				AI();
				//TRAINING
				P();
			}
		}
	}
	return 0;
}