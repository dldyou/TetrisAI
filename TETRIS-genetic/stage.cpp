#include <stdio.h>
#include <windows.h>
#include <functional>
#include <random>
#include <time.h>
#include "block.h"
#include "dna.h"

using namespace std;
using namespace std::tr1;
//gameboard
#define GW 12
#define GH 21
//color
#define BLACK 0 
#define BLUE 1 
#define GREEN 2 
#define CYAN 3 
#define RED 4 
#define MAGENTA 5 
#define BROWN 6 
#define LIGHTGRAY 7 
#define DARKGRAY 8 
#define LIGHTBLUE 9 
#define LIGHTGREEN 10 
#define LIGHTCYAN 11 
#define LIGHTRED 12 
#define LIGHTMAGENTA 13 
#define YELLOW 14 
#define WHITE 15 

int board[GH][GW] = { 0 };
int curblock;
int nextblock;
bool firstblockF = false;
int curX, curY;
int rotateState;
int score = 0;
int line = 0;
int tempp = 0;

mt19937 engine((unsigned int)time(NULL));
uniform_int<> distribution(0, 28 * 100);
auto generator = bind(distribution, engine);

void ShowScore(int p);
void RemoveLine(int parent);
int GetCurrentBlock(void);

void Color(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void RemoveCursor(void)
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void InitNewBlockPos(int x, int y, int p)
{
	if (x < 0 || y < 0)
		return;

	curX = x;
	curY = y;
	rotateState = 0;
	if(p == GetIdxB())
		gotoxy(curX * 2, curY);
}

void P(int parent) {
	/*if (parent < 25) gotoxy(45, parent);
	else gotoxy(75, parent - 25);
	printf("[%03d] %010d", parent, GetLT(parent));*/
	if (parent == GetIdxB()) {
		/*for (int i = 0; i < GH; i++) {
			for (int j = 0; j < GW; j++) {
				gotoxy(35 + j, i); printf("%d", board[i][j]);
			}
		}
		*/
		/*
		gotoxy(35, GH); printf("%03d gene (score: %06d)", parent, GetLT(parent));
		for (int i = 0; i < 8; i++) {
			gotoxy(35, GH + i + 1); printf("%lf", GetWT(parent, i));
		}*/
		gotoxy(25, 10); printf("generation: %05d", GetG());
		gotoxy(25, 11); printf("best w");
		for (int i = 0; i < 8; i++) {
			gotoxy(25, 12 + i); printf("%lf", GetW(i));
		}
		gotoxy(25, 20); printf("score: %010d", GetL());
		gotoxy(25, 21); printf("block: %010d", GetB());
	}
}

void DrawBoard(int p) {
	if (p == GetIdxB()) {
		for (int i = 0; i < GH; i++) {
			gotoxy(0, i);
			if (i == GH - 1) printf("└");
			else printf("│");
			gotoxy((GW - 1) * 2, i);
			if (i == GH - 1) printf("┘");
			else printf("│");
		}
		for (int i = 1; i < GW - 1; i++) {
			gotoxy(i * 2, GH - 1);
			printf("─");
		}
	}
	for (int i = 0; i < GH; i++) {
		board[i][0] = 1;
		board[i][GW - 1] = 1;
	}
	for (int i = 0; i < GW; i++) {
		board[GH - 1][i] = 1;
	}
}


void DeleteNextBlock(int idx, int p) {
	if (p == GetIdxB()) {
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				gotoxy(8 + x * 2, 23 + y);

				if (block[idx][y][x] == 1)
					printf("  ");
			}
		}
	}
}

void ShowNextBlock(int idx, int p) {
	if (p == GetIdxB()) {
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				gotoxy(8 + x * 2, 23 + y);

				if (block[idx][y][x] == 1)
					printf("■");
			}
		}
	}
}

void ChooseBlock(int p)
{
	if (firstblockF == false)
	{
		curblock = (generator() % 7) * 4;
		firstblockF = true;
	}
	else
		curblock = nextblock;
	nextblock = (generator() % 7) * 4;
	DeleteNextBlock(curblock, p);
	ShowNextBlock(nextblock, p);
}

