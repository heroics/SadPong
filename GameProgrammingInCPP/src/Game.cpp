#include "Game.h"
// Global Constants
const int windowWidth = 1024;
const int windowHeight = 768;
const int thickness = 15;
const float paddleHeight = 100.0f;
const float paddleSpeed = 300.0f;
Game::Game()
{
    this->ticksCount = 0;
}
bool Game::Initialize()
{
    char *gameTitle = "J-Pong";
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    const int windowXCoordinate = 100;
    const int windowYCoordinate = 100;
    if (sdlResult != 0)
    {
        SDL_Log("Unable to Initialize SDL: %s", SDL_GetError());
        return false;
    }
    this->window = SDL_CreateWindow(gameTitle, windowXCoordinate, windowYCoordinate, windowWidth, windowHeight, 0);
    if (!this->window)
    {
        SDL_Log("Failed to created window: %s", SDL_GetError());
        return false;
    }
    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!this->renderer)
    {
        SDL_Log("Failed to created renderer: %s", SDL_GetError());
        return false;
    }
    this->paddlePosition.x = 10.0f;
    this->paddlePosition.y = windowHeight / 2.0f;
    this->ballPosition.x = windowWidth / 2.0f;
    this->ballPosition.y = windowHeight / 2.0f;
    this->ballSpeed.x = -200.0f;
    this->ballSpeed.y = 235.0f;
    return true;
}
void Game::Exit()
{
    SDL_DestroyWindow(this->window);
    SDL_DestroyRenderer(this->renderer);
    SDL_Quit();
}
void Game::RunLoop()
{
    while (this->isRunning)
    {
        HandleInput();
        Update();
        GenerateOutput();
    }
}
void Game::HandleInput()
{
    SDL_Event event;
    // While there are still events in the queue
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        // User closes window
        case SDL_QUIT:
            this->isRunning = false;
            break;
        }
    }
    // Get state of keyboard
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    // If ESC is pressed, end loop
    if (keyboardState[SDL_SCANCODE_ESCAPE])
    {
        this->isRunning = false;
    }
    // Control the player paddle
    this->paddleDirection = 0;
    if (keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_UP])
    {
        this->paddleDirection = -1;
    }
    if (keyboardState[SDL_SCANCODE_S] || keyboardState[SDL_SCANCODE_DOWN])
    {
        this->paddleDirection = 1;
    }
}
void Game::Update()
{
    // Wait until 16ms have elapsed since last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), this->ticksCount + 16))
        ;
    // Delta time is the difference in tick from last frame (converted to seconds)
    float deltaTime = (SDL_GetTicks() - this->ticksCount) / 1000.0f;
    // Clamp maximum delta time value
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }
    // Update ticks counts for next frame
    this->ticksCount = SDL_GetTicks();
    // TODO: Update objects in game world as function of delta time.
    // Move player paddle
    if (this->paddleDirection != 0)
    {
        this->paddlePosition.y += this->paddleDirection * paddleSpeed * deltaTime;
        // Keep player paddle in bounds
        if (this->paddlePosition.y < (paddleHeight / 2.0f) + thickness)
        {
            this->paddlePosition.y = (paddleHeight / 2.0f) + thickness;
        }
        else if (this->paddlePosition.y > (windowHeight - thickness - (paddleHeight / 2.0f)))
        {
            this->paddlePosition.y = windowHeight - (paddleHeight / 2.0f) - thickness;
        }
    }
    // Move the ball
    this->ballPosition.x += this->ballSpeed.x * deltaTime;
    this->ballPosition.y += this->ballSpeed.y * deltaTime;
    // Collide with paddle
    float difference = this->paddlePosition.y - this->ballPosition.y;
    difference = (difference > 0.0f) ? difference : -difference;
    if (difference <= (paddleHeight / 2.0f) &&
        this->ballPosition.x <= 25.0f &&
        this->ballPosition.x >= 20.0f &&
        this->ballSpeed.x < 0.0f)
    {
        this->ballSpeed.x *= -1.0f;
    }
    else if (this->ballPosition.x <= 0.0f)
    {
        this->isRunning = false;
    }

    // Bounce ball on top wall
    if (this->ballPosition.y <= thickness)
    {
        this->ballSpeed.y *= -1;
    }
    // Bounce ball on right wall
    else if (this->ballPosition.x >= (windowWidth - thickness))
    {
        this->ballSpeed.x *= -1.0f;
    }
    // Bounce ball on bottom
    else if (this->ballPosition.y >= (windowHeight - thickness))
    {
        this->ballSpeed.y *= -1;
    }
    else if (this->ballPosition.y >= (768 - thickness) && this->ballSpeed.y > 0.0f)
    {
        this->ballSpeed.y *= -1;
    }
}
void Game::GenerateOutput()
{
    // Set Background draw color - Blue
    SDL_SetRenderDrawColor(this->renderer, 0, 129, 175, 255);
    // Clear back buffer
    SDL_RenderClear(this->renderer);
    // Change the render color - Purple
    SDL_SetRenderDrawColor(this->renderer, 204, 2, 180, 255);
    // Draw top wall
    SDL_Rect wall{0, 0, windowWidth, thickness};
    SDL_RenderFillRect(this->renderer, &wall);
    // Draw bottom wall
    wall.y = windowHeight - thickness;
    SDL_RenderFillRect(this->renderer, &wall);
    // Right wall
    wall.x = windowWidth - thickness;
    wall.y = 0;
    wall.w = thickness;
    wall.h = windowWidth;
    SDL_RenderFillRect(this->renderer, &wall);
    // Create ball
    SDL_Rect player{
        static_cast<int>(this->paddlePosition.x),
        static_cast<int>(this->paddlePosition.y - paddleHeight / 2),
        thickness,
        static_cast<int>(paddleHeight)};
    SDL_RenderFillRect(this->renderer, &player);
    SDL_Rect ball{
        static_cast<int>(this->ballPosition.x - thickness / 2),
        static_cast<int>(this->ballPosition.y - thickness / 2),
        thickness,
        thickness};
    SDL_RenderFillRect(this->renderer, &ball);
    SDL_RenderPresent(this->renderer);
}
