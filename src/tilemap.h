#ifndef TILEMAP_H
#define TILEMAP_H

#include "bgfx/c99/bgfx.h"
#include "tileset.h"
#include "vertex_layout.h"

typedef struct tilemap {
    bgfx_dynamic_vertex_buffer_handle_t vertex_buffer;
    int vertex_count;
    bgfx_index_buffer_handle_t index_buffer;
    int index_count;
    bgfx_program_handle_t program;
    tileset tileset;
    int *tiles;
    int width;
    int height;
} tilemap;

void tilemap_init(tilemap *tilemap, tileset tileset, int width, int height);
int tilemap_get_tile(tilemap *tilemap, int x, int y);
void tilemap_set_tile(tilemap *tilemap, int tile_id, int x, int y);
void tilemap_draw(tilemap *tilemap);

#endif
