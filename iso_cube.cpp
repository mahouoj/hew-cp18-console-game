#include "iso_cube.h"
#include "color.h"
#include "draw.h"
#include <cmath>

const wchar_t glyph_block = L'\u2588';
const wchar_t glyph_right_half = L'\u2590';
const wchar_t glyph_lower_seven_eighths = L'\u2587';
const wchar_t glyph_lower_five_eighths = L'\u2585';
const wchar_t glyph_lower_three_eighths = L'\u2583';
const wchar_t glyph_lower_one_eighth = L'\u2581';

int iso_cube_get_half_width() {
    return 6;
}

int iso_cube_get_half_height() {
    return 2;
}

void draw_pixel_overlay_top(int x, int y, Pixel pixel) {
    Pixel* prev_pixel = buffer_get_pixel_at(x, y);
    // Color color_back = create_color(0, 0, 0); // TODO global background color?
    if (prev_pixel && (prev_pixel->text == glyph_lower_five_eighths
        || prev_pixel->text == glyph_lower_three_eighths
        || prev_pixel->text == L' ')) {
        pixel.color_back = prev_pixel->color_back;
    }
    draw_pixel(x, y, pixel);
}

void draw_pixel_overlay_down(int x, int y, Pixel pixel) {
    Pixel* prev_pixel = buffer_get_pixel_at(x, y);
    // Color color_back = create_color(0, 0, 0); // TODO global background color?
    if (prev_pixel) {
        if (prev_pixel->text == glyph_lower_five_eighths
            || prev_pixel->text == glyph_lower_three_eighths) {
            pixel.color_fore = prev_pixel->color_fore;
        }
        else if (prev_pixel->text == L' ') {
            pixel.color_fore = prev_pixel->color_back;
        }
    }
    draw_pixel(x, y, pixel);
}

void draw_pixel_overlay_left(int x, int y, Pixel pixel) {
    Pixel* prev_pixel = buffer_get_pixel_at(x, y);
    // Color color_back = create_color(0, 0, 0); // TODO global background color?
    if (prev_pixel && (prev_pixel->text == glyph_right_half
        || prev_pixel->text == L' ')) {
        pixel.color_back = prev_pixel->color_back;
    }
    draw_pixel(x, y, pixel);
}

void draw_pixel_overlay_right(int x, int y, Pixel pixel) {
    Pixel* prev_pixel = buffer_get_pixel_at(x, y);
    // Color color_back = create_color(0, 0, 0); // TODO global background color?
    if (prev_pixel) {
        if (prev_pixel->text == glyph_right_half) {
            pixel.color_fore = prev_pixel->color_fore;
        }
        else if (prev_pixel->text == L' ') {
            pixel.color_fore = prev_pixel->color_back;
        }
    }
    draw_pixel(x, y, pixel);
}

void draw_pixel_mix(int x, int y, Pixel pixel, float t) {
    Color color = pixel.color_back;
    Pixel* prev_pixel = buffer_get_pixel_at(x, y);
    if (prev_pixel && prev_pixel->text == L' ') {
        color = lerp(color, prev_pixel->color_back, 1 - t);
    }
    draw_pixel(x, y, create_pixel(color));
}

