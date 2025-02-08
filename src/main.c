#define GLFW_EXPOSE_NATIVE_WAYLAND

#include <stdio.h>
#include <string.h>
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include "bgfx/c99/bgfx.h"
#include "cglm/cglm.h"
#include "util/renderer.h"
#include "vertex_layout.h"
#include "tileset.h"
#include "tilemap.h"

int main() {
    GLFWwindow *window;

    glfwInit();

    window = glfwCreateWindow(640, 480, "Testing", NULL, NULL);

    glfwMakeContextCurrent(window);

    bgfx_init_t init;
    bgfx_init_ctor(&init);

    init.type = BGFX_RENDERER_TYPE_OPENGL;

    init.platformData.ndt = glfwGetWaylandDisplay();
    init.platformData.nwh = (void *)glfwGetWaylandWindow(window);
    init.platformData.type = BGFX_NATIVE_WINDOW_HANDLE_TYPE_WAYLAND;

    bgfx_init(&init);
    bgfx_reset(640, 480, BGFX_RESET_VSYNC, init.resolution.format);
    // bgfx_set_debug(BGFX_DEBUG_TEXT | BGFX_DEBUG_STATS);

    bgfx_set_view_clear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff, 1,
                        0);

    vertex_layout_init();

    bgfx_texture_handle_t texture =
        util_renderer_load_texture("build/textures/tilemap.dds", NULL);
    bgfx_uniform_handle_t texture_uniform =
        bgfx_create_uniform("s_texture", BGFX_UNIFORM_TYPE_SAMPLER, 0);

    tileset tileset;
    tileset_init(&tileset, 16, 16, "build/textures/tilemap.dds");

    tilemap tilemap;
    tilemap_init(&tilemap, tileset, 25, 15);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        int width;
        int height;

        glfwGetWindowSize(window, &width, &height);

        bgfx_reset(width, height, BGFX_RESET_VSYNC, init.resolution.format);
        bgfx_set_view_rect(0, 0, 0, width, height);

        float view_matrix[4][4];
        mat4 glm_view_matrix = GLM_MAT4_ZERO;
        glm_look((vec3){0, 0, 2}, (vec3){0, 0, -1}, (vec3){0, 1, 0},
                 glm_view_matrix);

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                view_matrix[i][j] = glm_view_matrix[i][j];
            }
        }

        float projection_matrix[4][4];
        mat4 glm_projection_matrix = GLM_MAT4_ZERO;

        float zoom = 64;

        glm_ortho(0.0f, (float)width / zoom, (float)height / zoom, 0.0f, 0.0f,
                  100.0f, glm_projection_matrix);

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                projection_matrix[i][j] = glm_projection_matrix[i][j];
            }
        }

        bgfx_set_view_transform(0, view_matrix, projection_matrix);

        bgfx_touch(0);

        bgfx_set_texture(0, texture_uniform, texture, BGFX_SAMPLER_POINT);

        bgfx_set_state(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                           BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |
                           BGFX_STATE_CULL_CW,
                       0);

        tilemap_draw(&tilemap);

        bgfx_frame(false);
    }

    bgfx_shutdown();

    glfwTerminate();

    return 0;
}
