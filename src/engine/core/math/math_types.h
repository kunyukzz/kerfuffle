#ifndef MATH_TYPES_H
#define MATH_TYPES_H

#include "engine/core/define.h" // IWYU pragma: keep

#if defined(__SSE__) || (_M_IX86_FP >= 1) || (_M_X64 == 1)
#    define MATH_SSE 1
#    include <xmmintrin.h> // SSE
#    include <emmintrin.h> // SSE2
#else
#    define MATH_SSE 0
#endif

// Vector types
typedef union VEC2
{
    struct {
        f32 x, y;
    };
    struct {
        f32 u, v;
    };
    struct {
        f32 r, g;
    };
    f32 elements[2];
} ALIGN(8) vec2;

typedef union VEC3
{
    struct {
        f32 x, y, z;
    };
    struct {
        f32 r, g, b;
    };
    struct {
        f32 u, v, t;
    };
    f32 elements[3];
} ALIGN(16) vec3;

typedef union VEC4
{
    struct {
        f32 x, y, z, w;
    };
    struct {
        f32 r, g, b, a;
    };
    struct {
        f32 u, v, t, s;
    };
    f32 elements[4];
} ALIGN(16) vec4;

typedef union MAT4
{
    f32 data[16];
    vec4 columns[4];

    struct {
        f32 m0, m1, m2, m3;
        f32 m4, m5, m6, m7;
        f32 m8, m9, m10, m11;
        f32 m12, m13, m14, m15;
    };
} ALIGN(16) mat4;

typedef union QUAT
{
    struct {
        f32 x, y, z, w;
    };

    vec4 as_vec4;
    f32 elements[4];
} ALIGN(16) quat;

typedef struct vertex {
    vec3 position;
    vec3 normal;
    vec2 texcoord;
} ALIGN(16) vertex;

#endif // MATH_TYPES_H
