#pragma once

extern const int ROWS ;
extern const int COLM ;

// The game state
extern int blockX; 
extern int blockY; 
extern int board[23][10];  // defined size for the Board
extern int currentShape;  // shapes from 0-6 and starts with I
extern int currentPiece[4][4]; // temp grid to rotate / WORKSPACE
extern bool isGameOver;
extern bool blinkState; // The "On/Off" switch for the flashing effect

// The stats
extern int score; 
extern int highScore;
extern int level ;
extern int gameSpeed ;  // Start speed (50ms)
extern int totalLines ;

//The UI state
extern char activeKey; // track the buttom pressed
extern int keyCooldown ; 
enum AppState { MENU, PLAYING, EXITING};
extern AppState currentState;
extern bool hasPlayedBefore;
extern int startingLevel; 
extern int menuCursor;