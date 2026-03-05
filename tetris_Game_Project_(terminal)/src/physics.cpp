#include "physics.h"
#include "state.h"
#include<cstring> // for memcpy


void rotateShape(){
    int temp[4][4]; // temporary buffer / variable

    //Maths for rotation 
    for(int r=0; r < 4; r++){
        for(int c=0; c < 4; c++){
            temp[c][3-r] = currentPiece[r][c];  // top row to right colm

        }
    }
    // save it to WORKSPACE
    memcpy(currentPiece, temp, sizeof(currentPiece));

    /*for(int r=0; r < 4; r++){      // this is for basics to understand 
        for(int c=0; c < 4; c++){
            currentPiece[r][c] = temp[r][c];

        }
    }*/
}

bool isValidMove(int newX, int newY, int newShape[4][4]){
    for(int r=0; r<4; r++){
        for(int c=0; c<4; c++){

            if (newShape[r][c] == 1){    //check solid part of 4x4 grid
                int destX = newX + c;  // x cord of main board
                int destY = newY + r; // y cord of main board

                if (destX <0 || destX >= COLM) return false;   //lest/right wall check

                if (destY >= ROWS) return false;    //floor check

                // remind board[y][x]
                if (destY >= 0 && board[destY][destX] == 1) return false;  // is there a block below me
            }
        }
    }
    return true;
}

void lockPiece(){ // temp piece -> main / permanent board
    for (int r=0; r<4; r++){
        for(int c=0; c<4; c++){
            if (currentPiece[r][c] == 1){
                int finalY = blockY + r;    // x cord of main board
                int finalX = blockX + c;    // y cord of main board
                board[finalY][finalX] =1;   // fixing acc to pattern
            }
        }
    }
}

int checkLines(){      // Deletes -> the filled lines

    int linesCleared = 0;

    for(int y = ROWS - 1; y >= 0; y--){ // from bottom -> top
        bool full = true;
        for (int x=0; x<COLM; x++){     //checking for gap
            if (board[y][x] == 0){
                full = false;   //found a gap
                break; 
            }
        }
        if (full){
            linesCleared++;

            for (int k = y; k>0 ; k--){
                for(int x = 0; x < COLM; x++){
                    board[k][x] = board[k-1][x]; // Copy the row above
                }
            }

            // Clear the very top row (new empty air)
            for(int x = 0; x < COLM; x++){
                board[0][x] = 0;
            }
            y++; // row fell down so new y
        }
    }
    return linesCleared;
}