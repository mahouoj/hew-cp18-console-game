#pragma once
#include "color.h"

struct IsoCube{
    Color color_top;
    Color color_left;
    Color color_right;
};

int iso_cube_get_half_width();
int iso_cube_get_half_height();

void draw_cube(const IsoCube* cube, int offset_x, int offset_y);