#pragma once

#ifndef __STAGE_H__
#define __STAGE_H__

void P(void);
void Color(int color);
void RemoveCursor(void);
void gotoxy(int x, int y); 
void InitNewBlockPos(int x, int y);

void DrawBoard(void);
void DeleteNextBlock(int idx);
void ShowNextBlock(int idx);

void ChooseBlock(void);
bool BlockNext(void);
bool DetectCollision(int x, int y);

void AddBlockInfo(void);
void SubBlockInfo(int x, int y);
void ShowBlock(void);
void RemoveBlock(void);

void RemoveLine(void);
void LineDown(int line);
void ShowAllBlock(void);

bool BlockDown(void);
void ShiftLeft(void);
void ShiftRight(void);
void Rotate(void);
void Drop(void);

void ShowScore(void);
int SetSpeed(void);
void Reset(void);

int GetCurrentBlock(void);
double GetScore(void);
void AI(void);
#endif