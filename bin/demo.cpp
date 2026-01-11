#include <Eigen/Core>
#include <SDL3/SDL.h>
#include <iostream>

int main()
{
    // Connect to display driver
    SDL_Init(SDL_INIT_VIDEO);

    // Create window
    SDL_Window* window = SDL_CreateWindow("Window name", 640,480,0);

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window,NULL);

    // Draw one pixel
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderLine(renderer,320,240,640,480);
    SDL_RenderPresent(renderer);

    //Keep the screen alive for some time
    SDL_Delay(3000);

    // Destroy screen
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}