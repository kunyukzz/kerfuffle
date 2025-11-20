#ifndef INPUT_H
#define INPUT_H

#include "engine/core/define.h" // IWYU pragma: keep
#include <GLFW/glfw3.h>

#define MAX_KEYS (GLFW_KEY_LAST + 1)
#define MAX_MOUSE_BUTTONS GLFW_MOUSE_BUTTON_LAST

typedef struct {
    b8 keys_curr[MAX_KEYS];
    b8 keys_prev[MAX_KEYS];
    int modifier;

    b8 mouse_curr[MAX_MOUSE_BUTTONS];
    b8 mouse_prev[MAX_MOUSE_BUTTONS];
    int mouse_x, mouse_y;
    f32 mouse_wheel;

    b8 is_inside_window;
} input_system_t;

input_system_t *input_sys_init(void);

void input_sys_kill(input_system_t *ins);

void input_sys_update(input_system_t *ins, f64 delta);

// query keys
b8 key_held_press(int key);

b8 key_once_pressed(int key);

b8 key_once_released(int key);

b8 key_mod_active(int mod);

// query mouse button
b8 mouse_held_press(int button);

b8 mouse_once_pressed(int button);

b8 mouse_once_released(int button);

void mouse_pos(int *x, int *y);

float mouse_wheel(void);

b8 mouse_inside_window(void);

#endif // INPUT_H
