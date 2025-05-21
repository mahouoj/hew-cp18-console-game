#include "scene_tilemap.h"
#define CONIOEX
#include "conioex.h"
#include "draw.h"
#include "sprite.h"

const wchar_t* map = {
    L"                                "
    L"                                "
    L"          #          ##         "
    L"                  #####    #####"
    L"#########################  #####"
    L"                        #       "
    L"                     ## #       "
    L"                 ########  #####"
    L"########   #####          #####"
    L"       #       # ##########     "
    L"       #       #                "
    L"       #       #######          "
    L"       #                   #####"
    L"       ####      ###############"
    L"                                "
    L"                                "
    L"################################"
};
const int map_width = 32;
const int map_height = 16;

static int tile_width = 4;
static int tile_height = 2;
static float tile_width_inv = 1.0f / tile_width;
static float tile_height_inv = 1.0f / tile_height;

static int visible_tiles_x;
static int visible_tiles_y;

float player_speed = 10.0f;

Vec2 camera_pos;
Vec2 player_pos;
Vec2 player_velocity;

wchar_t get_tile(int x, int y) {
    if (x < 0 || x >= map_width || y < 0 || y >= map_height) {
        return L'X';
    }
    return map[y * map_width + x];
}

void scene_tilemap_init() {
    visible_tiles_x = buffer_get_width() * tile_width_inv;
    visible_tiles_y = buffer_get_height() * tile_height_inv;
}
void scene_tilemap_update(float delta_time) {
    Vec2 target_velocity{ 0, 0 };
    if (inport(PK_UP)) {
        // player_pos.y -= 20.0f * delta_time;
        target_velocity.y = -player_speed - 10.0f;
    }
    if (inport(PK_DOWN)) {
        // player_pos.y += 20.0f * delta_time;
        target_velocity.y = player_speed;
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

    player_velocity.y += 10.0f; // gravity

    Vec2 player_pos_new = {
        round(player_pos.x + player_velocity.x * delta_time, tile_width_inv),
        // setting heigh makes block unable to move when speed is low
        round(player_pos.y + player_velocity.y * delta_time, tile_width_inv)
    };

    // check collision
    if (player_velocity.x <= 0) { // moving left

        if (get_tile(player_pos_new.x, player_pos.y) != L' ' || get_tile(player_pos_new.x, player_pos.y + 0.9) != L' ') {
            player_pos_new.x = (int)player_pos_new.x + 1;
            player_velocity.x = 0;
        }
    }
    else { // moving right
        if (get_tile(player_pos_new.x + 1.0f, player_pos.y) != L' ' || get_tile(player_pos_new.x + 1.0f, player_pos.y + 0.9) != L' ') {
            player_pos_new.x = (int)player_pos_new.x;
            player_velocity.x = 0;
        }
    }
    if (player_velocity.y <= 0) { // moving up
        if (get_tile(player_pos_new.x, player_pos_new.y) != L' ' || get_tile(player_pos_new.x + 0.9, player_pos_new.y) != L' ') {
            player_pos_new.y = (int)player_pos_new.y + 1;
            player_velocity.y = 0;
        }
    }
    else { // moving down
        if (get_tile(player_pos_new.x, player_pos_new.y + 1.0f) != L' ' || get_tile(player_pos_new.x + 0.9, player_pos_new.y + 1.0f) != L' ') {
            player_pos_new.y = (int)player_pos_new.y;
            player_velocity.y = 0;
        }
    }

    player_pos = player_pos_new;

    camera_pos.x = player_pos.x;
    camera_pos.y = player_pos.y;
}

void scene_tilemap_draw() {
    Vec2 offset = {
        camera_pos.x - visible_tiles_x / 2.0f,
        camera_pos.y - visible_tiles_y / 2.0f
    };
    offset.x = clamp(0.0f, offset.x, (float)(map_width - visible_tiles_x));
    offset.y = clamp(0.0f, offset.y, (float)(map_height - visible_tiles_y));

    // draw tilemap
    for (int i = -1; i < visible_tiles_y + 1; ++i) {
        for (int j = -1; j < visible_tiles_x + 1; ++j) {
            wchar_t tile = get_tile(j + offset.x, i + offset.y);
            Color color = tile == L' ' ? create_color(0, 0, 0) : create_color(255, 255, 255);
            draw_fill(Coord2{ j * tile_width, i * tile_height }, create_pixel(color), tile_width, tile_height);
        }
    }
    // draw player
    draw_fill(Coord2{ (int)((player_pos.x - offset.x) * tile_width + 0.5) , (int)((player_pos.y - offset.y) * tile_height + 0.5) }, create_pixel(create_color(255, 255, 0)), tile_width, tile_height);
}