bool DetectCollision(int x, int y) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[GetCurrentBlock()][i][j] == 1 && board[y + i][x + j] == 1)
				return true;
		}
	}
	return false;
}

void AddBlockInfo(void) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[GetCurrentBlock()][i][j] == 1)
				board[curY + i][curX + j] = 1;
		}
	}
}
void SubBlockInfo(int x, int y) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[GetCurrentBlock()][i][j] == 1)
				board[curY + i][curX + j] = 0;
		}
	}
	curX = x, curY = y;
}

void RemoveBlock(int p) {
	if (p == GetIdxB()) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (block[GetCurrentBlock()][i][j] == 1) {
					gotoxy((curX + j) * 2, curY + i);
					printf("  ");
				}
			}
		}
	}
}

void ShowBlock(int p) {
	if (p == GetIdxB()) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (block[GetCurrentBlock()][i][j] == 1) {
					gotoxy((curX + j) * 2, curY + i);
					printf("■");
				}
			}
		}
	}
}

void ShowAllBlock(int p) {
	if (p == GetIdxB()) {
		for (int i = 0; i < GH - 1; i++) {
			for (int j = 1; j < GW - 1; j++) {
				gotoxy(j * 2, i);
				if (board[i][j] == 1) printf("■");
				else printf("  ");
			}
		}
	}
}

bool BlockDown(int p) {
	if (DetectCollision(curX, curY + 1)) return false;
	else {
		RemoveBlock(p);
		curY++;
		ShowBlock(p);
		return true;
	}
}

void ShiftLeft(int p) {
	if (!DetectCollision(curX - 1, curY)) {
		RemoveBlock(p);
		curX--;
		ShowBlock(p);
	}
}

void ShiftRight(int p) {
	if (!DetectCollision(curX + 1, curY)) {
		RemoveBlock(p);
		curX++;
		ShowBlock(p);
	}
}

void Rotate(int p) {
	RemoveBlock(p);
	rotateState = (rotateState + 1) % 4;
	if (DetectCollision(curX, curY)) rotateState = (rotateState + 3) % 4;
	ShowBlock(p);
}

void Drop(int p) {
	while (BlockDown(p));
	RemoveLine(p);
}

void LineDown(int line, int p) {
	for (int y = line; y >= 0; y--) {
		for (int x = 1; x < GW - 1; x++) {
			if (y != 0) board[y][x] = board[y - 1][x];
			else board[y][x] = 0;
		}
	}
	if(p == GetIdxB()) ShowAllBlock(p);
}

void RemoveLine(int p) {
	int l = 0;
	int scorep = 0;
	for (int i = 0; i < 4; i++) {
		int x, y;
		for (y = GH - 2; y >= 0; y--) {
			bool flag = true;
			for (x = 1; x < GW - 1; x++)
				if (board[y][x] != 1) flag = false;
			if (flag) {
				for (x = 1; x < GW - 1; x++)
					board[y][x] = 0;
				LineDown(y, p);
				l++;
			}
		}
	}
	if (l == 1) scorep = 1;
	else if (l == 2) scorep = 2;
	else if (l == 3) scorep = 3;
	else if (l == 4) scorep = 4;
	AddScore(p, scorep);
	score += scorep;
	ShowScore(p);
}
bool BlockNext(void) {
	if (!DetectCollision(curX, curY)) return true;
	else return false;
}

void Reset(int p) {
	score = 0, firstblockF = false;
	for (int i = 0; i < GH - 1; i++)
		for (int j = 1; j < GW - 1; j++)
			board[i][j] = 0;
	if(p == GetIdxB()) ShowAllBlock(p);
}

void ShowScore(int p) {
	if (p == GetIdxB()) {
		gotoxy(10, 21); printf("             ");
		gotoxy(10, 21); printf("score: %06d", score);
	}
}

int SetSpeed(void) {
	int pStep = 1;
	int mxSpeed = 10;
	return score / pStep > mxSpeed ? mxSpeed : score / pStep;
}

