#include<iostream>
#include<thread> // for sleep func
#include<chrono> //for time units
#include<cstdlib> //for system
#include<ctime> 
#include<cstring>
using namespace std;

//-------------------------------------

// --- MAC/LINUX SPECIFIC HEADERS ---
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// This function checks if a key is pressed on Mac
int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}
// ------------------------------------


const int ROWS = 23;
const int COLM = 10;

int blockX = 4;
int blockY = 0;

int currentShape = 0; // shapes from 0-6 and starts with I
int board[ROWS][COLM] = {0};
int currentPiece[4][4]; // temp grid to rotate / WORKSPACE

int score = 0;
int level = 1;
int gameSpeed = 50;  // Start speed (50ms)
int totalLines = 0;

char activeKey = '0';

// The 7 Tetromino Shapes
// 0=Empty, 1=Solid
int shapes[7][4][4] = {
    { // 0: I (Line)
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
    },
    { // 1: J (Left Hook)
        {0,0,1,0},
        {0,0,1,0},
        {0,1,1,0},
        {0,0,0,0}
    },
    { // 2: L (Right Hook)
        {0,1,0,0},
        {0,1,0,0},
        {0,1,1,0},
        {0,0,0,0}
    },
    { // 3: O (Square)
        {0,0,0,0},
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0}
    },
    { // 4: S (Right Snake)
        {0,0,0,0},
        {0,0,1,1},
        {0,1,1,0},
        {0,0,0,0}
    },
    { // 5: T (Pyramid)
        {0,0,0,0},
        {0,1,1,1},
        {0,0,1,0},
        {0,0,0,0}
    },
    { // 6: Z (Left Snake)
        {0,0,0,0},
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0}
    }
};


void setShape(int shapeIndex){  // main -> temp grid to rotate / WORKSPACE
    for(int r=0; r < 4; r++){
        for(int c=0; c < 4; c++){
            currentPiece[r][c] = shapes[shapeIndex][r][c];

        }
    }
}

void rotateShape(){
    int temp[4][4]; // temporary buffer / variable

    //Maths for rotation 
    for(int r=0; r < 4; r++){
        for(int c=0; c < 4; c++){
            temp[c][3-r] = currentPiece[r][c];  // top row to right colm

        }
    }
    // save it to WORKSPACE
    for(int r=0; r < 4; r++){
        for(int c=0; c < 4; c++){
            currentPiece[r][c] = temp[r][c];

        }
    }
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
        else if(y == 4)  cout << " |    SCORE     | ";
        else if(y == 5)  cout << " | " << score << (score < 10 ? "            | " : (score < 100 ? "           | " : (score < 1000 ? "          | " : "         | ")));
        else if(y == 6)  cout << " |--------------| ";
        else if(y == 7)  cout << " |    LEVEL     | ";
        else if(y == 8)  cout << " | " << level << (level < 10 ? "            | " : "           | ");
        else if(y == 9)  cout << " |--------------| ";
        else if(y == 10) cout << " |    LINES     | ";
        else if(y == 11) cout << " | " << totalLines << (totalLines < 10 ? "            | " : (totalLines < 100 ? "           | " : "          | "));
        
        // ... your existing stats box ...
        else if(y == 12) cout << " ================ ";
        
        // --- NEW: STACKED CONTROLS ---
        else if(y == 14) cout << "   [CONTROLS]     ";
        else if(y == 15) cout << " A / D : Move     ";
        else if(y == 16) cout << "   W   : Rotate   ";
        else if(y == 17) cout << "   S   : Soft Drop";
        else if(y == 18) cout << " Space : Hard Drop";
        
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



int main(){
    system("clear");

    setShape(rand() % 7);

    bool gameover = false;
    int timer = 0;

    while( !gameover ){

        if (kbhit()){                  //verify the key hit
            char key = getchar();      // func to get key
            activeKey = key;          // for visual elements

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
        timer ++;

        
        /*blockY++;                   // gravity update
        if(blockY >= ROWS){        //collision detection
            blockY = 0;           //reset to top
        }*/
        // board[blockY][blockX]=1;  // OLD: Stamped block on board (Causes bugs)


        if (timer >= gameSpeed){                 // 50 x 10 = 500 ms
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
                    gameover = true;
                    cout << "GAME OVER" << endl;
                }
            }
            timer = 0;
        }
        
        

        drawboard();
       

        // board[blockY][blockX]=0; // OLD: Erased trail (Not needed for projection)

        this_thread::sleep_for(chrono::milliseconds(10));
    }
    return 0;
}
