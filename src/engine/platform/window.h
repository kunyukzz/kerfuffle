#ifndef WINDOW_H
#define WINDOW_H

#include "engine/core/define.h" // IWYU pragma: keep

#include <GLFW/glfw3.h>

typedef struct {
    GLFWwindow *handle;
    int width, height;
    const char *title;
} window_system_t;

window_system_t *window_sys_init(int width, int height, const char *title);

void window_sys_kill(window_system_t *ws);

b8 window_sys_close(window_system_t *ws);

void window_sys_poll(window_system_t *ws);

void window_sys_swapbuffer(window_system_t *ws);

void window_sys_get_size(int width, int height);

GLFWwindow *window_sys_get_handle(void);

#endif // WINDOW_H
