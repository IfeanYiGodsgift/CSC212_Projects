#include "main.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <string>
#include "Animator.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

int playerX = SCREEN_WIDTH / 2;
int playerY = SCREEN_HEIGHT / 2;
int playerSpeed = 10;

const int FPS = 10; // Desired frames per second
const int FRAME_DELAY = 1000 / FPS;

static SDL_Texture* loadTexture(const std::string& path) {
    // The final texture
    SDL_Texture* newTexture = NULL;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else {
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

static bool initSDL()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create window
    gWindow = SDL_CreateWindow("2D Top-Down Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create renderer
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Set renderer color
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Allows for (Initializes) Image Loading abilities using SDL_image library
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

static void closeSDL()
{
    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (!initSDL()) {
        printf("Failed to initialize SDL!\n");
        return -1;
    }

    //Create an Animator
    Animator animator = Animator(gRenderer);

    std::vector<SDL_Texture*> playerIdleTextures;
    playerIdleTextures.push_back(loadTexture("./Assets/PlayerAnimations/player_idle1.png"));
    playerIdleTextures.push_back(loadTexture("./Assets/PlayerAnimations/player_idle2.png"));
    playerIdleTextures.push_back(loadTexture("./Assets/PlayerAnimations/player_idle3.png"));
    playerIdleTextures.push_back(loadTexture("./Assets/PlayerAnimations/player_idle4.png"));
    playerIdleTextures.push_back(loadTexture("./Assets/PlayerAnimations/player_idle5.png"));

    std::vector<SDL_Texture*> playerWalkTextures;
    playerWalkTextures.push_back(loadTexture("./Assets/PlayerAnimations/playerWalk1.png"));
    playerWalkTextures.push_back(loadTexture("./Assets/PlayerAnimations/playerWalk2.png"));
    playerWalkTextures.push_back(loadTexture("./Assets/PlayerAnimations/playerWalk3.png"));
    playerWalkTextures.push_back(loadTexture("./Assets/PlayerAnimations/playerWalk4.png"));
    playerWalkTextures.push_back(loadTexture("./Assets/PlayerAnimations/playerWalk5.png"));
    playerWalkTextures.push_back(loadTexture("./Assets/PlayerAnimations/playerWalk6.png"));
    playerWalkTextures.push_back(loadTexture("./Assets/PlayerAnimations/playerWalk7.png"));

    //Create a new Animation Type
    animator.CreateAnimationType('idle', AnimationState::Idle, playerIdleTextures);
    animator.CreateAnimationType('walk', AnimationState::Moving, playerWalkTextures);

    //SDL_Texture* enemyTexture = loadTexture("enemy.png");
    SDL_Texture* backgroundTexture = loadTexture("./Assets/Floor.png");

    // Main loop flag
    bool quit = false;
    Uint32 frameStart;
    int frameTime;
    SDL_Rect playerRect;
    Direction direction = Direction::Right;

    // Event handler
    SDL_Event e;

    // Main loop
    while (!quit) {
        frameStart = SDL_GetTicks();
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            //User presses a KeyDown
            else if (e.type == SDL_KEYDOWN) {
                animator.movementState = AnimationState::Moving;

                switch (e.key.keysym.sym) {
                case SDLK_UP:
                    playerY -= playerSpeed;
                    direction = Direction::Right;
                    break;
                case SDLK_DOWN:
                    playerY += playerSpeed;
                    direction = Direction::Right;
                    break;
                case SDLK_LEFT:
                    playerX -= playerSpeed;
                    direction = Direction::Left;
                    break;
                case SDLK_RIGHT:
                    playerX += playerSpeed;
                    direction = Direction::Right;
                    break;
                }
            }
            else {
                animator.movementState = AnimationState::Idle;
            }
        }

        // Clear screen
        SDL_RenderClear(gRenderer);

        // Render background
        SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);

        // Render player at its current position
        playerRect = { playerX, playerY, 100, 100 }; // x, y, width, height
        bool playing = animator.PlayAnimation(&playerRect, direction);

        if (playing == false) quit = true;

        // Render enemies

        // Update screen
        SDL_RenderPresent(gRenderer);

        // Calculate frame time and delay if necessary to achieve desired FPS
        frameTime = SDL_GetTicks() - frameStart; // Calculate time taken to process the frame
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime); // Delay if the frame was processed too quickly
        }
    }

    // Destroy textures
    //SDL_DestroyTexture(enemyTexture);
    SDL_DestroyTexture(backgroundTexture);

    animator.CleanAnimations();

    // Close SDL
    closeSDL();

    return 0;
}


