#include "engine.h"
#include "state.h"
#include "physics.h"
#include "input.h"
#include "../include/pieces.h"
#include <iostream>
#include<cstdlib> //for system


void setShape(int shapeIndex){  // main -> temp grid to rotate / WORKSPACE
    for(int r=0; r < 4; r++){
        for(int c=0; c < 4; c++){
            currentPiece[r][c] = shapes[shapeIndex][r][c];

        }
    }
}


void handleInput(){
    if(!isGameOver){
        if (kbhit()){                  //verify the key hit
            char key = getchar();      // func to get key
            activeKey = key;          // for visual elements
            keyCooldown = 15; // Reset the "light timer" to 15 frames
    
            if (key == 'a'){
                
                if (isValidMove(blockX - 1, blockY , currentPiece)){     // advance move to check the cell is in the main board
                    blockX--;                            //move left  
    
                }
    
                //if  (blockX < 0) blockX = 0;        // hit the left wall
             
            }else if (key == 'd'){
    
                if (isValidMove(blockX + 1, blockY, currentPiece)){  // advance move to check the cell is in the main board
                    blockX++;                                       // move right    
    
                }
                
               // if (blockX > COLM - 4) blockX = COLM - 4;      //hit the right wall at 6 (bcs 6+4 = 10)
    
            }else if (key == 'w') {          // ROTATE!
                int temp[4][4]; 
    
                // 1. The Backup: Copy currentPiece into temp
                memcpy(temp, currentPiece, sizeof(currentPiece));  // memcpy fork the 4x4 grid
    
                 // 2. The Action: Spin it!
                rotateShape();
    
                // 3. The Guard: Did we spin into a wall?
                if (!isValidMove(blockX, blockY, currentPiece)) { 
        
                // 4. The Undo: Copy temp back into currentPiece
                memcpy(currentPiece, temp, sizeof(currentPiece));
                }
            }
            else if (key == 's'){
                if (isValidMove(blockX , blockY+1, currentPiece)){   // SOFT drop 
                    blockY ++;
                }
            }
            else if (key == ' '){
                while(isValidMove(blockX , blockY+1, currentPiece)){   // HARD drop 
                    blockY ++;
                }
            }
        }
        else{
            activeKey = '0';
        } 
    }
    

}

void update(int *timer){

    // --- BLINK LOGIC ---
    if (isGameOver) {
        static int blinkTimer = 0;
        blinkTimer++;
        if (blinkTimer >= 30) { // Flip the switch every 300ms
            blinkState = !blinkState;
            blinkTimer = 0;
        }
        return;
    }
    

    // --- UI COOLDOWN LOGIC (Runs every 10ms) ---
    if (keyCooldown > 0) {
        keyCooldown--;
    } else {
        activeKey = '0'; // Turn off the light only when the timer hits zero
    }

    // --- GRAVITY LOGIC (Runs every 500ms) ---
    (*timer)++;
    if (*timer >= gameSpeed){                 // 50 x 10 = 500 ms
        if (isValidMove(blockX, blockY + 1, currentPiece)){
            blockY++;                       // moving forward if safe
        }
        else{
            lockPiece();                   // coliding with floor / piece

            int cleared = checkLines();

            if(cleared > 0){
                totalLines += cleared;

                // The Division Trick (Handles the "Tetris Skip")
            int newLevel = totalLines / 10; 

            // The Level-Up Trigger
            if (newLevel > level) {
                level = newLevel;
                if (gameSpeed > 5) gameSpeed -= 5; // increases the speed
                }

            }
            

            // SCORE LOGIC (Nintendo Rules)
            switch (cleared) {
                case 1: score += 40 * level; break;
                case 2: score += 100 * level; break;
                case 3: score += 300 * level; break;
                case 4: score += 1200 * level; break; // TETRIS!
            }

            //checkLines(); // checks for full lines

            setShape(rand() % 7);
            blockX = 4;
            blockY = 0;

            if (!isValidMove(blockX, blockY, currentPiece)){
                isGameOver = true;
                std::cout << "GAME OVER" << std::endl;
            }
        }
        *timer = 0;
    }      

}
