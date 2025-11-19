#ifndef APPLICATION_H
#define APPLICATION_H

#include "define.h" // IWYU pragma: keep

#include "engine/platform/window.h"

typedef struct {
    window_system_t *ws;
} application_t;

b8 application_init(application_t *app);
b8 application_run(application_t *app);

#endif // APPLICATION_H
