#include "Renderer3D.h"
#include <iostream>
#include <vector>
#include <SDL.h>
#include <cmath>

// Define points for the square-based pyramid
std::vector<Point3D> points{
    Point3D{ -1, -1, -1 }, Point3D{ -1, -1, 1 },
    Point3D{ 1,  -1, -1 }, Point3D{ 1,  -1, 1 },
    Point3D{ 0,  1, 0 } // Top point of the pyramid
};

// Define edges for the square-based pyramid
std::vector<Edge> edges{
    Edge{ 0, 1 }, Edge{ 0, 2 }, Edge{ 0, 3 }, Edge{ 1, 2 },
    Edge{ 1, 3 }, Edge{ 2, 3 }, Edge{ 4, 0 }, Edge{ 4, 1 },
    Edge{ 4, 2 }, Edge{ 4, 3 }
};
int main(int argc, char* argv[])
{
    // Creating a window and a renderer
    SDL_Window* window;
    SDL_Renderer* renderer;

    // Create a centered window with 960 width and 540 height
    window = SDL_CreateWindow("Pyramid Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;

    // Create an instance of the Renderer3D class
    Renderer3D render3D(window, renderer, points, edges);

    while (running)
    {
        // If the quit button is pressed, the loop breaks
        if (SDL_QuitRequested()) { running = false; break; }

        // Calls the function created in the Renderer3D class
        render3D.render();
    }

    // Clean up resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
