#pragma once

#ifndef __STAGE_H__
#define __STAGE_H__

void P(int parent);
void Color(int color);
void RemoveCursor(void);
void gotoxy(int x, int y);
void InitNewBlockPos(int x, int y, int p);

void DrawBoard(int p);
void DeleteNextBlock(int idx, int p);
void ShowNextBlock(int idx, int p);

void ChooseBlock(int p);
bool BlockNext(void);
bool DetectCollision(int x, int y);

void AddBlockInfo(void);
void SubBlockInfo(int x, int y);
void ShowBlock(int p);
void RemoveBlock(int p);

void RemoveLine(int parent);
void LineDown(int line, int p);
void ShowAllBlock(int p);

bool BlockDown(int p);
void ShiftLeft(int p);
void ShiftRight(int p);
void Rotate(int p);
void Drop(int p);

void ShowScore(int p);
int SetSpeed(void);
void Reset(int p);

int GetCurrentBlock(void);
double GetScore(int parent);
void AI(int p);
#endif
