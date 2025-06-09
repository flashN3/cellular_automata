#include "grid.h"

internal void grid_set_cell(Grid* grid, int x, int y, u8 value)
{
    grid->next_state[grid->width * y + x].alive = value;
}

internal void grid_set_cell_current(Grid* grid, int x, int y, u8 value)
{
    grid->current_state[grid->width * y + x].alive = value;
}

void grid_init(Grid *grid, int width, int height)
{
    grid->width = width;
    grid->height = height;

    int total_cells = width * height;
    grid->current_state = (Cell*)malloc(total_cells * sizeof(Cell));
    grid->next_state = (Cell*)malloc(total_cells * sizeof(Cell));

    memset(grid->current_state, 0, total_cells * sizeof(Cell));
    memset(grid->next_state, 0, total_cells * sizeof(Cell));

     // Glider 1 (southeast)
    grid_set_cell_current(grid, 10, 10, 1);
    grid_set_cell_current(grid, 11, 11, 1);
    grid_set_cell_current(grid, 12, 9, 1);
    grid_set_cell_current(grid, 12, 10, 1);
    grid_set_cell_current(grid, 12, 11, 1);

     // Glider 1 (southeast)
    grid_set_cell_current(grid, 30, 10, 1);
    grid_set_cell_current(grid, 31, 11, 1);
    grid_set_cell_current(grid, 32, 9, 1);
    grid_set_cell_current(grid, 32, 10, 1);
    grid_set_cell_current(grid, 32, 11, 1);
 
    
    // Glider 2 (different orientation - northwest)
    grid_set_cell_current(grid, 50, 30, 1);
    grid_set_cell_current(grid, 51, 30, 1);
    grid_set_cell_current(grid, 52, 30, 1);
    grid_set_cell_current(grid, 52, 29, 1);
    grid_set_cell_current(grid, 51, 28, 1);

    
    // Oscillators for visual interest
    // Blinker
    grid_set_cell_current(grid, 20, 20, 1);
    grid_set_cell_current(grid, 21, 20, 1);
    grid_set_cell_current(grid, 22, 20, 1);
    
    // Toad
    grid_set_cell_current(grid, 30, 25, 1);
    grid_set_cell_current(grid, 31, 25, 1);
    grid_set_cell_current(grid, 32, 25, 1);
    grid_set_cell_current(grid, 31, 26, 1);
    grid_set_cell_current(grid, 32, 26, 1);
    grid_set_cell_current(grid, 33, 26, 1);
}

Cell grid_get_cell(Grid *grid, int x, int y)
{
    return grid->current_state[y * grid->width + x];
}

internal void grid_swap_buffers(Grid *grid)
{
    Cell* temp = grid->current_state;
    grid->current_state = grid->next_state;
    grid->next_state = temp;
}

internal int grid_count_neighbours(Grid* grid, int x, int y) {

    int count = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {

            if (dx == 0 && dy == 0) continue;

            int nx = (x + dx + grid->width) % grid->width;
            int ny = (y + dy + grid->height) % grid->height;
            count += grid_get_cell(grid, nx, ny).alive;
        }
    }

    return count;
}

void grid_update(Grid* grid)
{
    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {

            Cell current = grid_get_cell(grid, x, y);
            int num_neighbours = grid_count_neighbours(grid, x, y);
            
            if (current.alive) {
                int value = (num_neighbours == 2 || num_neighbours == 3) ? 1 : 0;
                grid_set_cell(grid, x, y, value);
            } else {
                int value = (num_neighbours == 3) ? 1 : 0;
                grid_set_cell(grid, x, y, value);
            }
        }
    }

    // Double buffering
    grid_swap_buffers(grid);
}


void grid_render(Grid* grid, SDL_Renderer* renderer) {

    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {

            float cell_size = 8.0f;
            int screen_x = x * cell_size;
            int screen_y = y * cell_size;

            Cell cell = grid_get_cell(grid, x, y);

            if (cell.alive) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }

            SDL_FRect cell_rect = {
                (float)screen_x,
                (float)screen_y,
                cell_size,
                cell_size
            };

            SDL_RenderFillRect(renderer, &cell_rect);
        }
    }
}