//void draw_cube(const IsoCube* cube, int offset_x, int offset_y) {
//    // TODO
//    Color color_background = create_color(0, 0, 0);
//
//    Pixel pixel_top = create_pixel(cube->color_top);
//    Pixel pixel_tall = create_pixel(glyph_lower_five_eighths, cube->color_top);
//    Pixel pixel_short = create_pixel(glyph_lower_three_eighths, cube->color_top);
//    // top line 0
//    draw_pixel_overlay_top(offset_x + 3, offset_y, pixel_short);
//    draw_pixel_overlay_top(offset_x + 4, offset_y, pixel_tall);
//    draw_pixel_overlay_top(offset_x + 5, offset_y, pixel_tall);
//    draw_pixel_overlay_top(offset_x + 6, offset_y, pixel_short);
//    // top line 1, line2
//    draw_pixel_overlay_top(offset_x + 0, offset_y + 1, pixel_short);
//    draw_pixel_overlay_top(offset_x + 1, offset_y + 1, pixel_tall);
//    for (int x = 2; x <= 7; ++x) {
//        draw_pixel(offset_x + x, offset_y + 1, pixel_top);
//        draw_pixel(offset_x + x, offset_y + 2, pixel_top);
//    }
//    draw_pixel_overlay_top(offset_x + 8, offset_y + 1, pixel_tall);
//    draw_pixel_overlay_top(offset_x + 9, offset_y + 1, pixel_short);
//
//    // left
//    Pixel pixel_left = create_pixel(cube->color_left);
//    pixel_tall.color_fore = cube->color_left;
//    pixel_tall.color_back = cube->color_top;
//    pixel_short.color_fore = cube->color_left;
//    pixel_short.color_back = cube->color_top;
//    // left line 2
//    draw_pixel(offset_x + 0, offset_y + 2, pixel_tall);
//    draw_pixel(offset_x + 1, offset_y + 2, pixel_short);
//    // left line 3
//    for (int x = 0; x <= 2; ++x) {
//        draw_pixel(offset_x + x, offset_y + 3, pixel_left);
//    }
//    draw_pixel(offset_x + 3, offset_y + 3, pixel_tall);
//    draw_pixel(offset_x + 4, offset_y + 3, pixel_short);
//    // left line 4
//    pixel_tall.color_fore = color_background;
//    pixel_tall.color_back = cube->color_left;
//    pixel_short.color_fore = color_background;
//    pixel_short.color_back = cube->color_left;
//    draw_pixel_overlay_down(offset_x + 0, offset_y + 4, pixel_tall);
//    draw_pixel_overlay_down(offset_x + 1, offset_y + 4, pixel_short);
//    for (int x = 2; x <= 4; ++x) {
//        draw_pixel(offset_x + x, offset_y + 4, pixel_left);
//    }
//    // left line 5
//    draw_pixel_overlay_down(offset_x + 3, offset_y + 5, pixel_tall);
//    draw_pixel_overlay_down(offset_x + 4, offset_y + 5, pixel_short);
//
//    // right
//    Pixel pixel_right = create_pixel(cube->color_right);
//    pixel_tall.color_fore = cube->color_right;
//    pixel_tall.color_back = cube->color_top;
//    pixel_short.color_fore = cube->color_right;
//    pixel_short.color_back = cube->color_top;
//    // right line 2
//    draw_pixel(offset_x + 8, offset_y + 2, pixel_short);
//    draw_pixel(offset_x + 9, offset_y + 2, pixel_tall);
//    // right line 3
//    draw_pixel(offset_x + 5, offset_y + 3, pixel_short);
//    draw_pixel(offset_x + 6, offset_y + 3, pixel_tall);
//    for (int x = 7; x <= 9; ++x) {
//        draw_pixel(offset_x + x, offset_y + 3, pixel_right);
//    }
//    // right line 4
//    pixel_tall.color_fore = color_background;
//    pixel_tall.color_back = cube->color_right;
//    pixel_short.color_fore = color_background;
//    pixel_short.color_back = cube->color_right;
//    for (int x = 5; x <= 7; ++x) {
//        draw_pixel(offset_x + x, offset_y + 4, pixel_right);
//    }
//    draw_pixel_overlay_down(offset_x + 8, offset_y + 4, pixel_short);
//    draw_pixel_overlay_down(offset_x + 9, offset_y + 4, pixel_tall);
//    // left line 5
//    draw_pixel_overlay_down(offset_x + 5, offset_y + 5, pixel_short);
//    draw_pixel_overlay_down(offset_x + 6, offset_y + 5, pixel_tall);
//}

