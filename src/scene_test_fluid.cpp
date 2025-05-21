#include "scene_test_fluid.h"
#include "draw.h"
#include "sprite.h"
#include "vec.h"
#define CONIOEX
#include "conioex.h"

const static int box_width = 50;
const static int box_height = 25;
const static int max_particles = box_width * box_height * 4;
static int box_offset_x = 0;
static int box_offset_y = 0;
//static int box_inner_width = box_width - 2;
//static int box_inner_height = box_height - 2;
wchar_t fluid_grid;

float gravity_t = 1;

struct Particle {
    Vec2 pos;
    Vec2 acc;
    Vec2 vel;
    float density;
    bool is_wall;
};

static Particle particles[max_particles];
static int particles_cnt = 0;

void scene_test_fluid_init() {
    int buffer_width = buffer_get_width();
    int buffer_height = buffer_get_height();
    
    box_offset_x = (buffer_width - box_width) / 2;
    box_offset_y = (buffer_height - box_height) / 2;

    // init particles;
    const wchar_t particles_init[box_width * box_height + 1] = {
        L"##################################################"
        L"##################################################"
        L"##                                              ##"
        L"##                                              ##"
        L"##                                              ##"
        L"##                                              ##"
        L"##                                              ##"
        L"##                                              ##"
        L"##                                              ##"
        L"##        aaaaaaaaaaaaaaaaaaaaaaaaaaaaa         ##"
        L"##        aaaaaaaaaaaaaaaaaaaaaaaaaaaaa         ##"
        L"##        aaaaaaaaaaaaaaaaaaaaaaaaaaaaa         ##"
        L"##        aaaaaaaaaaaaaaaaaaaaaaaaaaaaa         ##"
        L"##        aaaaaaaaaaaaaaaaaaaaaaaaaaaaa         ##"
        L"##                                              ##"
        L"##                                              ##"
        L"##                                              ##"
        L"##                                              ##"
        L"##                                              ##"
        L"##                                              ##"
        L"##                                              ##"
        L"##                                              ##"
        L"##                                              ##"
        L"##################################################"
        L"##################################################"
    };
    memset(particles, 0, sizeof(Particle) * max_particles);
    for (int y = 0; y < box_height; ++y) {
        for (int x = 0; x < box_width; ++x) {
            int i = y * box_width + x;
            if (particles_init[i] == L' ') {
                continue;
            }
            // int i_particle = i * 2;

            bool is_wall = particles_init[i] == L'#';
            float x_particle = -(float)x;
            float y_particle = (float)y * 2;
            // top
            particles[particles_cnt].pos = Vec2{ x_particle, y_particle };
            particles[particles_cnt].is_wall = is_wall;
            ++particles_cnt;
            // bottom
            particles[particles_cnt].pos = Vec2{ x_particle, y_particle + 1 };
            particles[particles_cnt].is_wall = is_wall;
            ++particles_cnt;
        }
    }

}
void scene_test_fluid_update(float delta_time) {
    const float gravity = 1;
    const float pressure = 2;
    const float viscosity = 4;
    const float mass = 1;
    const float radius = 2.0f;
    const float r0 = 1.5f;

    gravity_t -= 0.002;

    // simulate
    // calculate density
    for (int i = 0; i < particles_cnt; ++i) {
        Particle* p = particles + i;
        p->density = p->is_wall ? 100 : 0;
        for (int j = 0; j < particles_cnt; ++j) {
            Particle* q = particles + j;
            float dist = vec2_dist(particles[i].pos, particles[j].pos);
            if (dist < radius) {
                particles[i].density += mass * (dist / radius - 1) * (dist / radius - 1);
            }
        }
        
    }

    // calculate interaction
    for (int i = 0; i < particles_cnt; ++i) {
        particles[i].acc.x = 0;
        particles[i].acc.y = gravity_t;
        for (int j = 0; j < particles_cnt; ++j) {
            //if (j == i) {
            //    continue;
            //}
            Vec2 diff = vec2_sub(particles[i].pos, particles[j].pos);
            // diff.x = -diff.x;
            float dist = vec2_dist(particles[i].pos, particles[j].pos);
            if (dist < radius) {
                int x = -(int)particles[i].pos.x;
                int y = (int)particles[i].pos.y / 2;
                float viscosity_dynamic = viscosity; // (x <= 10 && y >= 15) ? viscosity * 2 : viscosity;

                Vec2 press = vec2_mult(diff, (particles[i].density + particles[j].density - 2 * r0) * pressure);
                Vec2 visc = vec2_mult(vec2_sub(particles[i].vel, particles[j].vel), viscosity_dynamic);// viscosity);
                particles[i].acc = vec2_add(particles[i].acc,
                    vec2_mult(
                        vec2_sub(press, visc),
                        (1 - dist / radius) / particles[i].density
                    ));

            }

        }
    }
}
void scene_test_fluid_draw() {
    // draw fluid
    wchar_t fluid_buffer[box_height][box_width];
    wchar_t fluid_buffer_2[box_height][box_width];
    memset(fluid_buffer, 0, sizeof(wchar_t) * box_width * box_height); // clear buffer
    for (int i = 0; i < particles_cnt; ++i) {
        int x = -(int)particles[i].pos.x;
        int y = (int)particles[i].pos.y / 2;

        if (!particles[i].is_wall) {
            particles[i].vel = vec2_add(particles[i].vel, vec2_mult(particles[i].acc, 0.01f * !particles[i].is_wall));
            particles[i].pos = vec2_add(particles[i].pos, particles[i].vel);
        }
        //if (x >= 0 && x < box_width && y >= 0 && y < box_height) {
        //    if (particles[i].is_wall || fluid_buffer[y][x] == L'\u2588') {
        //        fluid_buffer[y][x] = L'\u2588';
        //    }
        //    else {
        //        fluid_buffer[y][x] = L'*';
        //    }
        //}
        if (x >= 0 && x < box_width && y >= 0 && y < box_height) {
            int i_buffer = y * box_width + x;
            //fluid_buffer[i_buffer] |= 8;
            //fluid_buffer[i_buffer + 1] |= 4;
            fluid_buffer[y][x] |= 8;
            if (x < box_width - 1) {
                fluid_buffer[y][x + 1] |= 4;
            }
            if (y < box_height - 1) {
                fluid_buffer[y + 1][x] |= 2;
                if (x < box_width - 1) {
                    fluid_buffer[y + 1][x + 1] |= 1;
                }
            }
        }
    }
    const wchar_t marching_cube[] = L" '`-.|//,\\|\\_\\/#\n";
    for (int y = 1; y < box_height - 1; ++y) {
        for (int x = 1;x < box_width - 1; ++x) {
            if (fluid_buffer[y][x - 1] && fluid_buffer[y][x + 1]
                && fluid_buffer[y - 1][x] && fluid_buffer[y + 1][x]) {
                fluid_buffer_2[y][x] = 0;
            }
            else {
                fluid_buffer_2[y][x] = fluid_buffer[y][x];
            }
        }
    }
    for (int y = 1; y < box_height - 1; ++y) {
        for (int x = 1; x < box_width - 1; ++x) {
            fluid_buffer[y][x] = fluid_buffer_2[y][x];
        }
    }
    for (int i = 2; i < box_height - 1; ++i) {
        for (int j = 2; j < box_width - 1; ++j) {
                Coord2 pos{j + box_offset_x - 1, i + box_offset_y - 1};
                draw_pixel(pos, create_pixel(
                    marching_cube[fluid_buffer[i][j]],
                    // fluid_buffer[i][j],
                    create_color(255, 255, 255),
                    create_color(0, 0, 0))
                );
        }
    }
    
    // draw box border
    for (int j = 0; j < box_width; ++j) {
        draw_pixel(Coord2{ j + box_offset_x, box_offset_y },
            create_pixel(create_color(255, 255, 255)));
        draw_pixel(Coord2{ j + box_offset_x, box_height - 1 + box_offset_y },
            create_pixel(create_color(255, 255, 255)));
    }
    for (int i = 0; i < box_height; ++i) {
        draw_pixel(Coord2{  box_offset_x, i + box_offset_y },
            create_pixel(create_color(255, 255, 255)));
        draw_pixel(Coord2{ box_width - 1 + box_offset_x, i + box_offset_y },
            create_pixel(create_color(255, 255, 255)));
    }
}