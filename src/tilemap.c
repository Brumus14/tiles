#include "tilemap.h"

#include "vertex_layout.h"
#include "util/renderer.h"
#include <string.h>
#include <stdio.h>

void tilemap_init(tilemap *tilemap, tileset tileset, int width, int height) {
    tilemap->tileset = tileset;
    tilemap->width = width;
    tilemap->height = height;

    tilemap->tiles = malloc(sizeof(int) * width * height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            tilemap->tiles[y * width + x] = 9;

            if (y == 0) {
                tilemap->tiles[y * width + x] = 1;
            } else if (y == height - 1) {
                tilemap->tiles[y * width + x] = 17;
            }

            if (x == 0) {
                tilemap->tiles[y * width + x] = 8;
            } else if (x == width - 1) {
                tilemap->tiles[y * width + x] = 10;
            }

            if (y == 0) {
                if (x == 0) {
                    tilemap->tiles[y * width + x] = 0;
                } else if (x == width - 1) {
                    tilemap->tiles[y * width + x] = 2;
                }
            } else if (y == height - 1) {
                if (x == 0) {
                    tilemap->tiles[y * width + x] = 16;
                } else if (x == width - 1) {
                    tilemap->tiles[y * width + x] = 18;
                }
            }

            if (x == 3 || x == 4) {
                tilemap->tiles[y * width + x] = 3;
            }
        }
    }

    tilemap->vertex_count = 4 * width * height;
    tilemap->index_count = 6 * width * height;

    const bgfx_memory_t *vertex_memory =
        bgfx_alloc(sizeof(pos_tex_vertex) * tilemap->vertex_count);
    pos_tex_vertex *vertices = (pos_tex_vertex *)vertex_memory->data;

    const bgfx_memory_t *index_memory =
        bgfx_alloc(sizeof(uint16_t) * tilemap->index_count);
    uint16_t *indices = (uint16_t *)index_memory->data;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int tile_index = y * width + x;
            int vertex_index = 4 * tile_index;
            int index_index = 6 * tile_index;

            tile_rect rect = tileset_get_tile_rect(&tilemap->tileset,
                                                   tilemap->tiles[tile_index]);

            // use memcpy instead and add positions to template of vertices
            vertices[vertex_index] =
                (pos_tex_vertex){x, y + 1, 0.0, rect.x, rect.y + rect.height};

            vertices[vertex_index + 1] = (pos_tex_vertex){
                x + 1, y + 1, 0.0, rect.x + rect.width, rect.y + rect.height};

            vertices[vertex_index + 2] =
                (pos_tex_vertex){x + 1, y, 0.0, rect.x + rect.width, rect.y};

            vertices[vertex_index + 3] =
                (pos_tex_vertex){x, y, 0.0f, rect.x, rect.y};

            indices[index_index] = vertex_index;
            indices[index_index + 1] = 1 + vertex_index;
            indices[index_index + 2] = 2 + vertex_index;
            indices[index_index + 3] = vertex_index;
            indices[index_index + 4] = 2 + vertex_index;
            indices[index_index + 5] = 3 + vertex_index;
        }
    }

    tilemap->vertex_buffer = bgfx_create_dynamic_vertex_buffer_mem(
        vertex_memory, &pos_tex_vertex_layout, 0);
    tilemap->index_buffer = bgfx_create_index_buffer(index_memory, 0);

    bgfx_shader_handle_t vertex_shader =
        util_renderer_load_shader("build/shaders/tilemap/vs.bin");

    bgfx_shader_handle_t fragment_shader =
        util_renderer_load_shader("build/shaders/tilemap/fs.bin");

    tilemap->program =
        bgfx_create_program(vertex_shader, fragment_shader, true);
}

int tilemap_get_tile(tilemap *tilemap, int x, int y) {
    return tilemap->tiles[y * tilemap->width + x];
}

void tilemap_set_tile(tilemap *tilemap, int tile_id, int x, int y) {
    tilemap->tiles[y * tilemap->width + x] = tile_id;
}

void tilemap_draw(tilemap *tilemap) {
    bgfx_set_dynamic_vertex_buffer(0, tilemap->vertex_buffer, 0,
                                   tilemap->vertex_count);
    bgfx_set_index_buffer(tilemap->index_buffer, 0, tilemap->index_count);

    bgfx_submit(0, tilemap->program, 0, 0);
}
