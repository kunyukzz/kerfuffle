#include "maths.h"

f32 vec2_length(vec2 v) { return sqrtf(vec2_length_sq(v)); }

f32 vec3_length(vec3 v) { return sqrtf(vec3_length_sq(v)); }

f32 vec4_length(vec4 v) { return sqrtf(vec4_length_sq(v)); }

// clang-format off

/*
mat4 mat4_inverse(mat4 m)
{
    mat4 result;
    f32 *inv = result.data;
    const f32 *mat = m.data;

    f32 det;
    int i;

    inv[0] = mat[5] * mat[10] * mat[15] - mat[5] * mat[11] * mat[14] -
             mat[9] * mat[6] * mat[15] + mat[9] * mat[7] * mat[14] +
             mat[13] * mat[6] * mat[11] - mat[13] * mat[7] * mat[10];

    inv[4] = -mat[4] * mat[10] * mat[15] + mat[4] * mat[11] * mat[14] +
             mat[8] * mat[6] * mat[15] - mat[8] * mat[7] * mat[14] -
             mat[12] * mat[6] * mat[11] + mat[12] * mat[7] * mat[10];

    inv[8] = mat[4] * mat[9] * mat[15] - mat[4] * mat[11] * mat[13] -
             mat[8] * mat[5] * mat[15] + mat[8] * mat[7] * mat[13] +
             mat[12] * mat[5] * mat[11] - mat[12] * mat[7] * mat[9];

    inv[12] = -mat[4] * mat[9] * mat[14] + mat[4] * mat[10] * mat[13] +
              mat[8] * mat[5] * mat[14] - mat[8] * mat[6] * mat[13] -
              mat[12] * mat[5] * mat[10] + mat[12] * mat[6] * mat[9];

    inv[1] = -mat[1] * mat[10] * mat[15] + mat[1] * mat[11] * mat[14] +
             mat[9] * mat[2] * mat[15] - mat[9] * mat[3] * mat[14] -
             mat[13] * mat[2] * mat[11] + mat[13] * mat[3] * mat[10];

    inv[5] = mat[0] * mat[10] * mat[15] - mat[0] * mat[11] * mat[14] -
             mat[8] * mat[2] * mat[15] + mat[8] * mat[3] * mat[14] +
             mat[12] * mat[2] * mat[11] - mat[12] * mat[3] * mat[10];

    inv[9] = -mat[0] * mat[9] * mat[15] + mat[0] * mat[11] * mat[13] +
             mat[8] * mat[1] * mat[15] - mat[8] * mat[3] * mat[13] -
             mat[12] * mat[1] * mat[11] + mat[12] * mat[3] * mat[9];

    inv[13] = mat[0] * mat[9] * mat[14] - mat[0] * mat[10] * mat[13] -
              mat[8] * mat[1] * mat[14] + mat[8] * mat[2] * mat[13] +
              mat[12] * mat[1] * mat[10] - mat[12] * mat[2] * mat[9];

    inv[2] = mat[1] * mat[6] * mat[15] - mat[1] * mat[7] * mat[14] -
             mat[5] * mat[2] * mat[15] + mat[5] * mat[3] * mat[14] +
             mat[13] * mat[2] * mat[7] - mat[13] * mat[3] * mat[6];

    inv[6] = -mat[0] * mat[6] * mat[15] + mat[0] * mat[7] * mat[14] +
             mat[4] * mat[2] * mat[15] - mat[4] * mat[3] * mat[14] -
             mat[12] * mat[2] * mat[7] + mat[12] * mat[3] * mat[6];

    inv[10] = mat[0] * mat[5] * mat[15] - mat[0] * mat[7] * mat[13] -
              mat[4] * mat[1] * mat[15] + mat[4] * mat[3] * mat[13] +
              mat[12] * mat[1] * mat[7] - mat[12] * mat[3] * mat[5];

    inv[14] = -mat[0] * mat[5] * mat[14] + mat[0] * mat[6] * mat[13] +
              mat[4] * mat[1] * mat[14] - mat[4] * mat[2] * mat[13] -
              mat[12] * mat[1] * mat[6] + mat[12] * mat[2] * mat[5];

    inv[3] = -mat[1] * mat[6] * mat[11] + mat[1] * mat[7] * mat[10] +
             mat[5] * mat[2] * mat[11] - mat[5] * mat[3] * mat[10] -
             mat[9] * mat[2] * mat[7] + mat[9] * mat[3] * mat[6];

    inv[7] = mat[0] * mat[6] * mat[11] - mat[0] * mat[7] * mat[10] -
             mat[4] * mat[2] * mat[11] + mat[4] * mat[3] * mat[10] +
             mat[8] * mat[2] * mat[7] - mat[8] * mat[3] * mat[6];

    inv[11] = -mat[0] * mat[5] * mat[11] + mat[0] * mat[7] * mat[9] +
              mat[4] * mat[1] * mat[11] - mat[4] * mat[3] * mat[9] -
              mat[8] * mat[1] * mat[7] + mat[8] * mat[3] * mat[5];

    inv[15] = mat[0] * mat[5] * mat[10] - mat[0] * mat[6] * mat[9] -
              mat[4] * mat[1] * mat[10] + mat[4] * mat[2] * mat[9] +
              mat[8] * mat[1] * mat[6] - mat[8] * mat[2] * mat[5];

    det =
        mat[0] * inv[0] + mat[1] * inv[4] + mat[2] * inv[8] + mat[3] * inv[12];

    if (m_abs(det) < 1e-10f) return mat4_identity();

    det = 1.0f / det;

    for (i = 0; i < 16; i++) result.data[i] = inv[i] * det;

    return result;
}
*/

