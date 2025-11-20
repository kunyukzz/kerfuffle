#ifndef APPLICATION_H
#define APPLICATION_H

#include "define.h" // IWYU pragma: keep

#include "engine/core/clock.h"
#include "engine/platform/window.h"
#include "engine/platform/input.h"

typedef struct {
    clock_timer_t time;

    window_system_t *ws;
    input_system_t *ip;
} application_t;

b8 application_init(application_t *app);
b8 application_run(application_t *app);

#endif // APPLICATION_H
