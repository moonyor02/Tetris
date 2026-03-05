#pragma once

//Game Physics for blocks
void rotateShape();
bool isValidMove(int newX, int newY, int shape[4][4]);
void lockPiece();
int checkLines();