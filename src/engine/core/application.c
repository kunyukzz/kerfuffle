#include "application.h"

b8 application_init(application_t *app)
{
    app->ws = window_sys_init(1280, 720, "Kerfuffle");
    app->ip = input_sys_init();

    LOG_INFO("Engine Initialize");
    return true;
}

b8 application_run(application_t *app)
{
    const f64 TARGET_FPS = 60.0;
    const f64 TARGET_FRAME_TIME = 1.0 / TARGET_FPS;

    b8 cap_fps = false;
    f64 fps_timer = 0.0f;
    u32 fps_counter = 0;

    timer_start(&app->time);
    f64 prev = timer_get();

    while (!window_sys_close(app->ws))
    {
        f64 curr = timer_get();
        f64 delta = curr - prev;
        prev = curr;

        fps_timer += delta;
        fps_counter++;

        if (fps_timer >= 1.0)
        {
            f64 avg_delta = fps_timer / fps_counter;
            f64 ms = avg_delta * 1000.0;
            f64 fps = fps_counter / fps_timer;

            LOG_INFO("FPS: %.0f | Frame: %.2f ms", fps, ms);

            fps_counter = 0;
            fps_timer -= 1.0;
        }

        window_sys_poll(app->ws);
        input_sys_update(app->ip, delta);

        window_sys_swapbuffer(app->ws);

        // frame limiting
        if (cap_fps)
        {
            f64 frame_end = timer_get();
            f64 frame_dur = frame_end - curr;

            if (frame_dur < TARGET_FRAME_TIME)
            {
                f64 target_wake = curr + TARGET_FRAME_TIME;
                get_sleep(target_wake);
            }
        }
    }

    input_sys_kill(app->ip);
    window_sys_kill(app->ws);

    LOG_INFO("Engine Shutdown");
    return true;
}
