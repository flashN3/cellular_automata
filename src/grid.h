#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <SDL3/SDL.h>
#include "common.h"

struct Grid {
    int width, height;
    u8* current_state;
    u8* next_state;
};

void grid_init(Grid* grid, int width, int height);
u8 grid_get_cell(Grid* grid, int x, int y);

void grid_update(Grid* grid);
void grid_render(Grid* grid, SDL_Renderer* renderer);
