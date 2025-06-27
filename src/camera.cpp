#include "camera.h"
#include <SDL3/SDL.h>

void camera_update(Camera *camera)
{
    const bool* keyboard_state = SDL_GetKeyboardState(nullptr);
    int move_speed = 8;

    if (keyboard_state[SDL_SCANCODE_W]) camera->y -= move_speed;
    if (keyboard_state[SDL_SCANCODE_S]) camera->y += move_speed;
    if (keyboard_state[SDL_SCANCODE_A]) camera->x -= move_speed;
    if (keyboard_state[SDL_SCANCODE_D]) camera->x += move_speed;
}
