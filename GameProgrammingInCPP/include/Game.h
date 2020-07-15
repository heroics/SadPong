#ifndef GAME_H
#define GAME_H
#include "SDL2/SDL.h"
#include "string"
// Vector2 struct to store X and Y coordinates
struct Vector2D
{
    float x;
    float y;
};
class Game
{
public:
    Game();
    // Initialize the game
    bool Initialize();
    // Runs the game loop until the game is over
    void RunLoop();
    // Exit the game gracefully
    void Exit();

private:
    // Helper functions for the game loop
    void HandleInput();
    void Update();
    void GenerateOutput();
    Vector2D paddlePosition;
    int paddleDirection;
    Vector2D ballPosition;
    Vector2D ballSpeed;
    // Window Created using SDL
    SDL_Window *window;
    // Renderer for 2D graphics
    SDL_Renderer *renderer;
    // Indicator if the game is running
    bool isRunning;
    // Calculate Delta Time
    Uint32 ticksCount;
};
#endif // GAME_H
