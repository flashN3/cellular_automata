#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <SDL3/SDL.h>
#include "common.h"

struct Cell {
    u8 alive;
    u8 age;
};

struct Grid {
    int width, height;
    Cell* current_state;
    Cell* next_state;
};

struct Pattern {
    int width;
    int height;
    Cell* cells;
};

void grid_init(Grid* grid, int width, int height);
Cell grid_get_cell(Grid* grid, int x, int y);

void grid_update(Grid* grid);
void grid_render(Grid* grid, SDL_Renderer* renderer);
