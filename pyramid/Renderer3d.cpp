#include "Renderer3D.h"
#include <SDL.h>
#include <cmath>

Renderer3D::Renderer3D(SDL_Window* _window, SDL_Renderer* renderer, std::vector<Point3D>& _points, std::vector<Edge>& _edges)
{
    SDL_GetWindowSize(_window, &WindowSizeX, &WindowSizeY);
    SDL_render = renderer;
    points = _points;
    edges = _edges;
}

/*void Renderer3D::render()
{
    auto time1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration(0);

    SDL_SetRenderDrawColor(SDL_render, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(SDL_render);
    SDL_SetRenderDrawColor(SDL_render, 255, 192, 203, SDL_ALPHA_OPAQUE);

    rotation += 1 * DeltaTime;

    // Pulsating effect using sine function
    scaleFactor = 1.0 + 0.5 * sin(rotation); // Adjust the pulsation speed and intensity as needed

    for (auto& edge : edges) {
        Point3D rotatedPointStart = rotateOnX(points[edge.start]);
        Point3D rotatedPointEnd = rotateOnX(points[edge.end]);

        // Apply scaling
        rotatedPointStart.x *= scaleFactor;
        rotatedPointStart.y *= scaleFactor;
        rotatedPointStart.z *= scaleFactor;

        rotatedPointEnd.x *= scaleFactor;
        rotatedPointEnd.y *= scaleFactor;
        rotatedPointEnd.z *= scaleFactor;

        Point2D start = projection(rotatedPointStart);
        Point2D end = projection(rotatedPointEnd);

        SDL_RenderDrawLine(SDL_render, start.x, start.y, end.x, end.y);
    }

    SDL_RenderPresent(SDL_render);

    auto time2 = std::chrono::high_resolution_clock::now();
    duration = time2 - time1;
    DeltaTime = duration.count();
    time1 = time2;
}
*/
//Above is the function used to rotate and pulsate at the same time


void Renderer3D::render()
{
    auto time1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration(0);

    SDL_SetRenderDrawColor(SDL_render, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(SDL_render);
    SDL_SetRenderDrawColor(SDL_render, 255, 192, 203, SDL_ALPHA_OPAQUE);

    // Pulsating effect using sine function
    scaleFactor = 1.0 + 0.5 * sin(SDL_GetTicks() / 1000.0); // this controls pulsation speed, reducing the divisible reduces pulsating time

    for (auto& edge : edges) {
        Point3D scaledPointStart = points[edge.start];
        Point3D scaledPointEnd = points[edge.end];

        // Apply scaling
        scaledPointStart.x *= scaleFactor;
        scaledPointStart.y *= scaleFactor;
        scaledPointStart.z *= scaleFactor;

        scaledPointEnd.x *= scaleFactor;
        scaledPointEnd.y *= scaleFactor;
        scaledPointEnd.z *= scaleFactor;

        Point2D start = projection(scaledPointStart);
        Point2D end = projection(scaledPointEnd);

        SDL_RenderDrawLine(SDL_render, start.x, start.y, end.x, end.y);
    }

    SDL_RenderPresent(SDL_render);

    auto time2 = std::chrono::high_resolution_clock::now();
    duration = time2 - time1;
    DeltaTime = duration.count();
    time1 = time2;
}

Point2D Renderer3D::projection(Point3D point)
{
    return Point2D{ WindowSizeX / 2 + (point.x * FL) / (FL + point.z) * 100, WindowSizeY / 2 - (point.y * FL) / (FL + point.z) * 100 };
}

/* Was copied from the Cube code but is not essential to this project im leaving it in for my later use 
* might remove before i submit tho 

Point3D Renderer3D::rotateOnX(Point3D point)
{
    Point3D rotatedPoint;
    rotatedPoint.x = point.x;
    rotatedPoint.y = cos(rotation) * point.y - sin(rotation) * point.z;
    rotatedPoint.z = sin(rotation) * point.y + cos(rotation) * point.z;
    return rotatedPoint;
}

Point3D Renderer3D::rotateOnY(Point3D point)
{
    Point3D rotatedPoint;
    rotatedPoint.x = cos(rotation) * point.x + sin(rotation) * point.z;
    rotatedPoint.y = point.y;
    rotatedPoint.z = -sin(rotation) * point.x + cos(rotation) * point.z;
    return rotatedPoint;
}
*/