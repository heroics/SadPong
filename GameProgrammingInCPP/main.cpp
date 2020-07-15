#include "Game.h"
#include <SDL2/SDL.h>
#include <exception>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    Game game;
    bool isSuccessful = game.Initialize();
    if (isSuccessful)
    {
        game.RunLoop();
    }
    game.Exit();
    return 0;
}
