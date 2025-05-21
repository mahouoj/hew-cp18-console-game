#pragma once
#include "iso_cube.h"
#include "color.h"
#include "vec.h"

const static int map_height = 10;
const static int map_width = 10;
const static int map_depth = 10;
const static int cube_types = 3;

enum CubeType {
    CubeTest0,
    CubeTest1,
    CuebTypesCnt
};

struct MapCube {
    CubeType type;
    bool active;
};

static IsoCube cube_prototypes[CuebTypesCnt];
static MapCube map[map_height][map_width][map_depth];

void init_map();
void draw_map(Vec3 camera_focus);