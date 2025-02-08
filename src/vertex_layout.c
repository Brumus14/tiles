#include "vertex_layout.h"

bgfx_vertex_layout_t pos_tex_vertex_layout;

void vertex_layout_init() {
    bgfx_vertex_layout_begin(&pos_tex_vertex_layout, bgfx_get_renderer_type());
    bgfx_vertex_layout_add(&pos_tex_vertex_layout, BGFX_ATTRIB_POSITION, 3,
                           BGFX_ATTRIB_TYPE_FLOAT, false, false);
    bgfx_vertex_layout_add(&pos_tex_vertex_layout, BGFX_ATTRIB_TEXCOORD0, 2,
                           BGFX_ATTRIB_TYPE_FLOAT, false, false);
    bgfx_vertex_layout_end(&pos_tex_vertex_layout);
}
