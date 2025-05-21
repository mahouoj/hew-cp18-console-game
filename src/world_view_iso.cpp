#include "world_view_iso.h"
#include "world.h"
#include "iso_cube.h"
#include "color.h"
#include "vec.h"
#include "draw.h"
#include <cmath>

static float screen_width_half;
static float screen_height_half;
static float cube_scale_width;
static float cube_scale_height;

static int g_world_height;
static int g_world_width;
static int g_world_depth;

IsoCube cube_prototypes[2];

// utils
Vec2 world_view_iso_world2screen(float world_x, float world_y, float world_z) {
    return Vec2{
        (world_x - world_z) * cube_scale_width,
        -(world_x + world_z + world_y) * cube_scale_height
    };
}

Coord2 world_view_iso_get_draw_offset(float world_x, float world_y, float world_z, Vec2 camera_offset) {
    Vec2 screen_origin = world_view_iso_world2screen(world_x, world_y, world_z);
    return Coord2{
        (int)(screen_origin.x + camera_offset.x - cube_scale_width)  ,
        (int)(screen_origin.y + camera_offset.y - cube_scale_height * 3)
    };
}
// utils end

Vec2 world_view_iso_get_camera_offset(Vec3 camera_focus) {
    Vec2 screen_camera = world_view_iso_world2screen(camera_focus.x, camera_focus.y, camera_focus.z);
    return Vec2{
        std::floor(screen_width_half - screen_camera.x + 0.5f),
        std::floor(screen_height_half - screen_camera.y + 0.5f)
    };
}

Vec2 world_view_iso_world2screen(Vec3 pos, Vec2 camera_offset) {
    return vec2_add(world_view_iso_world2screen(pos.x, pos.y, pos.z), camera_offset);
}

void world_view_iso_init() {
    g_world_height = world_get_height();
    g_world_width = world_get_width();
    g_world_depth = world_get_depth();

    screen_width_half = (float)buffer_get_width() / 2;
    screen_height_half = (float)buffer_get_height() / 2;
    cube_scale_width = iso_cube_get_half_width();
    cube_scale_height = iso_cube_get_half_height();

    cube_prototypes[1] = IsoCube{
        create_color(108, 200, 225),
        create_color(47, 155, 168),
        create_color(33, 111, 117)
    };
}

void world_view_iso_draw(Vec2 camera_offset) {
    for (int y = 0; y < g_world_height; ++y) {
        for (int x = g_world_width - 1; x >= 0; --x) {
            for (int z = g_world_depth - 1; z >= 0; --z) {
                const Block* map_block = world_get_block_at(x, y, z);
                // int block_type = world_get_collision_block_at(x, y, z);
                // if (!map_block || map_block->type == 0) {
                if (!map_block|| map_block->type == BlockViewType_Air) {
                    continue;
                }
                Coord2 draw_offset = world_view_iso_get_draw_offset(x, y, z, camera_offset);
                draw_cube(&cube_prototypes[map_block->type], draw_offset.x, draw_offset.y);
            }
        }
    }
}

void world_view_iso_draw(Vec3 camera_focus) {
    Vec2 camera_offset = world_view_iso_get_camera_offset(camera_focus);
    world_view_iso_draw(camera_offset);
}

float world_view_iso_get_shadow_y(Vec3 pos) {
    for (int y = pos.y; y >= 0; --y) {
        const Block* map_block = world_get_block_at(pos.x, y, pos.z);
        // int block_type = world_get_collision_block_at(pos.x, y, pos.z);
        // if (block_type != 0) {
        if (map_block && map_block->type == BlockViewType_Ground) {
            return y + 1;
        }
    }
    return -1;
}