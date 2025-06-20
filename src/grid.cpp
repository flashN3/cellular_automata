#include "grid.h"

#include <cstdio>

internal void grid_set_cell(Grid* grid, int x, int y, Cell cell)
{
    grid->next_state[grid->width * y + x] = cell;
}

internal void grid_set_cell_current(Grid* grid, int x, int y, u8 value)
{
    grid->current_state[grid->width * y + x].alive = value;
}

internal void parse_pattern_from_RLE(const char* pattern_rle_data, Pattern* out_pattern) {

    //
    const char* data = 
    "#N $rats\n"
    "#O David Buckingham\n"
    "#C A period 6 oscillator found in 1972.\n"
    "#C www.conwaylife.com/wiki/index.php?title=$rats\n"
    "x = 12, y = 11, rule = B3/S23\n"
    "5b2o5b$6bo5b$4bo7b$2obob4o3b$2obo5bobo$3bo2b3ob2o$3bo4bo3b$4b3obo3b$7bo4b$6bo5b$6b2o!";
    
    data = 
    "#N 1 beacon\n"
    "#C Approximately the 32nd-most common oscillator.\n"
    "#C www.conwaylife.com/wiki/index.php?title=1_beacon\n"
    "x = 7, y = 7, rule = b3/s23\n"
    "2b2o3b$bobo3b$o2bob2o$2obo2bo$bobo3b$bo2bo2b$2b2o!\n";

    data =
    "#N 104P177\n"
    "#O Karel Suhajda\n"
    "#C A period 177 oscillator discovered by Karel Suhajda in June 2007\n"
    "#C http://www.conwaylife.com/wiki/index.php?title=104P177\n"
    "x = 46, y = 46, rule = b3/s23\n"
    "16bo12bo16b$9b2o24b2o9b$8b3o3b2o14b2o3b3o8b$14b2ob2o8b2ob2o14b$16bo12b"
    "o16b4$2bo40bo2b$b2o40b2ob$b2o40b2ob4$2b2o38b2o2b$2b2o38b2o2b$o3bo36bo"
    "3bo$3bo38bo3b$3bo38bo3b9$3bo38bo3b$3bo38bo3b$o3bo36bo3bo$2b2o38b2o2b$"
    "2b2o38b2o2b4$b2o40b2ob$b2o40b2ob$2bo40bo2b4$16bo12bo16b$14b2ob2o8b2ob"
    "2o14b$8b3o3b2o14b2o3b3o8b$9b2o24b2o9b$16bo12bo!";

    const char* line = data;
    
    // skip comments
    while (*line == '#') {
        while (*line && *line != '\n') line++;
        if (*line == '\n') line++;
    }

    sscanf_s(line, "x = %d, y = %d", &out_pattern->width, &out_pattern->height);
    while (*line != '\n') line++;
    line++;

    out_pattern->cells = (Cell*)calloc(out_pattern->width * out_pattern->height, sizeof(Cell));

    int x = 0, y = 0;
    int num_entries = 0;

    while (*line && *line != '!') {
        if (*line >= '0' && *line <= '9') {
            num_entries = num_entries * 10 + (*line - '0');
        } else {
            if (num_entries == 0) num_entries = 1;

            switch(*line) {
                case 'b': {
                    x += num_entries;
                } break;
                case 'o': {
                    for (int i = 0; i < num_entries; i++) {
                        if (x < out_pattern->width && y < out_pattern->height) {
                            Cell cell;
                            cell.alive = 1;
                            out_pattern->cells[y * out_pattern->width + x] = cell;
                        }
                        x++;
                    }
                } break;
                case '$': {
                    y += num_entries;
                    x = 0;
                } break;
            }

            num_entries = 0;
        }

        line++;
    }

    printf("Read patern:\n");
    printf("(%d, %d)\n", out_pattern->width, out_pattern->height);
    for (int y = 0; y < out_pattern->height; y++) {
        for (int x  = 0; x < out_pattern->width; x++) {
            Cell cell = out_pattern->cells[y * out_pattern->width + x];
            printf("%d", cell.alive);
        }
        printf("\n");
    }
    
}

internal void grid_place_pattern(Grid* grid, Pattern* pattern, int x_pos, int y_pos) {

    for (int y = y_pos; y < y_pos + pattern->height; y++) {
        for (int x = x_pos; x < x_pos + pattern->width; x++) {
            grid_set_cell_current(grid, x, y, pattern->cells[(y - y_pos) * pattern->width + (x - x_pos)].alive);
        }
    }
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

    Pattern pattern;
    parse_pattern_from_RLE(nullptr, &pattern);

    grid_place_pattern(grid, &pattern, 100, 50);
    grid_place_pattern(grid, &pattern, 20, 20);
    grid_place_pattern(grid, &pattern, 160, 20);
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

            Cell cell = grid_get_cell(grid, x, y);
            int num_neighbours = grid_count_neighbours(grid, x, y);
            
            if (cell.alive) {
                cell.alive = (num_neighbours == 2 || num_neighbours == 3) ? 1 : 0;
                cell.age += 1;
                grid_set_cell(grid, x, y, cell);
            } else {
                cell.alive = (num_neighbours == 3) ? 1 : 0;
                cell.age = 0;
                grid_set_cell(grid, x, y, cell);
            }
        }
    }

    // Double buffering
    grid_swap_buffers(grid);
}

void grid_render(Grid* grid, SDL_Renderer* renderer, const Camera* camera) {

    // Render only the part the camera can see
    for (int y = camera->y; y < camera->height; y++) {
        for (int x = camera->x; x < camera->width; x++) {

            float cell_size = 8.0f;
            int screen_x = (x - camera->x) * cell_size;
            int screen_y = (y - camera->y) * cell_size;

            Cell cell = grid_get_cell(grid, x, y);

            if (cell.alive) {
                u8 color = 255 - cell.age;    
                SDL_SetRenderDrawColor(renderer, color, color, color, 255);
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
