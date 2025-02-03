#define GLFW_EXPOSE_NATIVE_WAYLAND

#include <stdio.h>
#include <string.h>
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include "bgfx/c99/bgfx.h"
#include "cglm/cglm.h"
#include "util/renderer.h"

typedef struct vertex {
    float x;
    float y;
    float z;
    float uv_x;
    float uv_y;
    uint32_t abgr;
} vertex;

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

    bgfx_set_view_clear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff, 1,
                        0);

    bgfx_vertex_layout_t layout;
    bgfx_vertex_layout_begin(&layout, BGFX_RENDERER_TYPE_OPENGL);
    bgfx_vertex_layout_add(&layout, BGFX_ATTRIB_POSITION, 3,
                           BGFX_ATTRIB_TYPE_FLOAT, false, false);
    bgfx_vertex_layout_add(&layout, BGFX_ATTRIB_TEXCOORD0, 2,
                           BGFX_ATTRIB_TYPE_FLOAT, false, false);
    bgfx_vertex_layout_add(&layout, BGFX_ATTRIB_COLOR0, 4,
                           BGFX_ATTRIB_TYPE_UINT8, true, false);
    bgfx_vertex_layout_end(&layout);

    vertex vertices[] = {
        {0.0f, 1.0f, 0.0f, 0.0, 1.0, 0xff0000ff},
        {1.0f, 1.0f, 0.0f, 1.0, 1.0, 0xff0000ff},
        {1.0f, 0.0f, 0.0f, 1.0, 0.0, 0xff00ff00},
        {0.0f, 0.0f, 0.0f, 0.0, 0.0, 0xff00ff00},
    };

    uint16_t indices[] = {0, 1, 2, 0, 2, 3};

    bgfx_vertex_buffer_handle_t vertex_buffer = bgfx_create_vertex_buffer(
        bgfx_make_ref(vertices, sizeof(vertices)), &layout, 0);

    bgfx_index_buffer_handle_t index_buffer =
        bgfx_create_index_buffer(bgfx_make_ref(indices, sizeof(indices)), 0);

    bgfx_shader_handle_t vertex_shader =
        util_renderer_load_shader("build/shaders/tilemap/vs.bin");
    bgfx_shader_handle_t fragment_shader =
        util_renderer_load_shader("build/shaders/tilemap/fs.bin");
    bgfx_program_handle_t program =
        bgfx_create_program(vertex_shader, fragment_shader, true);

    bgfx_texture_handle_t texture =
        util_renderer_load_texture("build/textures/Grass.dds");
    bgfx_uniform_handle_t texture_uniform =
        bgfx_create_uniform("s_texture", BGFX_UNIFORM_TYPE_SAMPLER, 0);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        int width;
        int height;

        glfwGetWindowSize(window, &width, &height);

        printf("%d, %d\n", width, height);

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

        bgfx_set_vertex_buffer(0, vertex_buffer, 0, 4);
        bgfx_set_index_buffer(index_buffer, 0, 6);

        bgfx_set_texture(0, texture_uniform, texture, BGFX_SAMPLER_POINT);

        bgfx_set_state(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                           BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |
                           BGFX_STATE_CULL_CW,
                       0);

        bgfx_submit(0, program, 0, 0);
        bgfx_frame(false);
    }

    bgfx_shutdown();

    glfwTerminate();

    return 0;
}
