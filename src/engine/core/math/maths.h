#ifndef MATHS_LIB_H
#define MATHS_LIB_H

#include "math_types.h"
#include <math.h>

#define M_PI 3.14159265358979323846f
#define M_SQRT_2 1.41421356237309504880f
#define M_SQRT_3 1.73205080756887729252f
#define M_SEC2MS_MULTI 1000.0f
#define M_MS2SEC_MULTI 0.001f
#define M_INFINITE 1e30f
#define M_EPSILON 1.192092896e-07f

#define M_PI2 (2.0f * M_PI)
#define M_HALF_PI (0.5f * M_PI)
#define M_QUARTER_PI (0.25f * M_PI)
#define M_DEG2RAD (M_PI / 180.0f)
#define M_RAD2DEG (180.0f / M_PI)

INL b8 mat4_has_nan(const mat4 *m)
{
    for (u32 i = 0; i < 16; i++)
    {
        if (isnan(m->data[i])) return true;
    }
    return false;
}

#if DEBUG
INL void mat4_print_col(const char *name, mat4 m)
{
    LOG_DEBUG("%s:", name);
    for (int row = 0; row < 4; ++row)
    {
        LOG_DEBUG("[%.2f %.2f %.2f %.2f]",
                  m.data[0 * 4 + row],  // column 0, row
                  m.data[1 * 4 + row],  // column 1, row
                  m.data[2 * 4 + row],  // column 2, row
                  m.data[3 * 4 + row]); // column 3, row
    }
}
#else
INL void mat4_print_col(const char *name, const mat4 *m)
{
    (void)name;
    (void)m;
}
#endif

INL f32 m_sin(f32 x) { return sinf(x); }
INL f32 m_cos(f32 x) { return cosf(x); }
INL f32 m_tan(f32 x) { return tanf(x); }
INL f32 m_asin(f32 x) { return asinf(x); }
INL f32 m_acos(f32 x) { return acosf(x); }
INL f32 m_atan2(f32 y, f32 x) { return atan2f(y, x); }
INL f32 m_sqrt(f32 x) { return sqrtf(x); }
INL f32 m_abs(f32 x) { return fabsf(x); }
INL f32 m_floor(f32 x) { return floorf(x); }
INL f32 m_ceil(f32 x) { return ceilf(x); }
INL f32 m_pow(f32 base, f32 exp) { return powf(base, exp); }
INL f32 m_exp(f32 x) { return expf(x); }
INL f32 m_log(f32 x) { return logf(x); }

i32 m_random(void);
i32 m_random_in_range(i32 min, i32 max);
f32 m_frandom(void);
f32 m_frandom_in_range(f32 min, f32 max);

INL b8 m_power_of_2(u64 value)
{
    return (value != 0) && ((value & (value - 1)) == 0);
}

INL f32 m_deg_to_rad(f32 degree) { return degree * M_DEG2RAD; }
INL f32 m_rad_to_deg(f32 rad) { return rad * M_RAD2DEG; }

/*************************
 * VECTOR 2
 *************************/
// Declaration (implement in maths.c)
f32 vec2_length(vec2 v);

// Constructor
INL vec2 vec2_create(f32 x, f32 y) { return (vec2){.x = x, .y = y}; }
INL vec2 vec2_zero(void) { return (vec2){.x = 0.0f, .y = 0.0f}; }
INL vec2 vec2_one(void) { return (vec2){.x = 1.0f, .y = 1.0f}; }
INL vec2 vec2_up(void) { return vec2_create(0.0f, 1.0f); }
INL vec2 vec2_down(void) { return vec2_create(0.0f, -1.0f); }
INL vec2 vec2_left(void) { return vec2_create(-1.0f, 0.0f); }
INL vec2 vec2_right(void) { return vec2_create(1.0f, 0.0f); }

// Arithmetic
INL vec2 vec2_add(vec2 a, vec2 b) { return vec2_create(a.x + b.x, a.y + b.y); }
INL vec2 vec2_sub(vec2 a, vec2 b) { return vec2_create(a.x - b.x, a.y - b.y); }
INL vec2 vec2_mul(vec2 a, vec2 b) { return vec2_create(a.x * b.x, a.y * b.y); }
INL vec2 vec2_div(vec2 a, vec2 b) { return vec2_create(a.x / b.x, a.y / b.y); }

