#ifndef VERTEX_LAYOUT_H
#define VERTEX_LAYOUT_H

#include "bgfx/c99/bgfx.h"

typedef struct pos_tex_vertex {
    float x;
    float y;
    float z;
    float uv_x;
    float uv_y;
} pos_tex_vertex;

extern bgfx_vertex_layout_t pos_tex_vertex_layout;

void vertex_layout_init();

#endif
