#include <stdio.h>
#include <windows.h>
#include "block.h"
#include "dna.h"
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

static int board[GH][GW] = { 0 };
static int curblock;
static int nextblock;
static bool firstblockF = false;
static int curX, curY;
static int rotateState;
static int score = 0;
static int line = 0;

void ShowScore(void);
void RemoveLine(void);
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

void InitNewBlockPos(int x, int y)
{
	if (x < 0 || y < 0)
		return;

	curX = x;
	curY = y;
	rotateState = 0;
	gotoxy(curX * 2, curY);
}

void P(void) {
	for (int i = 0; i < GH; i++) {
		for (int j = 0; j < GW; j++) {
			gotoxy(35 + j, i); printf("%d", board[i][j]);
		}
	}
}

void DrawBoard(void) {
	for (int i = 0; i < GH; i++) {
		gotoxy(0, i);
		if (i == GH - 1) printf("¦¦");
		else printf("¦¢");
		gotoxy((GW - 1) * 2, i);
		if (i == GH - 1) printf("¦¥");
		else printf("¦¢");
	}
	for (int i = 1; i < GW - 1; i++) {
		gotoxy(i * 2, GH - 1);
		printf("¦¡");
	}
	for (int i = 0; i < GH; i++) {
		board[i][0] = 1;
		board[i][GW - 1] = 1;
	}
	for (int i = 0; i < GW; i++) {
		board[GH - 1][i] = 1;
	}
}


void DeleteNextBlock(int idx) {
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

void ShowNextBlock(int idx) {
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			gotoxy(8 + x * 2, 23 + y);

			if (block[idx][y][x] == 1)
				printf("¡á");
		}
	}
}

void ChooseBlock(void)
{
	if (firstblockF == false)
	{
		curblock = (rand() % 7) * 4;
		firstblockF = true;
	}
	else
		curblock = nextblock;
	nextblock = (rand() % 7) * 4;
	DeleteNextBlock(curblock);
	ShowNextBlock(nextblock);
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
void SubBlockInfo(void) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[GetCurrentBlock()][i][j] == 1)
				board[curY + i][curX + j] = 0;
		}
	}
}

void RemoveBlock(void) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[GetCurrentBlock()][i][j] == 1) {
				gotoxy((curX + j) * 2, curY + i);
				printf("  ");
			}
		}
	}
}

void ShowBlock(void) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[GetCurrentBlock()][i][j] == 1) {
				gotoxy((curX + j) * 2, curY + i);
				printf("¡á");
			}
		}
	}
}

void ShowAllBlock(void) {
	for (int i = 0; i < GH - 1; i++) {
		for (int j = 1; j < GW - 1; j++) {
			gotoxy(j * 2, i);
			if (board[i][j] == 1) printf("¡á");
			else printf("  ");
		}
	}
}

bool BlockDown(void) {
	if (DetectCollision(curX, curY + 1)) return false;
	else {
		RemoveBlock();
		curY++;
		ShowBlock();
		return true;
	}
}

void ShiftLeft(void) {
	if (!DetectCollision(curX - 1, curY)) {
		RemoveBlock();
		curX--;
		ShowBlock();
	}
}

void ShiftRight(void) {
	if (!DetectCollision(curX + 1, curY)) {
		RemoveBlock();
		curX++;
		ShowBlock();
	}
}

void Rotate(void) {
	RemoveBlock();
	rotateState = (rotateState + 1) % 4;
	if (DetectCollision(curX, curY)) rotateState = (rotateState + 3) % 4;
	ShowBlock();
}

void Drop(void) {
	while (BlockDown());
	RemoveLine();
}

void LineDown(int line) {
	for (int y = line; y >= 0; y--) {
		for (int x = 1; x < GW - 1; x++) {
			if (y != 0) board[y][x] = board[y - 1][x];
			else board[y][x] = 0;
		}
	}
	ShowAllBlock();
}

