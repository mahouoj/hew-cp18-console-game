#include "scene_racing.h"
#include "draw.h"
#include "sprite.h"
#define CONIOEX
#include "conioex.h"

//Coord2 p1, p2;
//bool p1Ready = false;
//bool p2Ready = false;
//bool mouseLoading = false;
Sprite car_sprites[3];

float distance = 0;
float speed_rate = 0;
float speed_max = 2.0f;
int direction = 0;

// track
float track_segments[10][2];
int track_curr_segment = 0;
float track_curr_offset = 0;
float curvature = 0;
float curvature_track = 0;
float curvature_player = 0;

void scene_racing_init() {
    const wchar_t* car_sprite_center = {
        L"    ||##||    "
        L"      ##      "
        L"     ####     "
        L"|||########|||"
        L"|||  ####  |||"
    };
    const wchar_t* car_sprite_left = {
        L"  ||##||      "
        L"     ##       "
        L"     ####     "
        L"|||#######||| "
        L" |||  #### |||"
    };
    const wchar_t* car_sprite_right = {
        L"      ||##||  "
        L"       ##     "
        L"     ####     "
        L" |||#######|||"
        L"||| ####  ||| "
    };
    int car_sprite_width = 14;
    int car_sprite_height = 5;
    sprite_init(car_sprites, car_sprite_width, car_sprite_height, car_sprite_center, create_color(255, 255, 0));
    sprite_init(car_sprites + 1, car_sprite_width, car_sprite_height, car_sprite_left, create_color(255, 255, 0));
    sprite_init(car_sprites + 2, car_sprite_width, car_sprite_height, car_sprite_right, create_color(255, 255, 0));

    // init track
    float track_data[10][2] = {
        {0.0f, 10.0f},
        {0.0f, 200.0f},
        {1.0f, 200.0f},
        {0.0f, 400.0f},
        {-1.0f, 100.0f},
        {0.0f, 10.0f},
        {0.0f, 200.0f},
        {1.0f, 200.0f},
        {0.0f, 400.0f},
        {-1.0f, 100.0f},
    };
    for (int i = 0; i < 10; ++i) {
        track_segments[i][0] = track_data[i][0];
        track_segments[i][1] = track_data[i][1];
    }
}

void scene_racing_update(float deltaTime) {
    //if (!mouseLoading && inport(PM_LEFT) != 0) {
    //	if (!p1Ready) {
    //		p1 = Coord2{ (inport(PM_CURX) - 1), inport(PM_CURY) - 1 };
    //		p1Ready = true;
    //		p2Ready = false;
    //		mouseLoading = true;
    //	}
    //	else {
    //		p2 = Coord2{ (inport(PM_CURX) - 1), inport(PM_CURY) - 1 };
    //		p2Ready = true;
    //		p1Ready = false;
    //		mouseLoading = true;
    //	}
    //}
    //if (inport(PM_LEFT) == 0) {
    //	mouseLoading = false;
    //}


    if (inport(PK_UP)) {
        // distance += 0.2f * deltaTime;
        speed_rate += 2.0f * deltaTime;
    }
    else {
        speed_rate -= 1.0f * deltaTime;
    }
    speed_rate = clamp(0.0f, speed_rate, 1.0f);
    distance += speed_max * speed_rate;

    direction = 0;
    if (inport(PK_LEFT)) {
        direction -= 1;
        curvature_player -= 1.0f * deltaTime;
    }
    if (inport(PK_RIGHT)) {
        direction += 1;
        curvature_player += 1.0f * deltaTime;
    }

    // track
    while (distance - track_curr_offset > track_segments[track_curr_segment][1]) {
        track_curr_offset += track_segments[track_curr_segment][1];
        // curvature_acc += track_segments[track_curr_segment][0];
        track_curr_segment = (track_curr_segment + 1) % 10;
    }
    float target_curvature = track_segments[track_curr_segment][0];
    //float curvature_speed = 0.001 * deltaTime;
    curvature += speed_rate * deltaTime * (target_curvature - curvature);
    curvature_track += speed_rate * deltaTime * curvature;
    // curvature += speed_rate * clamp(-curvature_speed, target_curvature - curvature, curvature_speed);
    // curvature_track = speed_rate * clamp(-curvature_speed, target_curvature - curvature_track, curvature_speed);
}

void scene_racing_draw() {
    //Pixel pixel{
    //		create_color(255, 0, 0),
    //		create_color(0, 128, 128),
    //		L'#'
    //};
    //if (p2Ready) {
    //	draw_line(
    //		p1, p2, pixel
    //	);
    //}


    // gotoxy(3, 3);
    // printf("Hello");

    //Pixel p{
    //		create_color(255, 0, 0),
    //		create_color(0, 128, 128),
    //		L'#'
    //};

    //draw_pixel(
    //	&p,
    //	Coord2{ 3, 3 }
    //);
    //for (int i = 0; i < 25; ++i) {
    //	for (int j = 0; j < 80; ++j) {
    //		Pixel p{
    //			create_color(i * 5, 0, 0),
    //			create_color(0, j, 128),
    //			L'#'
    //		};
    //		draw_pixel(
    //			&p,
    //			Coord2{ j, i }
    //		);
    //	}
    //}
    int height = buffer_get_height();
    int width = buffer_get_width();
    int height_horizon = height * 0.2;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (y < height_horizon) {
                draw_pixel(Coord2{ x, y }, create_pixel(create_color(0, 0, 255)));
            }
            else {
                Color color;

                float perspective = (float)(y - height_horizon) / (height - height_horizon);
                float road_middle = 0.5f + curvature * powf((1.0f - perspective), 3);
                float road_width = 0.08f + perspective * 0.6; // half
                float clip_width = road_width * 0.2f;
                road_width *= 0.5f;

                int left_grass = (road_middle - road_width - clip_width) * width;
                int left_clip = (road_middle - road_width) * width;
                int right_clip = (road_middle + road_width) * width;
                int right_grass = (road_middle + road_width + clip_width) * width;

                Color grass_color = sinf(10.0f * powf(1.0f - perspective, 3) + distance * 0.1f) > 0.0f ?
                    create_color(0, 255, 0) : create_color(0, 100, 0);
                Color clip_color = sinf(40.0f * powf(1.0f - perspective, 2) + distance * 0.1f) > 0.0f ?
                    create_color(255, 0, 0) : create_color(255, 255, 255);


                if (x < left_grass) {
                    color = grass_color; // grass
                }
                else if (x < left_clip) {
                    color = clip_color; // clip
                }
                else if (x < right_clip) {
                    color = create_color(100, 100, 100); // road
                }
                else if (x < right_grass) {
                    color = clip_color; // clip
                }
                else {
                    color = grass_color; // grass
                }
                draw_pixel(Coord2{ x, y }, create_pixel(color));
            }
        }
    }

    // draw car
    Coord2 car_pos;
    car_pos.x = (width - car_sprites[0].width) / 2 + (curvature_player - curvature_track) * width;
    car_pos.y = (height - car_sprites[0].height) - 2;

    switch (direction) {
    case -1:
        draw_sprite(car_pos, car_sprites + 1);
        break;
    case 0:
        draw_sprite(car_pos, car_sprites);
        break;
    case 1:
        draw_sprite(car_pos, car_sprites + 2);
        break;
    }

}