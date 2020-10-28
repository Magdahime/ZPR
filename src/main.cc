//File: main.cpp

//Program: SDL_HelloWorld

#include <iostream>
using std::cout;
using std::endl;

//SDL stuff.
#include "SDL.h"

int main (int argc, char* args[]) {
    cout << "Hello World!\n";
    
    // Initialize SDL
    if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
        cout << "Couldn't initialize SDL: %s\n";
        exit (1);
    } else {
        cout << "SDL initialized correctly!\n";
    }
    //Important, otherwise stdout.txt
    //might not be written two.
    atexit (SDL_Quit);
    
    SDL_Quit();

    return 0;
};