mat4 mat4_inverse(mat4 m)
{
    mat4 result;
    static f32 inv[16];

    const f32 *mat = m.data;

    // Calculate adjugate matrix
    inv[0] = mat[5] * mat[10] * mat[15] - mat[5] * mat[11] * mat[14] -
             mat[9] * mat[6] * mat[15] + mat[9] * mat[7] * mat[14] +
             mat[13] * mat[6] * mat[11] - mat[13] * mat[7] * mat[10];

    inv[4] = -mat[4] * mat[10] * mat[15] + mat[4] * mat[11] * mat[14] +
             mat[8] * mat[6] * mat[15] - mat[8] * mat[7] * mat[14] -
             mat[12] * mat[6] * mat[11] + mat[12] * mat[7] * mat[10];

    inv[8] = mat[4] * mat[9] * mat[15] - mat[4] * mat[11] * mat[13] -
             mat[8] * mat[5] * mat[15] + mat[8] * mat[7] * mat[13] +
             mat[12] * mat[5] * mat[11] - mat[12] * mat[7] * mat[9];

    inv[12] = -mat[4] * mat[9] * mat[14] + mat[4] * mat[10] * mat[13] +
              mat[8] * mat[5] * mat[14] - mat[8] * mat[6] * mat[13] -
              mat[12] * mat[5] * mat[10] + mat[12] * mat[6] * mat[9];

    inv[1] = -mat[1] * mat[10] * mat[15] + mat[1] * mat[11] * mat[14] +
             mat[9] * mat[2] * mat[15] - mat[9] * mat[3] * mat[14] -
             mat[13] * mat[2] * mat[11] + mat[13] * mat[3] * mat[10];

    inv[5] = mat[0] * mat[10] * mat[15] - mat[0] * mat[11] * mat[14] -
             mat[8] * mat[2] * mat[15] + mat[8] * mat[3] * mat[14] +
             mat[12] * mat[2] * mat[11] - mat[12] * mat[3] * mat[10];

    inv[9] = -mat[0] * mat[9] * mat[15] + mat[0] * mat[11] * mat[13] +
             mat[8] * mat[1] * mat[15] - mat[8] * mat[3] * mat[13] -
             mat[12] * mat[1] * mat[11] + mat[12] * mat[3] * mat[9];

    inv[13] = mat[0] * mat[9] * mat[14] - mat[0] * mat[10] * mat[13] -
              mat[8] * mat[1] * mat[14] + mat[8] * mat[2] * mat[13] +
              mat[12] * mat[1] * mat[10] - mat[12] * mat[2] * mat[9];

    inv[2] = mat[1] * mat[6] * mat[15] - mat[1] * mat[7] * mat[14] -
             mat[5] * mat[2] * mat[15] + mat[5] * mat[3] * mat[14] +
             mat[13] * mat[2] * mat[7] - mat[13] * mat[3] * mat[6];

    inv[6] = -mat[0] * mat[6] * mat[15] + mat[0] * mat[7] * mat[14] +
             mat[4] * mat[2] * mat[15] - mat[4] * mat[3] * mat[14] -
             mat[12] * mat[2] * mat[7] + mat[12] * mat[3] * mat[6];

    inv[10] = mat[0] * mat[5] * mat[15] - mat[0] * mat[7] * mat[13] -
              mat[4] * mat[1] * mat[15] + mat[4] * mat[3] * mat[13] +
              mat[12] * mat[1] * mat[7] - mat[12] * mat[3] * mat[5];

    inv[14] = -mat[0] * mat[5] * mat[14] + mat[0] * mat[6] * mat[13] +
              mat[4] * mat[1] * mat[14] - mat[4] * mat[2] * mat[13] -
              mat[12] * mat[1] * mat[6] + mat[12] * mat[2] * mat[5];

    inv[3] = -mat[1] * mat[6] * mat[11] + mat[1] * mat[7] * mat[10] +
             mat[5] * mat[2] * mat[11] - mat[5] * mat[3] * mat[10] -
             mat[9] * mat[2] * mat[7] + mat[9] * mat[3] * mat[6];

    inv[7] = mat[0] * mat[6] * mat[11] - mat[0] * mat[7] * mat[10] -
             mat[4] * mat[2] * mat[11] + mat[4] * mat[3] * mat[10] +
             mat[8] * mat[2] * mat[7] - mat[8] * mat[3] * mat[6];

    inv[11] = -mat[0] * mat[5] * mat[11] + mat[0] * mat[7] * mat[9] +
              mat[4] * mat[1] * mat[11] - mat[4] * mat[3] * mat[9] -
              mat[8] * mat[1] * mat[7] + mat[8] * mat[3] * mat[5];

    inv[15] = mat[0] * mat[5] * mat[10] - mat[0] * mat[6] * mat[9] -
              mat[4] * mat[1] * mat[10] + mat[4] * mat[2] * mat[9] +
              mat[8] * mat[1] * mat[6] - mat[8] * mat[2] * mat[5];

    f32 det =
        mat[0] * inv[0] + mat[1] * inv[4] + mat[2] * inv[8] + mat[3] * inv[12];

    if (m_abs(det) < 1e-10f) return mat4_identity();
    det = 1.0f / det;

    for (int i = 0; i < 16; i++) result.data[i] = inv[i] * det;

    return result;
}

