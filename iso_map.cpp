#include "iso_map.h"
#include "color.h"
#include "vec.h"
#include "draw.h"
#include <cmath>

static float screen_width_half;
static float screen_height_half;
static float cube_scale_width;
static float cube_scale_height;

void init_map() {
    // init screen
    screen_width_half = (float)buffer_get_width() / 2;
    screen_height_half = (float)buffer_get_height() / 2;
    cube_scale_width = iso_cube_get_half_width();
    cube_scale_height = iso_cube_get_half_height();
    // 
    // init prototypes
    cube_prototypes[CubeTest0] = IsoCube{
         create_color(108, 200, 225),
         create_color(47, 155, 168),
         create_color(33, 111, 117)
    };
    cube_prototypes[CubeTest1] = IsoCube{
         create_color(120, 250, 240),
         create_color(47, 155, 168),
         create_color(33, 111, 117)
    };
    // init map
    for (int k = 0; k < 1; ++k) {
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                if (k == 0 || i % 2) {
                    map[k][i][j] = MapCube{ (CubeType)(i % 2), true };
                }
            }
        }
    }
}

Vec2 world2screen(float world_x, float world_y, float world_z) {
    return Vec2{
        (world_x - world_z) * cube_scale_width,
        -(world_x + world_z + world_y) * cube_scale_height
    };
}

Coord2 get_draw_offset(float world_x, float world_y, float world_z,
    float camera_offset_x, float camera_offset_y) {
    Vec2 screen_origin = world2screen(world_x, world_y, world_z);
    return Coord2{
        (int)(screen_origin.x + camera_offset_x - cube_scale_width)  ,
        (int)(screen_origin.y + camera_offset_y - cube_scale_height * 3)
    };
}

Vec2 get_camera_offset(float world_x, float world_y, float world_z) {
    Vec2 screen_camera = world2screen(world_x, world_y, world_z);
    return Vec2{
        std::floor(screen_width_half - screen_camera.x + 0.5f),
        std::floor(screen_height_half - screen_camera.y + 0.5f)
    };
}

void draw_map(Vec3 camera_focus) {
    Vec2 camera_offset = get_camera_offset(camera_focus.x, camera_focus.y, camera_focus.z);
    for (int y = 0; y < map_height; ++y) {
        for (int x = map_width - 1; x >= 0; --x) {
            for (int z = map_depth - 1; z >= 0; --z) {
                MapCube map_cube = map[y][x][z];
                if (!map_cube.active) {
                    continue;
                }
                Coord2 draw_offset = get_draw_offset(x, y, z, camera_offset.x, camera_offset.y);
                draw_cube(&cube_prototypes[map_cube.type], draw_offset.x, draw_offset.y);
            }
        }
    }
}