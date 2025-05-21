#include "scene_test_dungeon.h"
#include "draw.h"
#include "sprite.h"
#include "vec.h"
#define CONIOEX
#include "conioex.h"

static float distance = 0;
static float max_distance = 0;
static float speed = 3.0f;

void scene_test_dungeon_init() {}
void scene_test_dungeon_update(float delta_time) {
    distance += speed * delta_time;
    if (distance >= 1.0f) {
        distance = 0;
    }
}
void scene_test_dungeon_draw() {
    int width = buffer_get_width();
    int height = buffer_get_height();
    Coord2 center{width / 2 - 1, height / 2 };
    Color line_color = create_color(100, 100, 100);
    Pixel line_pixel = create_pixel(L'\u2588', line_color);
    draw_line(center, Coord2{0, 0}, line_pixel);
    draw_line(center, Coord2{ 0, height - 1 }, line_pixel);
    draw_line(Coord2{ center.x + 1, center.y }, Coord2{ width - 1, 0 }, line_pixel, true);
    draw_line(Coord2{ center.x + 1, center.y }, Coord2{ width - 1, height - 1 }, line_pixel, true);

    float camera_height = (float)height / 2.0f;
    float width_half = (float)width / 2.0f;
    float height_half = (float)height / 2.0f;
    Pixel line_pixel_vertical = create_pixel(L'\u2588', create_color(100, 100, 100));
    draw_line(Coord2{ width - 1, height - 1 }, Coord2{ width - 1, height - 1 }, line_pixel_vertical);
    for (int i = 0; i < 5; ++i) {
        float line_distance = (float)(i + 1) - distance;
        float x_half = (width_half - 1) / line_distance;
        float y_half = (height_half - 1) / line_distance;

        int x_left = width_half - x_half;
        int x_right = width_half + x_half;
        int y_top = height_half - y_half;
        int y_bottom = height_half + y_half;
        
        draw_line(Coord2{ x_left, y_top }, Coord2{ x_left, y_bottom }, line_pixel_vertical);
        draw_line(Coord2{ x_right, y_top }, Coord2{ x_right, y_bottom }, line_pixel_vertical);
    }
    draw_fill(Coord2{center.x - 5, center.y - 1}, create_pixel(L'\u2588', create_color(40, 40, 40)), 12, 3);
    
}