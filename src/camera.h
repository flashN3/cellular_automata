#pragma once

struct Camera {
    int x;
    int y;
    int width;
    int height;
    float zoom;
};

void camera_update(Camera* camera);