int GetCurrentBlock(void) {
	return curblock + rotateState;
}

double GetScore(int parent) {
	double score = 0;
	// height, hole, barrier, bump
	int heightSum = 0;
	int hole = 0;
	int barrier = 0;
	int bheight = -1, bump = 0;
	for (int j = 0; j < GW - 2; j++) {
		int height = 0;
		int fheight = 0, lheight = 0;
		bool flag = true;
		bool flag2 = true;
		int holeCnt = 0;
		for (int i = GH - 2; i >= 0; i--) {
			if (board[i][j] == 1) {
				if (flag) {
					hole += holeCnt;
					holeCnt = 0;
				}
				height = GH - 1 - i;
				lheight = GH - 1 - i;
				flag = false;
			}
			else {
				holeCnt++;
				flag = true;
				if (flag2) {
					fheight = GH - 1 - i;
					lheight = fheight;
					flag2 = false;
				}
			}
		}
		if (bheight != -1) bump += abs(height - bheight);
		heightSum += height;
		bheight = height;
		barrier += lheight - fheight;
	}
	heightSum /= (GW - 2);
	// line
	int line = 0;
	for (int i = curY; i < curY + 4; i++) {
		if (i > GH - 1) continue;
		bool flag = true;
		for (int j = 1; j < GW - 2; j++)
			if (board[i][j] == 0) flag = false;
		if (flag) line++;
	}
	// bottom, wall, blockcnt
	int bottom = 0, wall = 0, blockcnt = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int X = curX + j;
			int Y = curY + i;
			if (X > GW - 1 || Y > GH - 1) continue;
			if (block[GetCurrentBlock()][i][j] == 1) {
				if (X - 1 == 0 || X + 1 == GW - 1 || Y + 1 == GH - 1)
					if (Y + 1 == GH - 1) bottom++;
					else wall++;
			}
			else {
				if (block[GetCurrentBlock()][i][j - 1] != 1)
					if (board[Y][X - 1] == 1) blockcnt++;
				if (block[GetCurrentBlock()][i][j + 1] != 1)
					if (board[Y][X + 1] == 1) blockcnt++;
				if (block[GetCurrentBlock()][i + 1][j] != 1)
					if (board[Y + 1][X] == 1) blockcnt++;
			}
		}
	}
	score += (double)heightSum * GetWT(parent, 0);
	score += (double)hole * GetWT(parent, 1);
	score += (double)barrier * GetWT(parent, 2);
	score += (double)line * GetWT(parent, 3);
	score += (double)bump * GetWT(parent, 4);
	score += (double)bottom * GetWT(parent, 5);
	score += (double)blockcnt * GetWT(parent, 6);
	score += (double)wall * GetWT(parent, 7);
	return score;
}

void AI(int p) {
	RemoveBlock(p);
	double bestscore = -123456789, curscore = -123456789;
	int bx = -1234, br = -1234;
	for (int i = 0; i < 4; i++) {
		rotateState = i;
		if (DetectCollision(curX, curY)) break;
		while (!DetectCollision(curX - 1, curY)) curX--;
		while (!DetectCollision(curX, curY)) {
			while (!DetectCollision(curX, curY + 1)) curY++;
			AddBlockInfo();
			curscore = GetScore(p);
			if (curscore > bestscore) {
				bestscore = curscore;
				bx = curX, br = rotateState;
			}
			SubBlockInfo(curX, 0);
			curX++;
		}
		curX = 4; curY = 0; rotateState = 0;
	}
	if (br != -1234) {
		int t = 0;
		while (rotateState != br) {
			Rotate(p);
			t++;
			if (t > 4) break;
		}
	}
	if (bx != -1234) {
		int t = 0;
		while (curX != bx) {
			if (curX > bx) ShiftLeft(p);
			else if (curX < bx) ShiftRight(p);
			t++;
			if (t > 15) break;
		}
	}
	Drop(p);
	AddBlock(p);
	P(p);
}
