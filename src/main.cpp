#include <SDL3/SDL.h>
#include <stdio.h>
#include <grid.h>
#include "camera.h"

constexpr int screen_width{ 1920 };
constexpr int screen_height{ 1080 };

SDL_Window* window{ nullptr };
SDL_Renderer* renderer{ nullptr };

int main() {
        
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init Failed: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Cellular Automata", screen_width, screen_height, 0);
    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
        }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
        }


    Camera camera{};
    camera.x = 0;
    camera.y = 0;
    camera.height = screen_height / 8.0f;
    camera.width = screen_width / 8.0f;

    
    Grid grid;
    grid_init(&grid, 2 * screen_width / 8.0f, 2 * screen_height / 8.0f);


    bool is_running = true;
    while (is_running)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
            {
                is_running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // 
        camera_update(&camera);

        grid_update(&grid);
        grid_render(&grid, renderer, &camera);

        SDL_RenderPresent(renderer);

        SDL_Delay(64);

    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}