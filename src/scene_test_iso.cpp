#include "scene_test_iso.h"
#include "draw.h"
#include "sprite.h"
#include "vec.h"
#include "iso_cube.h"
#include "iso_map.h"
#define CONIOEX
#include "conioex.h"

static IsoCube cubes[2];

static Vec3 player_pos;
static Vec3 player_velocity;
static float player_speed = 1.0f;
static Coord2 player_pos_screen;
static Sprite player_sprite;

void scene_test_iso_init() {
    cubes[0].color_top = create_color(108, 200, 225);
    cubes[0].color_left = create_color(47, 155, 168);
    cubes[0].color_right = create_color(33, 111, 117);

    //cubes[1].color_top = create_color(150, 150, 150);
    //cubes[1].color_left = create_color(0, 255, 100);
    //cubes[1].color_right = create_color(0, 100, 255);
    cubes[1].color_top = create_color(108, 200, 225);
    cubes[1].color_left = create_color(47, 155, 168);
    cubes[0].color_right = create_color(33, 111, 117);

    init_map();

    sprite_load(&player_sprite, L"assets/isoplayer.txt");

    player_pos = Vec3{ 2.5, 0, 2.5 };
}

void scene_test_iso_update(float delta_time) {
    int width = buffer_get_width();
    int height = buffer_get_height();
    player_pos_screen = Coord2{ width / 2 - 2, height / 2 - 2 };

    Vec3 target_velocity{ 0, 0, 0 };
    if (inport(PK_UP)) {
        // player_pos.y -= 20.0f * delta_time;
        target_velocity.z = player_speed;
    }
    if (inport(PK_DOWN)) {
        // player_pos.y += 20.0f * delta_time;
        target_velocity.z = -player_speed;
    }
    if (inport(PK_LEFT)) {
        // player_pos.x -= 20.0f * delta_time;
        target_velocity.x = -player_speed;
    }
    if (inport(PK_RIGHT)) {
        // player_pos.x += 20.0f * delta_time;
        target_velocity.x = player_speed;
    }
    player_velocity = target_velocity; // TODO
    player_pos = vec3_add(player_pos, vec3_multiply(player_velocity, delta_time));

    //player_pos_screen.x = ((float)width / 2.0f) + (player_pos.x - player_pos.z) * 6.0f + 0.5;
    //player_pos_screen.y = ((float)height / 2.0f) + (player_pos.x + player_pos.z) * 2.0f + 0.5;
}

void scene_test_iso_draw() {
    // draw_map(Vec3{1.5, 0, 1.5});
    draw_map(player_pos);
    //int offset_x = 40;
    //int offset_y = 5;

    //for (int y = 0; y < 3; ++y) {
    //    for (int x = 0; x < 3; ++x) {
    //        int id = x % 2;
    //        int px = (x - y) * 6;
    //        int py = (x + y) * 2;
    //        draw_cube(cubes + id, offset_x + px, py + offset_y);
    //    }
    //}

    draw_sprite(player_pos_screen, &player_sprite);

    //offset_x = 10;
    //offset_y = 8;

    //for (int y = 0; y < 2; ++y) {
    //    for (int x = 0; x < 2; ++x) {
    //        int id = (x + y) % 2;
    //        int px = (x - y - 1) * 6;
    //        int py = (x + y - 1) * 2;
    //        draw_cube(cubes + id, offset_x + px, py + offset_y);
    //    }
    //}
}