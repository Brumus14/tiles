#ifndef TILESET_H
#define TILESET_H

#include "bgfx/c99/bgfx.h"

typedef struct tileset {
    bgfx_texture_handle_t texture;
    int width;
    int height;
    int tile_width;
    int tile_height;
} tileset;

typedef struct tile_rect {
    float x;
    float y;
    float width;
    float height;
} tile_rect;

void tileset_init(tileset *tileset, int tile_width, int tile_height,
                  char *texture_path);
tile_rect tileset_get_tile_rect(tileset *tileset, int tile_id);

#endif
