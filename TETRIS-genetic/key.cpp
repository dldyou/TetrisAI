#include <windows.h>
#include <conio.h>
#include "stage.h"
#define ARROW 224
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SPACE 32

static int keyDelayRate;

void ProcessKeyInput(void)
{
	int key;
	bool out = FALSE;
	for (int i = 0; i < 100; i++)
	{
		if (_kbhit() != 0)
		{
			key = _getch();
			if (key == ARROW) {
				key = _getch();
				if (key == LEFT) ShiftLeft();
				else if (key == RIGHT) ShiftRight();
				else if (key == UP) Rotate();
				else if (key == DOWN) BlockDown();
			}
			else if (key == SPACE) Drop();
		}
		if (i % keyDelayRate == 0)
			Sleep(20);
	}
}

void InitKeyDelayRate(int rate)
{
	if (rate < 1)
		return;
	keyDelayRate = rate;
}
