#include "application.h"

b8 application_init(application_t *app)
{
    app->ws = window_sys_init(1280, 720, "Kerfuffle");

    LOG_INFO("Engine Initialize");
    return true;
}

b8 application_run(application_t *app)
{
    while (!window_sys_close(app->ws))
    {
        window_sys_poll(app->ws);

        LOG_INFO("Engine Run....");

        window_sys_swapbuffer(app->ws);
    }

    window_sys_kill(app->ws);

    LOG_INFO("Engine Shutdown");
    return true;
}