mat4 mat4_look_at(vec3 eye, vec3 target, vec3 up)
{
    vec3 z = vec3_normalize(vec3_sub(target, eye));
    vec3 x = vec3_normalize(vec3_cross(z, up));
    vec3 y = vec3_cross(x, z);

    z = vec3_scale(z, -1.0f); // OpenGL looks down negative Z

    return mat4_create(x.x, x.y, x.z, -vec3_dot(x, eye), y.x, y.y, y.z,
                       -vec3_dot(y, eye), z.x, z.y, z.z, -vec3_dot(z, eye),
                       0.0f, 0.0f, 0.0f, 1.0f);
}

/*
mat4 mat4_perspective(f32 fov_degrees, f32 aspect, f32 near, f32 far)
{
    f32 fov_rad = m_deg_to_rad(fov_degrees);
    f32 f = 1.0f / m_tan(fov_rad / 2.0f);
    f32 range = far - near;

	return mat4_create(
		f / aspect, 0.0f, 0.0f, 0.0f,
		0.0f, f, 0.0f, 0.0f,
		0.0f, 0.0f, (far + near) / range, -1.0f,
		0.0f, 0.0f, (2.0f * far * near) / range, 0.0f);
}
*/

mat4 mat4_perspective(f32 fov_degrees, f32 aspect, f32 near, f32 far)
{
    f32 fov_rad = m_deg_to_rad(fov_degrees);
    f32 tan_half_fov = tanf(fov_rad / 2.0f);
    f32 f = 1.0f / tan_half_fov;
    f32 range_inv = 1.0f / (near - far);

    return mat4_create(
        f / aspect, 0.0f, 0.0f, 0.0f,
        0.0f, f, 0.0f, 0.0f,
        0.0f, 0.0f, (far + near) * range_inv, -1.0f,
        0.0f, 0.0f, (2.0f * far * near) * range_inv, 0.0f
    );
}

mat4 mat4_orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near,
                       f32 far)
{
    f32 rl = 1.0f / (right - left);
    f32 tb = 1.0f / (top - bottom);
    f32 fn = 1.0f / (far - near);

    return mat4_create(2.0f * rl, 0.0f, 0.0f, -(right + left) * rl, 0.0f,
                       2.0f * tb, 0.0f, -(top + bottom) * tb, 0.0f, 0.0f,
                       -2.0f * fn, -(far + near) * fn, 0.0f, 0.0f, 0.0f, 1.0f);
}

quat quat_slerp(quat a, quat b, f32 t)
{
    // Calculate cosine of angle between quaternions
    f32 cos_angle = quat_dot(a, b);

    if (cos_angle < 0.0f)
    {
        b = quat_scale(b, -1.0f);
        cos_angle = -cos_angle;
    }

    if (cos_angle > 0.9995f)
    {
        return quat_normalize(quat_lerp(a, b, t));
    }

    // Calculate the angle and its sine
    f32 angle = m_acos(cos_angle);
    f32 sin_angle = m_sin(angle);

    // Calculate interpolation factors
    f32 factor_a = m_sin((1.0f - t) * angle) / sin_angle;
    f32 factor_b = m_sin(t * angle) / sin_angle;

    return quat_add(quat_scale(a, factor_a), quat_scale(b, factor_b));
}

