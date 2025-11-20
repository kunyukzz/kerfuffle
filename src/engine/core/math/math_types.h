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

#ifdef __SSE__
    __m128 simd;
#endif
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

#ifdef __SSE__
    __m128 simd;
#endif
} ALIGN(32) vec4;

// Matrix types (use Colum-major)
typedef union MAT4
{
    // data[col * 4 + row]
    f32 data[16];

    // Column vectors
    vec4 columns[4];

    // Element access: m[col][row]
    struct {
        f32 m00, m10, m20, m30;
        f32 m01, m11, m21, m31;
        f32 m02, m12, m22, m32;
        f32 m03, m13, m23, m33;
    };

#ifdef __SSE__
    __m128 simd[4];
#endif
} ALIGN(32) mat4;

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
