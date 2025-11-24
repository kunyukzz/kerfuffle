#include "application.h"
#include "engine/core/memory/memory.h"
#include "engine/core/math/maths.h"

b8 application_init(application_t *app)
{
    u64 estimated_memory = 1 * 1024 * 1024;
    if (!memory_sys_init(estimated_memory))
    {
        LOG_ERROR("Failed to init memory system with estimated size: %lu",
                  estimated_memory);
        return false;
    }

    arena_create(128 * 1024, &app->arena, NULL);

    app->fs = file_system_init(&app->arena);
    app->ws = window_sys_init(&app->arena, 1280, 720, "Kerfuffle");
    app->ip = input_sys_init(&app->arena);
    app->cs = camera_sys_init(&app->arena);
    app->rs = render_sys_init(&app->arena);
    app->sh = shader_sys_init(&app->arena);
    app->game = game_init();

    // TODO: temp
    shader_sys_set(app->sh, "shaders/test");
    // shader_sys_bind(app->sh);

#if DEBUG
    LOG_DEBUG("--- Memory Addresses ---");
    LOG_DEBUG("Filesystem: %p", app->fs);
    LOG_DEBUG("Window:     %p", app->ws);
    LOG_DEBUG("Input:      %p", app->ip);
    LOG_DEBUG("Camera:     %p", app->cs);
    LOG_DEBUG("Render:     %p", app->rs);
    LOG_DEBUG("Shader:     %p", app->sh);
    // LOG_DEBUG("Mesh:       %p", app->mesh);

    u64 used = arena_used(&app->arena);
    u64 total = app->arena.total_size;
    f32 usage_percent = (f32)used / (f32)total * 100.0f;
    LOG_TRACE("Arena Usage: %lu/%lu bytes (%.1f%%)", used, total,
              usage_percent);

    if (usage_percent > 90.0f)
    {
        LOG_WARN("Arena nearly full! %.1f%% used", usage_percent);
    }
#endif

    LOG_INFO("Engine Initialize");
    return true;
}

b8 application_run(application_t *app)
{
    const f64 TARGET_FPS = 60.0;
    const f64 TARGET_FRAME_TIME = 1.0 / TARGET_FPS;

    // math_run_all_tests();
    // test_simd_vs_scalar();

    b8 cap_fps = false;
    b8 benchmark_mode = false;
    u32 benchmark_frames = 0;
    const u32 MAX_BENCHMARK_FRAMES = 1000;

    f64 fps_timer = 0.0;
    u32 fps_counter = 0;
    f64 math_total_time = 0.0;

    timer_start(&app->time);
    f64 prev = timer_get();

    LOG_INFO("%s", mem_debug_stat());

    while (!window_sys_close(app->ws))
    {
        f64 curr = timer_get();
        f64 delta = curr - prev;
        prev = curr;

        /*
        if (benchmark_mode && benchmark_frames < MAX_BENCHMARK_FRAMES)
        {
            f64 math_start = timer_get();
            math_benchmark bench;
            benchmark_simd_vs_scalar(&bench);

            f64 math_end = timer_get();
            math_total_time += (math_end - math_start);

            benchmark_frames++;
        }
        */

        fps_timer += delta;
        fps_counter++;

        if (fps_timer >= 1.0)
        {
            f64 avg_delta = fps_timer / fps_counter;
            f64 ms = avg_delta * 1000.0;
            f64 fps = fps_counter / fps_timer;

            LOG_INFO("FPS: %.0f | Frame: %.2f ms", fps, ms);

            /*
            if (benchmark_mode && benchmark_frames >= MAX_BENCHMARK_FRAMES)
            {
                f64 avg_math_time =
                    (math_total_time / benchmark_frames) * 1000.0;
                LOG_INFO("Math ops: %.4f ms/frame (avg over %u frames)",
                         avg_math_time, benchmark_frames);
                benchmark_mode = false;
            }
            */

            fps_counter = 0;
            fps_timer -= 1.0;
        }

        window_sys_poll(app->ws);
        input_sys_update(app->ip, delta);

        game_update(app->game, delta);
        game_render(app->game, delta);
        camera_update(app->cs);

        shader_sys_bind(app->sh);
        render_sys_begin(app->rs, WORLD_PASS);

        // TODO: temp code
        shader_sys_set_uniform_mat4(app->sh, mat4_identity());

        render_draw(app->rs);

        render_sys_end(app->rs, WORLD_PASS);

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

    game_kill(app->game);

    shader_sys_kill(app->sh);
    render_sys_kill(app->rs);
    camera_sys_kill(app->cs);
    input_sys_kill(app->ip);
    window_sys_kill(app->ws);
    file_system_kill(app->fs);

    arena_kill(&app->arena);
    memory_sys_kill();

    LOG_INFO("Engine Shutdown");
    return true;
}