// clang-format off
mat4 quat_to_mat4(quat q)
{
    q = quat_normalize(q);
    
    f32 xx = q.x * q.x;
    f32 yy = q.y * q.y;
    f32 zz = q.z * q.z;
    f32 xy = q.x * q.y;
    f32 xz = q.x * q.z;
    f32 yz = q.y * q.z;
    f32 wx = q.w * q.x;
    f32 wy = q.w * q.y;
    f32 wz = q.w * q.z;
    
    return mat4_create(
        1.0f - 2.0f * (yy + zz), 2.0f * (xy - wz),     		2.0f * (xz + wy),     		0.0f,
        2.0f * (xy + wz),        1.0f - 2.0f * (xx + zz), 	2.0f * (yz - wx),     		0.0f,
        2.0f * (xz - wy),        2.0f * (yz + wx),     		1.0f - 2.0f * (xx + yy), 	0.0f,
        0.0f,                    0.0f,                    	0.0f,                    	1.0f
    );
}

vec3 quat_to_euler(quat q) 
{
    q = quat_normalize(q);
    
    // Roll (x-axis rotation)
    f32 sinr_cosp = 2.0f * (q.w * q.x + q.y * q.z);
    f32 cosr_cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
    f32 roll = m_atan2(sinr_cosp, cosr_cosp);
    
    // Pitch (y-axis rotation)
    f32 sinp = 2.0f * (q.w * q.y - q.z * q.x);
    f32 pitch;
    if (m_abs(sinp) >= 1.0f) {
        // Use 90 degrees if out of range, preserve sign
        pitch = (sinp >= 0.0f) ? M_HALF_PI : -M_HALF_PI;
    } else {
        pitch = m_asin(sinp);
    }
    
    // Yaw (z-axis rotation)
    f32 siny_cosp = 2.0f * (q.w * q.z + q.x * q.y);
    f32 cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    f32 yaw = m_atan2(siny_cosp, cosy_cosp);
    
    return vec3_create(pitch, yaw, roll);
}

quat quat_look_rotation(vec3 direction, vec3 up) 
{
    vec3 forward = vec3_normalize(direction);
    vec3 right = vec3_normalize(vec3_cross(up, forward));
    vec3 new_up = vec3_cross(forward, right);
    
    // Build rotation matrix from orthonormal
    mat4 rot_matrix = mat4_create(
        right.x,   right.y,   right.z,   0.0f,
        new_up.x,  new_up.y,  new_up.z,  0.0f,
        forward.x, forward.y, forward.z, 0.0f,
        0.0f,      0.0f,      0.0f,      1.0f
    );
    
    // Convert matrix to quaternion
    // This is the inverse of quat_to_mat4
    f32 trace = rot_matrix.data[0] + rot_matrix.data[5] + rot_matrix.data[10];
    
    if (trace > 0.0f) {
        f32 s = 0.5f / m_sqrt(trace + 1.0f);
        return quat_create(
            (rot_matrix.data[9] - rot_matrix.data[6]) * s,
            (rot_matrix.data[2] - rot_matrix.data[8]) * s,
            (rot_matrix.data[4] - rot_matrix.data[1]) * s,
            0.25f / s
        );
    } else {
        if (rot_matrix.data[0] > rot_matrix.data[5] && rot_matrix.data[0] > rot_matrix.data[10]) {
            f32 s = 2.0f * m_sqrt(1.0f + rot_matrix.data[0] - rot_matrix.data[5] - rot_matrix.data[10]);
            return quat_create(
                0.25f * s,
                (rot_matrix.data[1] + rot_matrix.data[4]) / s,
                (rot_matrix.data[2] + rot_matrix.data[8]) / s,
                (rot_matrix.data[6] - rot_matrix.data[9]) / s
            );
        } else if (rot_matrix.data[5] > rot_matrix.data[10]) {
            f32 s = 2.0f * m_sqrt(1.0f + rot_matrix.data[5] - rot_matrix.data[0] - rot_matrix.data[10]);
            return quat_create(
                (rot_matrix.data[1] + rot_matrix.data[4]) / s,
                0.25f * s,
                (rot_matrix.data[6] + rot_matrix.data[9]) / s,
                (rot_matrix.data[2] - rot_matrix.data[8]) / s
            );
        } else {
            f32 s = 2.0f * m_sqrt(1.0f + rot_matrix.data[10] - rot_matrix.data[0] - rot_matrix.data[5]);
            return quat_create(
                (rot_matrix.data[2] + rot_matrix.data[8]) / s,
                (rot_matrix.data[6] + rot_matrix.data[9]) / s,
                0.25f * s,
                (rot_matrix.data[4] - rot_matrix.data[1]) / s
            );
        }
    }
}
