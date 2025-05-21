#include "scene_test_sprite.h"
#include "draw.h"
#include "sprite.h"
#include "vec.h"
#define CONIOEX
#include "conioex.h"
Sprite aim;
Sprite gradient;
Coord2 mouse_pos;

static float distance = 0;
static float speed_rate = 0;
static float speed_max = 2.0f;

struct SpotLight2D {

};

void scene_test_sprite_init() {
    sprite_load(&gradient, L"assets/testalign.txt");
    //sprite_load(&aim, L"assets/aim.txt");
    const wchar_t* test = {
        L"\u2580\u2588\u2588\u2588"
        L"\u2580\u2588\u2588\u2588"
        L"\u2588\u2588\u2588\u2588"
        L"\u2588\u2588\u2588\u2588"
    };
    //const wchar_t* test = { L"\u03c9 !" };
    sprite_init(&aim, 4, 4, test, create_color(255, 0, 0));
}
void scene_test_sprite_update(float delta_time) {
    mouse_pos.x = inport(PM_CURX) - 2;
    mouse_pos.y = inport(PM_CURY) - 2;
    if (inport(PK_UP)) {
        // distance += 0.2f * deltaTime;
        speed_rate += 2.0f * delta_time;
    }
    else {
        speed_rate -= 1.0f * delta_time;
    }
    speed_rate = clamp(0.0f, speed_rate, 1.0f);
    distance += speed_max * speed_rate;
}

void scene_test_sprite_draw() {
    draw_sprite(Coord2{ 10, 0 }, &gradient);
    // draw_sprite_color(Coord2{ 10, 0 }, &aim);
    // draw_sprite(Coord2{ 10, 0 }, &aim);
    return;
    int height = buffer_get_height();
    int width = buffer_get_width();

    //int height_horizon = height * 0.2;
    //for (int y = 0; y < height; ++y) {
    //    for (int x = 0; x < width; ++x) {
    //        if (y < height_horizon) {
    //            draw_pixel(Coord2{ x, y }, create_pixel(create_color(0, 0, 255)));
    //        }
    //        else {
    //            Color color;

    //            float perspective = (float)(y - height_horizon) / (height - height_horizon);
    //            Color grid_color = sinf(40.0f * powf(1.0f - perspective, 2) + distance * 0.1f) > 0.0f ?
    //                create_color(165, 165, 165) : create_color(100, 100, 100);

    //            bool grid_row = sinf(40.0f * powf(1.0f - perspective, 2) + distance * 0.1f) > 0.0f;

    //            //float step = perspective * 0.6 + 0.5;
    //            //float diff = x - height * 0.5;
    //            //diff = diff < 0 ? -diff + step : diff;
    //            //bool grid_col = ((int)(diff / step) % 2 == 0);

    //            color = grid_row ? create_color(165, 165, 165) : create_color(100, 100, 100); // grass

    //            draw_pixel(Coord2{ x, y }, create_pixel(color));
    //        }
    //    }
    //}

    //int height_horizon = height / 2;
    //for (int y = 0; y < height_horizon; ++y) {
    //    // Take a sample point for depth linearly related to rows down screen
    //    float fSampleDepth = (float)y / ((float)height / 2.0f);
    //    for (int x = 0; x < width; ++x) {

    //    }
    //    draw_fill(Coord2{ 0, y + height_horizon }, create_pixel(create_color(fSampleDepth * 255, fSampleDepth * 255, fSampleDepth * 255)), width, 1);
    //}
    //for (int y = 0; y < height; ++y) {
    //    for (int x = 0; x < width; ++x) {
    //        Coord2 curr_pos = Coord2{ x, y };
    //        Pixel* p = buffer_get_pixel_at(curr_pos);
    //        int dist = coord2_distance_squared(&mouse_pos, &curr_pos);
    //        if (dist > 400) {
    //            p->color_fore = create_color(0, 0, 0);
    //        }
    //        if (dist > 255) {
    //            p->color_fore = create_color(
    //                p->color_fore.r / 4,
    //                p->color_fore.g / 4,
    //                p->color_fore.b / 4
    //            );
    //        }
    //        if (dist > 164) {
    //            p->color_fore = create_color(
    //                p->color_fore.r / 3,
    //                p->color_fore.g / 3,
    //                p->color_fore.b / 3
    //            );
    //        }
    //        else if (dist > 100) {
    //            p->color_fore = create_color(
    //                p->color_fore.r / 2,
    //                p->color_fore.g / 2,
    //                p->color_fore.b / 2
    //            );
    //        }
    //        else if (dist > 36) {
    //            p->color_fore = create_color(
    //                p->color_fore.r / 1.5,
    //                p->color_fore.g / 1.5,
    //                p->color_fore.b / 1.5
    //            );
    //        }
    //    }
    //}    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Coord2 curr_pos = Coord2{ x, y };
            Pixel* p = buffer_get_pixel_at(curr_pos);
            int dist = coord2_distance_squared(&mouse_pos, &curr_pos);
            if (dist > 400) {
                p->color_fore = create_color(0, 0, 0);
            }
            if (dist > 255) {
                p->color_fore = create_color(
                    p->color_fore.r / 4,
                    p->color_fore.g / 4,
                    p->color_fore.b / 4
                );
            }
            if (dist > 164) {
                p->color_fore = create_color(
                    p->color_fore.r / 3,
                    p->color_fore.g / 3,
                    p->color_fore.b / 3
                );
            }
            else if (dist > 100) {
                p->color_fore = create_color(
                    p->color_fore.r / 2,
                    p->color_fore.g / 2,
                    p->color_fore.b / 2
                );
            }
            else if (dist > 36) {
                p->color_fore = create_color(
                    p->color_fore.r / 1.5,
                    p->color_fore.g / 1.5,
                    p->color_fore.b / 1.5
                );
            }
        }
    }

    //}
    // draw_sprite_color(mouse_pos, &aim);
}