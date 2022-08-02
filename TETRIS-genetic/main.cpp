#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <thread>
#include <conio.h>
#include "stage.h"
#include "key.h"
#include "training.h"
#include "dna.h"
using namespace std;
class T {
public:
	static void Thread(int parent) {
		Reset(parent);
		ShowScore(parent);
		while (1) {
			RemoveLine(parent);
			InitNewBlockPos(4, 0, parent);
			ChooseBlock(parent);
			if (!BlockNext()) break;
			while (1) {
				if (!BlockDown(parent)) {
					AddBlockInfo();
					break;
				}
				//PLAYER
				//AI
				AI(parent);
				//TRAINING
			}
		}
	}
};
int main(void) {
	srand((unsigned int)time(NULL));
	int p = GetP();
	int count = GetC();
	RemoveCursor();
	DrawBoard(0);
	ShowScore(0);
	thread t[50];
	DNA _DNA = DNA();
	T _T = T();
	char c = _getch();
	if (c == '0') {
		_DNA.GenerateNew();
		_DNA.SaveFile();
	}
	if(!_DNA.LoadFile()) _DNA.GenerateNew();
	_DNA.SaveFile();
	InitKeyDelayRate(4);
	while (1) {
		while (1) {
			for (int cnt = 0; cnt < count; cnt++) {
				for (int i = 0; i < p; i++) {
					t[i] = thread(_T.Thread, i);
					t[i].join();
				}
			}
			NextGeneration();
			_DNA.SaveFile();
		}
	}
	return 0;
}
