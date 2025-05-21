#pragma once
#include "vec.h"

void world_view_iso_init();
void world_view_iso_draw(Vec3 camera_focus);
void world_view_iso_draw(Vec2 camera_offset);
Vec2 world_view_iso_get_camera_offset(Vec3 camera_focus);
Vec2 world_view_iso_world2screen(Vec3 pos, Vec2 camera_offset);
float world_view_iso_get_shadow_y(Vec3 pos);