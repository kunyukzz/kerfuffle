#ifndef MATH_TEST_H
#define MATH_TEST_H

#include "maths.h" // IWYU pragma: keep

typedef struct math_benchmark {
    f64 matrix_mul_time;
    f64 matrix_simd_time;   // New
    f64 matrix_scalar_time; // New
    f64 vector_ops_time;
    f64 transform_time;
    f64 transform_simd_time;   // New
    f64 transform_scalar_time; // New
    u32 iterations;
} math_benchmark;

void benchmark_simd_vs_scalar(math_benchmark *result);

void math_run_all_tests(void);
void test_simd_vs_scalar(void);

b8 test_vectors(void);
b8 test_matrices(void);
b8 test_quaternions(void);
b8 test_transforms(void);

b8 expect_f32(f32 actual, f32 expected, f32 t, const char *test_name);
b8 expect_vec3(vec3 actual, vec3 expected, f32 t, const char *test_name);
b8 expect_mat4(mat4 actual, mat4 expected, f32 t, const char *test_name);
b8 expect_quat(quat actual, quat expected, f32 t, const char *test_name);
b8 expect_vec4(vec4 actual, vec4 expected, f32 t, const char *test_name);

#endif // MATH_TEST_H