// Scalar
INL vec2 vec2_scale(vec2 v, f32 s) { return vec2_create(v.x * s, v.y * s); }
INL vec2 vec2_scale_div(vec2 v, f32 s)
{
    return vec2_create(v.x / s, v.y / s);
}

// Operations
INL f32 vec2_length_sq(vec2 v) { return v.x * v.x + v.y * v.y; }
INL f32 vec2_dot(vec2 a, vec2 b) { return a.x * b.x + a.y * b.y; }
INL f32 vec2_cross(vec2 a, vec2 b) { return a.x * b.y - a.y * b.x; }

// Utility functions
INL f32 vec2_distance(vec2 a, vec2 b) { return vec2_length(vec2_sub(a, b)); }

INL f32 vec2_distance_sq(vec2 a, vec2 b)
{
    vec2 diff = vec2_sub(a, b);
    return vec2_length_sq(diff);
}

INL vec2 vec2_normalize(vec2 v)
{
    f32 len = vec2_length(v);
    if (len > M_EPSILON)
    {
        return vec2_scale(v, 1.0f / len);
    }
    return vec2_zero();
}

INL vec2 vec2_lerp(vec2 a, vec2 b, f32 t)
{
    return vec2_create(LERP(a.x, b.x, t), LERP(a.y, b.y, t));
}

INL b8 vec2_compare(vec2 a, vec2 b, f32 epsilon)
{
    return (ABS(a.x - b.x) <= epsilon) && (ABS(a.y - b.y) <= epsilon);
}

INL b8 vec2_is_zero(vec2 v) { return vec2_compare(v, vec2_zero(), M_EPSILON); }

/*************************
 * VECTOR 3
 *************************/
// Declaration (implement in maths.c)
f32 vec3_length(vec3 v);

