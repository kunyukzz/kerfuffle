#ifndef CLOCK_TIMER_H
#define CLOCK_TIMER_H

#include "define.h" // IWYU pragma: keep

#if PLATFORM_LINUX
#    include <time.h>
#elif PLATFORM_WINDOWS
#    include <Windows.h>
#endif

#include <GLFW/glfw3.h>

#include <string.h>

typedef struct {
    f64 start_time;
    f64 elapsed;
} clock_timer_t;

INL void timer_start(clock_timer_t *timer)
{
    timer->start_time = glfwGetTime();
    timer->elapsed = 0.0;
}

INL void timer_update(clock_timer_t *timer)
{
    timer->elapsed = glfwGetTime() - timer->start_time;
}

INL void timer_stop(clock_timer_t *timer)
{
    timer->start_time = 0.0;
    timer->elapsed = 0.0;
}

INL f64 timer_get() { return glfwGetTime(); }

// frame limiting
INL void get_sleep(f64 wake)
{
#if PLATFORM_LINUX
    struct timespec ts;
    ts.tv_sec = (time_t)wake;
    ts.tv_nsec = (long)((wake - (f64)ts.tv_sec) * 1e9);
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, NULL);
#elif PLATFORM_WINDOWS
    f64 now = glfwGetTime();
    f64 sleep_sec = wake - now;
    if (sleep_sec <= 0.0) return;
    Sleep((DWORD)(sleep_sec * 1000.0));
#endif
}

#endif // CLOCK_TIMER_H
