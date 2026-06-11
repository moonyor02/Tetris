#include<thread> // for sleep func
#include<chrono> //for time units
#include<ctime> 
#include "renderer.h"
#include "engine.h"
#include "state.h"

using namespace std;

int main(){
    while(currentState != EXITING){
        if (currentState == MENU){
            drawMenu();
        }
        else if (currentState == PLAYING){
            system("clear");

    // Seed the random number generator so every game is different
            srand(static_cast<unsigned int>(time(0)));

            setShape(rand() % 7);

            int timer = 0;

            while(1){

                handleInput();
                update(&timer);
                drawboard();
                this_thread::sleep_for(chrono::milliseconds(10));  
            }

        }
    }
    return 0;
}