// Constructor
INL vec3 vec3_create(f32 x, f32 y, f32 z)
{
    vec3 result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

INL vec3 vec3_zero(void) { return vec3_create(0.0f, 0.0f, 0.0f); }
INL vec3 vec3_one(void) { return vec3_create(1.0f, 1.0f, 1.0f); }
INL vec3 vec3_up(void) { return vec3_create(0.0f, 1.0f, 0.0f); }
INL vec3 vec3_down(void) { return vec3_create(0.0f, -1.0f, 0.0f); }
INL vec3 vec3_left(void) { return vec3_create(-1.0f, 0.0f, 0.0f); }
INL vec3 vec3_right(void) { return vec3_create(1.0f, 0.0f, 0.0f); }
INL vec3 vec3_forward(void) { return vec3_create(0.0f, 0.0f, -1.0f); }
INL vec3 vec3_back(void) { return vec3_create(0.0f, 0.0f, 1.0f); }

// Arithmetic
INL vec3 vec3_add(vec3 v1, vec3 v2)
{
    return vec3_create(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

INL vec3 vec3_sub(vec3 v1, vec3 v2)
{
    return vec3_create(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

INL vec3 vec3_mul(vec3 v1, vec3 v2)
{
    return vec3_create(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

INL vec3 vec3_div(vec3 v1, vec3 v2)
{
    return vec3_create(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

// Scalar
INL vec3 vec3_scale(vec3 v, f32 s)
{
    return (vec3){.x = v.x * s, .y = v.y * s, .z = v.z * s};
}

INL vec3 vec3_scale_div(vec3 v, f32 s)
{
    return (vec3){{v.x / s, v.y / s, v.z / s}};
}

// Operations
INL f32 vec3_length_sq(vec3 v) { return v.x * v.x + v.y * v.y + v.z * v.z; }

INL f32 vec3_dot(vec3 v1, vec3 v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

INL vec3 vec3_cross(vec3 v1, vec3 v2)
{
    return vec3_create(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
                       v1.x * v2.y - v1.y * v2.x);
}

// Normalization
INL vec3 vec3_normalize(vec3 v)
{
    f32 len = vec3_length(v);
    if (len > M_EPSILON) return vec3_scale(v, 1.0f / len);
    return vec3_zero();
}

INL void vec3_normalize_ptr(vec3 *v)
{
    f32 len = vec3_length(*v);
    if (len > M_EPSILON)
    {
        v->x /= len;
        v->y /= len;
        v->z /= len;
    }
    else
    {
        *v = vec3_zero();
    }
}

// Utility
INL f32 vec3_distance(vec3 v1, vec3 v2)
{
    return vec3_length(vec3_sub(v1, v2));
}

INL f32 vec3_distance_sq(vec3 v1, vec3 v2)
{
    vec3 diff = vec3_sub(v1, v2);
    return vec3_length_sq(diff);
}

INL vec3 vec3_lerp(vec3 v1, vec3 v2, f32 t)
{
    return vec3_create(LERP(v1.x, v2.x, t), LERP(v1.y, v2.y, t),
                       LERP(v1.z, v2.z, t));
}

INL b8 vec3_compare(vec3 a, vec3 b, f32 tolerance)
{
    return (m_abs(a.x - b.x) <= tolerance) &&
           (m_abs(a.y - b.y) <= tolerance) && (m_abs(a.z - b.z) <= tolerance);
}

INL b8 vec3_is_zero(vec3 v) { return vec3_compare(v, vec3_zero(), M_EPSILON); }

INL vec3 vec3_reflect(vec3 v, vec3 normal)
{
    f32 dot = vec3_dot(v, normal);
    return vec3_sub(v, vec3_scale(normal, 2.0f * dot));
}

/*************************
 * VECTOR 4
 *************************/
// Declaration (implement in maths.c)
f32 vec4_length(vec4 v);

// Constructor
INL vec4 vec4_create(f32 x, f32 y, f32 z, f32 w)
{
    vec4 result;
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;
    return result;
}

INL vec4 vec4_unit_zero(void) { return vec4_create(0.0f, 0.0f, 0.0f, 0.0f); }
INL vec4 vec4_unit_one(void) { return vec4_create(1.0f, 1.0f, 1.0f, 1.0f); }
INL vec4 vec4_unit_x(void) { return vec4_create(1.0f, 0.0f, 0.0f, 0.0f); }
INL vec4 vec4_unit_y(void) { return vec4_create(0.0f, 1.0f, 0.0f, 0.0f); }
INL vec4 vec4_unit_z(void) { return vec4_create(0.0f, 0.0f, 1.0f, 0.0f); }
INL vec4 vec4_unit_w(void) { return vec4_create(0.0f, 0.0f, 0.0f, 1.0f); }

// Arithmetic
INL vec4 vec4_add(vec4 v1, vec4 v2)
{
    return vec4_create(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

INL vec4 vec4_sub(vec4 v1, vec4 v2)
{
    return vec4_create(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

INL vec4 vec4_mul(vec4 v1, vec4 v2)
{
    return vec4_create(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

INL vec4 vec4_div(vec4 v1, vec4 v2)
{
    return vec4_create(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
}

// Scalar
INL vec4 vec4_scale(vec4 v, f32 s)
{
    return (vec4){.x = v.x * s, .y = v.y * s, .z = v.z * s, .w = v.w * s};
}

INL vec4 vec4_scale_div(vec4 v, f32 s)
{
    return (vec4){{v.x / s, v.y / s, v.z / s, v.w / s}};
}

// Operations
INL f32 vec4_length_sq(vec4 v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}

INL f32 vec4_dot(vec4 v1, vec4 v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

// Normalization
INL vec4 vec4_normalize(vec4 v)
{
    f32 len = vec4_length(v);
    if (len > M_EPSILON) return vec4_scale(v, 1.0f / len);
    return vec4_unit_zero();
}

INL void vec4_normalize_ptr(vec4 *v)
{
    f32 len = vec4_length(*v);
    if (len > M_EPSILON)
    {
        v->x /= len;
        v->y /= len;
        v->z /= len;
        v->w /= len;
    }
    else
    {
        *v = vec4_unit_zero();
    }
}

// Utility
INL f32 vec4_distance(vec4 a, vec4 b) { return vec4_length(vec4_sub(a, b)); }

INL f32 vec4_distance_sq(vec4 a, vec4 b)
{
    vec4 diff = vec4_sub(a, b);
    return vec4_length_sq(diff);
}

INL vec4 vec4_lerp(vec4 a, vec4 b, f32 t)
{
    return vec4_create(LERP(a.x, b.x, t), LERP(a.y, b.y, t), LERP(a.z, b.z, t),
                       LERP(a.w, b.w, t));
}

INL b8 vec4_compare(vec4 a, vec4 b, f32 tolerance)
{
    return (m_abs(a.x - b.x) <= tolerance) &&
           (m_abs(a.y - b.y) <= tolerance) &&
           (m_abs(a.z - b.z) <= tolerance) && (m_abs(a.w - b.w) <= tolerance);
}

INL b8 vec4_is_zero(vec4 v)
{
    return vec4_compare(v, vec4_unit_zero(), M_EPSILON);
}

/*************************
 * MATRIX
 *************************/
// Declaration (implement in maths.c)
mat4 mat4_inverse(mat4 m);
mat4 mat4_look_at(vec3 eye, vec3 target, vec3 up);
mat4 mat4_perspective(f32 fov, f32 aspect, f32 near, f32 far);
mat4 mat4_orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near,
                       f32 far);

// clang-format off
// Constructor
INL mat4 mat4_create(f32 m00, f32 m01, f32 m02, f32 m03, f32 m10, f32 m11,
                     f32 m12, f32 m13, f32 m20, f32 m21, f32 m22, f32 m23,
                     f32 m30, f32 m31, f32 m32, f32 m33)
{
    mat4 m;
    // Column-major: data[col * 4 + row]
    m.data[0] = m00;
    m.data[4] = m01;
    m.data[8] = m02;
    m.data[12] = m03;
    m.data[1] = m10;
    m.data[5] = m11;
    m.data[9] = m12;
    m.data[13] = m13;
    m.data[2] = m20;
    m.data[6] = m21;
    m.data[10] = m22;
    m.data[14] = m23;
    m.data[3] = m30;
    m.data[7] = m31;
    m.data[11] = m32;
    m.data[15] = m33;
    return m;
}

INL mat4 mat4_identity(void)
{
    return mat4_create(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

INL mat4 mat4_zero(void)
{
    mat4 m;
    for (u32 i = 0; i < 16; i++) m.data[i] = 0.0f;
    return m;
}

INL vec3 mat4_forward(mat4 m) {
    // Forward is usually -Z axis (third column, negated)
    return vec3_normalize(vec3_create(-m.data[8], -m.data[9], -m.data[10]));
}

INL vec3 mat4_backward(mat4 m) {
    // Backward is +Z axis (third column)
    return vec3_normalize(vec3_create(m.data[8], m.data[9], m.data[10]));
}

INL vec3 mat4_left(mat4 m) {
    // Left is -X axis (first column, negated)
    return vec3_normalize(vec3_create(-m.data[0], -m.data[1], -m.data[2]));
}

INL vec3 mat4_right(mat4 m) {
    // Right is +X axis (first column)
    return vec3_normalize(vec3_create(m.data[0], m.data[1], m.data[2]));
}

INL vec3 mat4_up(mat4 m) {
    // Up is +Y axis (second column)
    return vec3_normalize(vec3_create(m.data[4], m.data[5], m.data[6]));
}

INL vec3 mat4_down(mat4 m) {
    // Down is -Y axis (second column, negated)
    return vec3_normalize(vec3_create(-m.data[4], -m.data[5], -m.data[6]));
}

// Operations
INL mat4 mat4_add(mat4 a, mat4 b)
{
    mat4 result;
    for (u32 i = 0; i < 16; i++) result.data[i] = a.data[i] + b.data[i];
    return result;
}

INL mat4 mat4_sub(mat4 a, mat4 b)
{
    mat4 result;
    for (u32 i = 0; i < 16; i++) result.data[i] = a.data[i] - b.data[i];
    return result;
}

INL mat4 mat4_scale(mat4 m, f32 scalar)
{
    mat4 result;
    for (u32 i = 0; i < 16; i++) result.data[i] = m.data[i] * scalar;
    return result;
}

// Matrix multiplication (Column-major: A * B)
INL mat4 mat4_mul(mat4 m1, mat4 m2)
{
    mat4 result = mat4_zero();
    for (u32 col = 0; col < 4; col++) {
        for (u32 row = 0; row < 4; row++) {
            f32 sum = 0.0f;
            for (u32 k = 0; k < 4; k++) {
                sum += m1.data[k * 4 + row] * m2.data[col * 4 + k];
            }
            result.data[col * 4 + row] = sum;
        }
    }
    return result;
}

// Vector transformation
INL vec4 mat4_mul_vec4(mat4 m, vec4 v)
{
    vec4 result;
    for (u32 row = 0; row < 4; row++) {
        result.elements[row] = 
            m.data[0 * 4 + row] * v.x +
            m.data[1 * 4 + row] * v.y + 
            m.data[2 * 4 + row] * v.z +
            m.data[3 * 4 + row] * v.w;
    }
    return result;
}

INL vec3 mat4_mul_vec3(mat4 m, vec3 v)
{
    vec4 v4 = vec4_create(v.x, v.y, v.z, 1.0f);
    vec4 result = mat4_mul_vec4(m, v4);
    return vec3_create(result.x, result.y, result.z);
}

// Transpose
INL mat4 mat4_transpose(mat4 m)
{
    return mat4_create(
        m.data[0], m.data[1], m.data[2], m.data[3],
        m.data[4], m.data[5], m.data[6], m.data[7],
        m.data[8], m.data[9], m.data[10], m.data[11],
        m.data[12], m.data[13], m.data[14], m.data[15]
    );
}

// Transformation matrices
INL mat4 mat4_translation(vec3 translation)
{
    mat4 m = mat4_identity();
    m.data[12] = translation.x;
    m.data[13] = translation.y; 
    m.data[14] = translation.z;
    return m;
}

INL mat4 mat4_scaling(vec3 scale)
{
    return mat4_create(
        scale.x, 0.0f,    0.0f,    0.0f,
        0.0f,    scale.y, 0.0f,    0.0f,
        0.0f,    0.0f,    scale.z, 0.0f,
        0.0f,    0.0f,    0.0f,    1.0f
    );
}

INL mat4 mat4_rotation_x(f32 angle)
{
    f32 c = m_cos(angle);
    f32 s = m_sin(angle);
    return mat4_create(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, c,    -s,   0.0f,
        0.0f, s,    c,    0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

INL mat4 mat4_rotation_y(f32 angle)
{
    f32 c = m_cos(angle);
    f32 s = m_sin(angle);
    return mat4_create(
        c,    0.0f, s,    0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -s,   0.0f, c,    0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

INL mat4 mat4_rotation_z(f32 angle)
{
    f32 c = m_cos(angle);
    f32 s = m_sin(angle);
    return mat4_create(
        c,    -s,   0.0f, 0.0f,
        s,    c,    0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

INL mat4 mat4_rotation_xyz(vec3 angles)
{
    f32 cx = m_cos(angles.x);
    f32 sx = m_sin(angles.x);
    f32 cy = m_cos(angles.y);
    f32 sy = m_sin(angles.y);
    f32 cz = m_cos(angles.z);
    f32 sz = m_sin(angles.z);

    // Combined rotation matrix: Rz * Ry * Rx
    return mat4_create(
        cy * cz,     cz * sx * sy - cx * sz,    cx * cz * sy + sx * sz,    0.0f,
        cy * sz,     cx * cz + sx * sy * sz,    cx * sy * sz - cz * sx,    0.0f,
        -sy,         cy * sx,                   cx * cy,                   0.0f,
        0.0f,        0.0f,                      0.0f,                      1.0f
    );
}

INL mat4 mat4_rotation_xyz_angles(f32 x_rad, f32 y_rad, f32 z_rad)
{
    return mat4_rotation_xyz(vec3_create(x_rad, y_rad, z_rad));
}

/*************************
 * QUATERNION
 *************************/
// Declaration (implement in maths.c)
quat quat_slerp(quat a, quat b, f32 t);
mat4 quat_to_mat4(quat q);
vec3 quat_to_euler(quat q);
quat quat_look_rotation(vec3 direction, vec3 up);

// Constructor
INL quat quat_create(f32 x, f32 y, f32 z, f32 w) { 
    return (quat){.x = x, .y = y, .z = z, .w = w}; 
}

INL quat quat_identity(void) { 
    return quat_create(0.0f, 0.0f, 0.0f, 1.0f); 
}

INL quat quat_zero(void) { 
    return quat_create(0.0f, 0.0f, 0.0f, 0.0f); 
}

// Basic operations
INL quat quat_add(quat a, quat b) {
    return quat_create(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

INL quat quat_sub(quat a, quat b) {
    return quat_create(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

INL quat quat_scale(quat q, f32 scalar) {
    return quat_create(q.x * scalar, q.y * scalar, q.z * scalar, q.w * scalar);
}

// Multiplication (Hamilton product)
INL quat quat_mul(quat a, quat b) {
    return quat_create(
        a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
        a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
    );
}

// Conjugate (inverse for unit quaternions)
INL quat quat_conjugate(quat q) {
    return quat_create(-q.x, -q.y, -q.z, q.w);
}

// Dot product
INL f32 quat_dot(quat a, quat b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// Length operations
INL f32 quat_length_sq(quat q) {
    return quat_dot(q, q);
}

INL f32 quat_length(quat q) {
    return m_sqrt(quat_length_sq(q));
}

// Normalization
INL quat quat_normalize(quat q) {
    f32 len = quat_length(q);
    if (len > M_EPSILON) {
        return quat_scale(q, 1.0f / len);
    }
    return quat_identity();
}

INL void quat_normalize_ptr(quat* q) {
    f32 len = quat_length(*q);
    if (len > M_EPSILON) {
        q->x /= len;
        q->y /= len;
        q->z /= len;
        q->w /= len;
    } else {
        *q = quat_identity();
    }
}

// Rotation operations
INL quat quat_from_axis_angle(vec3 axis, f32 angle_rad) {
    f32 half_angle = angle_rad * 0.5f;
    f32 s = m_sin(half_angle);
    vec3 norm_axis = vec3_normalize(axis);
    
    return quat_create(
        norm_axis.x * s,
        norm_axis.y * s,
        norm_axis.z * s,
        m_cos(half_angle)
    );
}

INL quat quat_from_euler(f32 pitch, f32 yaw, f32 roll) {
    f32 half_pitch = pitch * 0.5f;
    f32 half_yaw = yaw * 0.5f;
    f32 half_roll = roll * 0.5f;
    
    f32 cy = m_cos(half_yaw);
    f32 sy = m_sin(half_yaw);
    f32 cp = m_cos(half_pitch);
    f32 sp = m_sin(half_pitch);
    f32 cr = m_cos(half_roll);
    f32 sr = m_sin(half_roll);
    
    return quat_create(
        sr * cp * cy - cr * sp * sy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy,
        cr * cp * cy + sr * sp * sy
    );
}

// Vector transformation
INL vec3 quat_mul_vec3(quat q, vec3 v) {
    vec3 q_vec = vec3_create(q.x, q.y, q.z);
    vec3 t = vec3_scale(vec3_cross(q_vec, v), 2.0f);
    return vec3_add(v, vec3_add(vec3_scale(t, q.w), vec3_cross(q_vec, t)));
}

// Interpolation
INL quat quat_lerp(quat a, quat b, f32 t) {
    return quat_normalize(quat_create(
        LERP(a.x, b.x, t),
        LERP(a.y, b.y, t),
        LERP(a.z, b.z, t),
        LERP(a.w, b.w, t)
    ));
}

#if MATH_SSE
INL mat4 mat4_mul_simd(mat4 a, mat4 b) {
    mat4 result;
    
    // Load all columns of matrix B
    __m128 b_col0 = _mm_load_ps(&b.data[0]);
    __m128 b_col1 = _mm_load_ps(&b.data[4]);
    __m128 b_col2 = _mm_load_ps(&b.data[8]);
    __m128 b_col3 = _mm_load_ps(&b.data[12]);
    
    for (u32 i = 0; i < 4; i++) {
        // Load row from matrix A (broadcast each element)
        __m128 a_row = _mm_load_ps(&a.data[i * 4]);
        
        // Broadcast each element of the row
        __m128 a_row0 = _mm_shuffle_ps(a_row, a_row, _MM_SHUFFLE(0, 0, 0, 0));
        __m128 a_row1 = _mm_shuffle_ps(a_row, a_row, _MM_SHUFFLE(1, 1, 1, 1));
        __m128 a_row2 = _mm_shuffle_ps(a_row, a_row, _MM_SHUFFLE(2, 2, 2, 2));
        __m128 a_row3 = _mm_shuffle_ps(a_row, a_row, _MM_SHUFFLE(3, 3, 3, 3));
        
        // Multiply and add
        __m128 col0 = _mm_mul_ps(a_row0, b_col0);
        __m128 col1 = _mm_mul_ps(a_row1, b_col1);
        __m128 col2 = _mm_mul_ps(a_row2, b_col2);
        __m128 col3 = _mm_mul_ps(a_row3, b_col3);
        
        __m128 result_col = _mm_add_ps(_mm_add_ps(col0, col1), _mm_add_ps(col2, col3));
        
        _mm_store_ps(&result.data[i * 4], result_col);
    }
    
    return result;
}
#endif // MATH_SSE

#endif // MATHS_LIB_H