/*
// anti aliasing
void draw_cube(const IsoCube* cube, int offset_x, int offset_y) {
    Pixel pixel_top = create_pixel(cube->color_top);
    Pixel pixel_left = create_pixel(cube->color_left);
    Pixel pixel_right = create_pixel(cube->color_right);
    // top
    // top line 0
    for (int x = 4; x <= 7; ++x) {
        // draw_pixel(offset_x + x, offset_y, pixel_top);
        float t = ((x - 4) * 2 + 1) / 8.0f;
        draw_pixel_mix(offset_x + x, offset_y, pixel_top, t);
        draw_pixel_mix(offset_x + (15 - x), offset_y, pixel_top, t);
    }
    // top line 1
    for (int x = 0; x <= 3; ++x) {
        float t = (x * 2 + 1) / 8.0f;
        draw_pixel_mix(offset_x + x, offset_y + 1, pixel_top, t);
        draw_pixel_mix(offset_x + (15 - x), offset_y + 1, pixel_top, t);
    }
    for (int x = 4; x <= 11; ++x) {
        draw_pixel(offset_x + x, offset_y + 1, pixel_top);
    }
    // top line 2, 3
    for (int x = 0; x <= 15; ++x) {
        draw_pixel(offset_x + x, offset_y + 2, pixel_top);
        draw_pixel(offset_x + x, offset_y + 3, pixel_top);
    }
    // left
    // left line 2
    for (int x = 0; x <= 3; ++x) {
        float t = 1.0f - (x * 2 + 1) / 8.0f;
        draw_pixel_mix(offset_x + x, offset_y + 2, pixel_left, t);
        draw_pixel_mix(offset_x + x + 4, offset_y + 3, pixel_left, t);
    }
    // left line 3
    for (int x = 0; x <= 3; ++x) {
        draw_pixel(offset_x + x, offset_y + 3, pixel_left);
    }
    // left line 4
    for (int x = 0; x <= 7; ++x) {
        draw_pixel(offset_x + x, offset_y + 4, pixel_left);
    }
    // left line 5
    for (int x = 4; x <= 7; ++x) {
        draw_pixel(offset_x + x, offset_y + 5, pixel_left);
    }
    // right
    for (int x = 0; x <= 3; ++x) {
        float t = 1.0f - (x * 2 + 1) / 8.0f;
        draw_pixel_mix(offset_x + 15 - x, offset_y + 2, pixel_right, t);
        draw_pixel_mix(offset_x + 11 - x, offset_y + 3, pixel_right, t);
    }
    for (int x = 12; x <= 15; ++x) {
        draw_pixel(offset_x + x, offset_y + 3, pixel_right);
    }
    // left line 4
    for (int x = 8; x <= 15; ++x) {
        draw_pixel(offset_x + x, offset_y + 4, pixel_right);
    }
    // left line 5
    for (int x = 8; x <= 11; ++x) {
        draw_pixel(offset_x + x, offset_y + 5, pixel_right);
    }
}
*/

