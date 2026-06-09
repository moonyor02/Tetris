#include "state.h"

const int ROWS = 23;
const int COLM = 10;

int blockX = 4;
int blockY = 0;
int board[ROWS][COLM] = {0};
int currentShape = 0; // shapes from 0-6 and starts with I
int currentPiece[4][4]; // temp grid to rotate / WORKSPACE
bool isGameOver = false;
bool blinkState = true;

int score = 0;
int highScore = 0;
int level = 1;
int gameSpeed = 50;  // Start speed (50ms)
int totalLines = 0;

char activeKey = '0'; // track the buttom pressed
int keyCooldown = 0 ; 
