#include "tileset.h"

#include "util/renderer.h"
#include <stdio.h>

void tileset_init(tileset *tileset, int tile_width, int tile_height,
                  char *texture_path) {
    tileset->tile_width = tile_width;
    tileset->tile_height = tile_height;

    bgfx_texture_info_t texture_info;
    tileset->texture = util_renderer_load_texture(texture_path, &texture_info);

    tileset->width = texture_info.width / tile_width;
    tileset->height = texture_info.height / tile_height;
}

tile_rect tileset_get_tile_rect(tileset *tileset, int tile_id) {
    tile_rect rect;

    int x = tile_id % tileset->width;
    int y = tile_id / tileset->width;

    // Initialise rectangle with pixel sizes
    rect.x = x * tileset->tile_width;
    rect.y = y * tileset->tile_height;
    rect.width = tileset->tile_width;
    rect.height = tileset->tile_height;

    // Scale rectangle to texture coordinates
    rect.x /= tileset->tile_width * tileset->width;
    rect.width /= tileset->tile_width * tileset->width;

    rect.y /= tileset->tile_height * tileset->height;
    rect.height /= tileset->tile_height * tileset->height;

    return rect;
}