/*
// cross
void draw_cube(const IsoCube* cube, int offset_x, int offset_y) {
    Pixel pixel_top = create_pixel(L'\u253C', cube->color_top);
    Pixel pixel_left = create_pixel(L'\u253C', cube->color_left);
    Pixel pixel_right = create_pixel(L'\u253C', cube->color_right);
    // top
    // top line 0, 1, 2
    for (int y = 0; y <= 2; ++y) {
        for (int x = 6 - 3 * y; x <= 8 + 3 * y; ++x) {
            draw_pixel(offset_x + x, offset_y + y, pixel_top);
        }
    }
    // top line 3, 4
    for (int y = 3; y <= 4; ++y) {
        for (int x = 3 * (y - 2); x <= 14 - 3 * (y - 2); ++x) {
            draw_pixel(offset_x + x, offset_y + y, pixel_top);
        }
    }
    // left
    for (int x = 0; x <= 2; ++x) {
        draw_pixel(offset_x + x, offset_y + 3, pixel_left);
    }
    for (int x = 0; x <= 5; ++x) {
        draw_pixel(offset_x + x, offset_y + 4, pixel_left);
    }
    for (int x = 3; x <= 7; ++x) {
        draw_pixel(offset_x + x, offset_y + 5, pixel_left);
    }
    for (int x = 6; x <= 7; ++x) {
        draw_pixel(offset_x + x, offset_y + 6, pixel_left);
    }
    
    // right
    for (int x = 12; x <= 14; ++x) {
        draw_pixel(offset_x + x, offset_y + 3, pixel_right);
    }
    for (int x = 9; x <= 14; ++x) {
        draw_pixel(offset_x + x, offset_y + 4, pixel_right);
    }
    for (int x = 7; x <= 11; ++x) {
        draw_pixel(offset_x + x, offset_y + 5, pixel_right);
    }
    for (int x = 7; x <= 8; ++x) {
        draw_pixel(offset_x + x, offset_y + 6, pixel_right);
    }
}
*/
/*
// border
void draw_cube(const IsoCube* cube, int offset_x, int offset_y) {
    Pixel pixel_top = create_pixel(cube->color_top);
    Pixel pixel_left = create_pixel(cube->color_left);
    Pixel pixel_right = create_pixel(cube->color_right);
    Pixel pixel_border = create_pixel(create_color(cube->color_top.r - 20, cube->color_top.g - 20, cube->color_top.b - 20));

    // top
    for (int x = 6; x <= 8; ++x) {
        draw_pixel(offset_x + x, offset_y, pixel_border);
    }
    for (int x = 3; x <= 5; ++x) {
        draw_pixel(offset_x + x, offset_y + 1, pixel_border);
        draw_pixel(offset_x + x + 3, offset_y + 1, pixel_top);
        draw_pixel(offset_x + x + 6, offset_y + 1, pixel_border);
    }
    for (int x = 0; x <= 2; ++x) {
        draw_pixel(offset_x + x, offset_y + 2, pixel_border);
        draw_pixel(offset_x + x + 12, offset_y + 2, pixel_border);
    }
    for (int x = 3; x <= 11; ++x) {
        draw_pixel(offset_x + x, offset_y + 2, pixel_top);
    }
    for (int x = 3; x <= 5; ++x) {
        draw_pixel(offset_x + x, offset_y + 3, pixel_border);
        draw_pixel(offset_x + x + 3, offset_y + 3, pixel_top);
        draw_pixel(offset_x + x + 6, offset_y + 3, pixel_border);
    }
    for (int x = 6; x <= 8; ++x) {
        draw_pixel(offset_x + x, offset_y + 4, pixel_border);
    }

    // left
    for (int y = 3; y <= 4; ++y) {
        draw_pixel(offset_x, offset_y + y, pixel_border);
        draw_pixel(offset_x + 1, offset_y + y, pixel_left);
        draw_pixel(offset_x + 2, offset_y + y, pixel_left);
    }
    for (int x = 3; x <= 5; ++x) {
        draw_pixel(offset_x + x, offset_y + 4, pixel_left);
    }
    for (int x = 3; x <= 6; ++x) {
        draw_pixel(offset_x + x, offset_y + 5, pixel_left);
    }
    draw_pixel(offset_x + 6, offset_y + 6, pixel_left);
    for (int x = 0; x <= 2; ++x) {
        draw_pixel(offset_x + x, offset_y + 5, pixel_border);
        draw_pixel(offset_x + x + 3, offset_y + 6, pixel_border);
    }
    draw_pixel(offset_x + 6, offset_y + 7, pixel_border);


    // right
    for (int y = 3; y <= 4; ++y) {
        draw_pixel(offset_x + 14, offset_y + y, pixel_border);
        draw_pixel(offset_x + 7, offset_y + y + 2, pixel_border);
        draw_pixel(offset_x + 12, offset_y + y, pixel_right);
        draw_pixel(offset_x + 13, offset_y + y, pixel_right);
    }
    for (int x = 9; x <= 11; ++x) {
        draw_pixel(offset_x + x, offset_y + 4, pixel_right);
    }
    for (int x = 8; x <= 11; ++x) {
        draw_pixel(offset_x + x, offset_y + 5, pixel_right);
    }
    draw_pixel(offset_x + 8, offset_y + 6, pixel_right);
    for (int x = 12; x <= 14; ++x) {
        draw_pixel(offset_x + x, offset_y + 5, pixel_border);
        draw_pixel(offset_x + x - 3, offset_y + 6, pixel_border);
    }
    draw_pixel(offset_x + 7, offset_y + 7, pixel_border);
    draw_pixel(offset_x + 8, offset_y + 7, pixel_border);
}
*/


