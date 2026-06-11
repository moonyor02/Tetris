#include "renderer.h"
#include "state.h"
#include <iostream>
#include<iomanip>

using namespace std;


bool isFallingBlock(int x, int y){
    if (x >= blockX && x< blockX + 4 && y >=blockY && y < blockY + 4){     // is the 4x4 block in main board
        
        //math for Global -> Local
        int localX = x - blockX;
        int localY = y- blockY;

        if (currentPiece[localY][localX] == 1){     //lookup / check the shape
            return true;  // the block is there
        }
    }
    return false; // empty space ;
    
}

string getLight(char x){                   // for the lighting up the buttons
    return ( x == activeKey ? "\x1B[7m" : "");  // \x1B[7m -> Light On
}

void drawboard(){
    
    cout << "\x1B[H";//ANSI esc code \x1B ESC  [2J clear screen. [H Home

    string margin = "          "; // left margin
    string    gap = "       "; // gap b/w border and hud
    string separator = " | ";

    for (int i=0; i<5; i++) cout << "\r" << endl;  // top 5 buffer line

    //Title
    cout << margin << "      TETRIS (CPP)      " << gap << "   STATISTICS   " << "\r" << endl;
    cout << margin << "========================" << gap << "  ============== " << "\r" << endl;


    for(int y=3; y < ROWS; y++){  // Hides the SPAWN

        cout << margin << "<|";      // left margin + wall |

        for(int x=0; x < COLM; x++){
            if(board[y][x] == 1 || isFallingBlock( x, y)) {
                cout << "[]";
            }else{
                cout << " .";
            }
        }
        cout << "|>";

        cout << gap;


        
        // --- THE HUD (The Right Side Box) ---
        if(y == 3)       cout << " ================ ";
        else if(y == 4)  cout << " |   HIGH-SCORE   | ";
        else if(y == 5)  cout << " |    " << setfill('0') << setw(6) << highScore << "    | ";
        else if(y == 6)  cout << " |--------------| ";
        else if(y == 7)  cout << " |    SCORE     | ";
        else if(y == 8)  cout << " | " << score << (score < 10 ? "            | " : (score < 100 ? "           | " : (score < 1000 ? "          | " : "         | ")));
        else if(y == 9)  cout << " |--------------| ";
        else if(y == 10)  cout << " |    LEVEL     | ";
        else if(y == 11)  cout << " | " << level << (level < 10 ? "            | " : "           | ");
        else if(y == 12)  cout << " |--------------| ";
        else if(y == 13) cout << " |    LINES     | ";
        else if(y == 14) cout << " | " << totalLines << (totalLines < 10 ? "            | " : (totalLines < 100 ? "           | " : "          | "));
        
        // ... your existing stats box ...
        else if(y == 15) cout << " ================ ";
        
        // --- NEW: STACKED CONTROLS ---
        else if(y == 16) cout << "   [CONTROLS]     ";
        else if(y == 17) cout << " A / D : Move     ";
        else if(y == 18) cout << "   W   : Rotate   ";
        else if(y == 19) cout << "   S   : Soft Drop";
        else if(y == 20) cout << " Space : Hard Drop";
        
        // Make sure to update the empty space catcher to 18 spaces!
        else             cout << "                ";
        
        // --------------------------------------------
        // If past row 12, just print empty space so the HUD doesn't repeat
        if(y > 12) cout << "                ";

        cout << "\r" << endl;
    }
    // The floor
    cout << margin << "========================" << gap << " ==================" << "\r" << endl;

    // The control Buttons
    cout << "                " << getLight('w') << "+-----------+" << "\x1B[0m" << '\r' << endl ;  //\x1B[0m -> Light off
    cout << "                " << getLight('w') << "|     W     |" << "\x1B[0m" << '\r' << endl ;
    cout << "                " << getLight('w') << "+-----------+" << "\x1B[0m" << '\r' << endl ;

    cout << " " << getLight('a') << "+-----------+" << "\x1B[0m" << "  " << getLight('s') << "+-----------+" << "\x1B[0m" << "  " << getLight('d') << "+-----------+" << "\x1B[0m" << gap <<getLight(' ') << "+---------------+" << "\x1B[0m" << '\r' << endl;
    cout << " " << getLight('a') << "|     A     |" << "\x1B[0m" << "  " << getLight('s') << "|     S     |" << "\x1B[0m" << "  " << getLight('d') << "|     D     |" << "\x1B[0m" << gap <<getLight(' ') << "|   SPACE BAR   |" << "\x1B[0m" << '\r' << endl;
    cout << " " << getLight('a') << "+-----------+" << "\x1B[0m" << "  " << getLight('s') << "+-----------+" << "\x1B[0m" << "  " << getLight('d') << "+-----------+" << "\x1B[0m" << gap <<getLight(' ') << "+---------------+" << "\x1B[0m" << '\r' << endl;


    if (isGameOver && blinkState) {
        // 1. Pick a starting row (e.g., Row 10)
        int startRow = 10;
    
        // 2. Draw a simple, clean message
        cout << "\033[" << startRow << ";13H"; // Jump to position
        cout << "\033[1;31m";                 // Make it Red + Bold
        
        cout << "********************" << endl;
        cout << "\033[" << startRow + 1 << ";13H*     GAME OVER    *" << endl;
        cout << "\033[" << startRow + 2 << ";13H********************" << endl;
        
        cout << "\033[0m"; // Reset colors so the rest of the game doesn't turn red
    }
   
}


void drawMenu() {
    // Clear the screen and home the cursor
    cout << "\x1B[2J\x1B[H";

    string margin = "          "; // Matches the left margin from your drawboard()

    // Top buffer
    for (int i = 0; i < 5; i++) cout << "\r" << endl;

    // Title Block
    cout << margin << "========================" << "\r" << endl;
    cout << margin << "      TETRIS (CPP)      " << "\r" << endl;
    cout << margin << "========================" << "\r" << endl;
    cout << "\r" << endl;

    // --- OPTION 0: New Game / Retry ---
    cout << margin << "    ";
    if (menuCursor == 0) cout << "\x1B[7m"; // Highlight ON
    
    if (hasPlayedBefore) {
        cout << "[0] Retry Session";
    } else {
        cout << "[0] New Game     ";
    }
    
    if (menuCursor == 0) cout << "\x1B[0m"; // Highlight OFF
    cout << "\r" << endl << "\r" << endl;   // Double space for aesthetics

    // --- OPTION 1: Settings ---
    cout << margin << "    ";
    if (menuCursor == 1) cout << "\x1B[7m"; 
    cout << "[1] Settings     ";
    if (menuCursor == 1) cout << "\x1B[0m"; 
    cout << "\r" << endl << "\r" << endl;

    // --- OPTION 2: Exit ---
    cout << margin << "    ";
    if (menuCursor == 2) cout << "\x1B[7m"; 
    cout << "[2] Exit         ";
    if (menuCursor == 2) cout << "\x1B[0m"; 
    cout << "\r" << endl << "\r" << endl;

    // Footer Controls
    cout << margin << "========================" << "\r" << endl;
    cout << margin << " W/S: Move | SPACE: Select " << "\r" << endl;
}