void RemoveLine(void) {
	for (int i = 0; i < 4; i++) {
		int x, y;
		for (y = GH - 2; y >= 0; y--) {
			bool flag = true;
			for (x = 1; x < GW - 1; x++)
				if (board[y][x] != 1) flag = false;
			if (flag) {
				for (x = 1; x < GW - 1; x++)
					board[y][x] = 0;
				LineDown(y);
				score++;
				ShowScore();
			}
		}
	}
}
bool BlockNext(void) {
	if (!DetectCollision(curX, curY)) return true;
	else return false;
}

void Reset(void) {
	for (int i = 0; i < GH - 1; i++)
		for (int j = 1; j < GW - 1; j++)
			board[i][j] = 0;
	ShowAllBlock();
}

void ShowScore(void) {
	gotoxy(10, 21); printf("                   ");
	gotoxy(10, 21); printf("score: %06d", score);
}

int SetSpeed(void) {
	int pStep = 1;
	int mxSpeed = 10;
	return score / pStep > mxSpeed ? mxSpeed : score / pStep;
}

int GetCurrentBlock(void) {
	return curblock + rotateState;
}

double GetScore(int x, int y, int r) {
	double score = 0;
	// height, hole, barrier, bump
	double heightAverage = 0;
	int hole = 0;
	int barrier = 0;
	int bheight = -1, bump = 0;
	for (int j = 0; j < GW - 2; j++) {
		int height = 0;
		int fheight = 0, lheight = 0;
		bool flag = true;
		bool flag2 = true;
		for (int i = GH - 2; i >= 0; i--) {
			int holeCnt = 0;
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
		heightAverage += height;
		bheight = height;
		barrier += lheight - fheight;
	}
	heightAverage /= (GW - 2);
	// line
	int line = 0;
	for (int i = y; i < y + 4; i++) {
		if (i > GH - 1) continue;
		bool flag = true;
		for (int j = 0; j < GW - 2; j++) 
			if (board[i][j] == 0) flag = false;
		if (flag) line++;
	}
	// bottom, wall, blockcnt
	int bottom = 0, int wall = 0, int blockcnt = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int X = x + j;
			int Y = y + i;
			if (X > GW - 1 || Y > GH - 1) continue;
			if (block[curblock + r][i][j] == 1) {
				if (X - 1 == 0 || X + 1 == GW - 1 || Y + 1 == GH - 1)
					if (Y + 1 == GH - 1) bottom++;
					else wall++;
			}
			else {
				if (block[curblock + r][i][j - 1] != 1) 
					if (board[Y][X - 1] == 1) blockcnt++;
				if (block[curblock + r][i][j + 1] != 1)
					if (board[Y][X + 1] == 1) blockcnt++;
				if (block[curblock + r][i + 1][j] != 1)
					if (board[Y + 1][X] == 1) blockcnt++;
			}
		}
	}
	score += heightAverage * GetW(0);
	score += (double)hole * GetW(1);
	score += (double)barrier * GetW(2); 
	score += (double)line * GetW(3);
	score += (double)bump * GetW(4);
	score += (double)bottom * GetW(5);
	score += (double)blockcnt * GetW(6);
	score += (double)wall * GetW(7);

	return score;
}

void Training(void) {
	int wmove = 0, hmove = 0;
	double bestscore = -123456789;
	for (int i = 0; i < 4; i++) {
		while (!DetectCollision(curX - 1 + wmove, curY + hmove)) wmove--;
		while (!DetectCollision(curX - 1 + wmove, curY + hmove)) hmove++;
		AddBlockInfo();

		while (!DetectCollision(curX + 1 + wmove, curY + hmove)) {
			if (!DetectCollision(curX - 1 + wmove, curY + hmove)) {
				wmove++;
				while (!DetectCollision(curX - 1 + wmove, curY + hmove)) hmove++;
				AddBlockInfo();
			}
		}
	}
}