void draw_cube(const IsoCube* cube, int offset_x, int offset_y) {
    // TODO

    Color color_background = create_color(0, 0, 0);

    Pixel pixel_top = create_pixel(cube->color_top);
    Pixel pixel_tall = create_pixel(glyph_lower_five_eighths, cube->color_top);
    Pixel pixel_short = create_pixel(glyph_lower_three_eighths, cube->color_top);
    // top line 0
    draw_pixel_overlay_top(offset_x + 4, offset_y, pixel_short);
    draw_pixel_overlay_top(offset_x + 5, offset_y, pixel_tall);
    draw_pixel(offset_x + 6, offset_y, pixel_top);
    draw_pixel_overlay_top(offset_x + 7, offset_y, pixel_tall);
    draw_pixel_overlay_top(offset_x + 8, offset_y, pixel_short);
    // top line 1, line2
    draw_pixel_overlay_top(offset_x + 1, offset_y + 1, pixel_short);
    draw_pixel_overlay_top(offset_x + 2, offset_y + 1, pixel_tall);
    for (int x = 3; x <= 9; ++x) {
        draw_pixel(offset_x + x, offset_y + 1, pixel_top);
        draw_pixel(offset_x + x, offset_y + 2, pixel_top);
    }
    draw_pixel_overlay_top(offset_x + 10, offset_y + 1, pixel_tall);
    draw_pixel_overlay_top(offset_x + 11, offset_y + 1, pixel_short);
    // top line 3
    draw_pixel(offset_x + 6, offset_y + 3, pixel_top);

    // left
    Pixel pixel_left = create_pixel(cube->color_left);
    Pixel pixel_left_right = create_pixel(glyph_right_half, cube->color_right, cube->color_left);
    Pixel pixel_left_half = create_pixel(glyph_right_half, cube->color_left);
    pixel_tall.color_fore = cube->color_left;
    pixel_tall.color_back = cube->color_top;
    pixel_short.color_fore = cube->color_left;
    pixel_short.color_back = cube->color_top;
    // left line 2
    draw_pixel_overlay_left(offset_x, offset_y + 2, pixel_left_half);
    draw_pixel(offset_x + 1, offset_y + 2, pixel_tall);
    draw_pixel(offset_x + 2, offset_y + 2, pixel_short);
    // left line 3
    draw_pixel_overlay_left(offset_x, offset_y + 3, pixel_left_half);
    for (int x = 1; x <= 3; ++x) {
        draw_pixel(offset_x + x, offset_y + 3, pixel_left);
    }
    draw_pixel(offset_x + 4, offset_y + 3, pixel_tall);
    draw_pixel(offset_x + 5, offset_y + 3, pixel_short);
    // left line 4
    pixel_tall.color_fore = color_background;
    pixel_tall.color_back = cube->color_left;
    pixel_short.color_fore = color_background;
    pixel_short.color_back = cube->color_left;
    draw_pixel_overlay_down(offset_x + 1, offset_y + 4, pixel_tall);
    draw_pixel_overlay_down(offset_x + 2, offset_y + 4, pixel_short);
    for (int x = 3; x <= 5; ++x) {
        draw_pixel(offset_x + x, offset_y + 4, pixel_left);
    }
    draw_pixel(offset_x + 6, offset_y + 4, pixel_left_right);
    // left line 5
    draw_pixel_overlay_down(offset_x + 4, offset_y + 5, pixel_tall);
    draw_pixel_overlay_down(offset_x + 5, offset_y + 5, pixel_short);
    draw_pixel(offset_x + 6, offset_y + 5, pixel_left_right);

    // right
    Pixel pixel_right = create_pixel(cube->color_right);
    Pixel pixel_right_half = create_pixel(glyph_right_half, color_background, cube->color_right);
    pixel_tall.color_fore = cube->color_right;
    pixel_tall.color_back = cube->color_top;
    pixel_short.color_fore = cube->color_right;
    pixel_short.color_back = cube->color_top;
    // right line 2
    draw_pixel(offset_x + 10, offset_y + 2, pixel_short);
    draw_pixel(offset_x + 11, offset_y + 2, pixel_tall);
    draw_pixel_overlay_right(offset_x + 12, offset_y + 2, pixel_right_half);
    // right line 3
    draw_pixel(offset_x + 7, offset_y + 3, pixel_short);
    draw_pixel(offset_x + 8, offset_y + 3, pixel_tall);
    for (int x = 9; x <= 11; ++x) {
        draw_pixel(offset_x + x, offset_y + 3, pixel_right);
    }
    draw_pixel_overlay_right(offset_x + 12, offset_y + 3, pixel_right_half);
    // right line 4
    pixel_tall.color_fore = color_background;
    pixel_tall.color_back = cube->color_right;
    pixel_short.color_fore = color_background;
    pixel_short.color_back = cube->color_right;
    for (int x = 7; x <= 9; ++x) {
        draw_pixel(offset_x + x, offset_y + 4, pixel_right);
    }
    draw_pixel_overlay_down(offset_x + 10, offset_y + 4, pixel_short);
    draw_pixel_overlay_down(offset_x + 11, offset_y + 4, pixel_tall);
    // left line 5
    draw_pixel_overlay_down(offset_x + 7, offset_y + 5, pixel_short);
    draw_pixel_overlay_down(offset_x + 8, offset_y + 5, pixel_tall);
}
