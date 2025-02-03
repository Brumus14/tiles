#include "renderer.h"

#include <stdio.h>

// add validation and checks
bgfx_shader_handle_t util_renderer_load_shader(char *file_path) {
    FILE *file = fopen(file_path, "rb");

    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    const bgfx_memory_t *memory = bgfx_alloc(file_size + 1);
    fread(memory->data, 1, file_size, file);
    memory->data[memory->size - 1] = '\0';

    fclose(file);

    return bgfx_create_shader(memory);
}

bgfx_texture_handle_t util_renderer_load_texture(char *file_path) {
    FILE *file = fopen(file_path, "rb");

    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    const bgfx_memory_t *memory = bgfx_alloc(file_size + 1);
    fread(memory->data, 1, file_size, file);
    memory->data[memory->size - 1] = '\0';

    fclose(file);

    return bgfx_create_texture(memory, BGFX_TEXTURE_NONE, true, NULL);
}
