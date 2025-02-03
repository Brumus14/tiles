#ifndef UTIL_RENDERER_H
#define UTIL_RENDERER_H

#include "bgfx/c99/bgfx.h"

bgfx_shader_handle_t util_renderer_load_shader(char *file_path);
bgfx_texture_handle_t util_renderer_load_texture(char *file_path);

#endif
