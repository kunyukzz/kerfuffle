#include "math_test.h"

#include "engine/core/clock.h"

#include <stdio.h>

#define RUN_TEST(test_func)                                                   \
    do                                                                        \
    {                                                                         \
        if (!test_func())                                                     \
        {                                                                     \
            printf("->FAIL: %s\n", #test_func);                               \
            all_passed = false;                                               \
        }                                                                     \
        else                                                                  \
        {                                                                     \
            printf("->PASS: %s\n", #test_func);                               \
        }                                                                     \
    }                                                                         \
    while (0)

b8 expect_f32(f32 actual, f32 expected, f32 t, const char *test_name)
{
    b8 passed = m_abs(actual - expected) <= t;
    if (!passed)
    {
        printf("  %s: expected %.6f, got %.6f\n", test_name, expected, actual);
    }
    return passed;
}

b8 expect_vec3(vec3 actual, vec3 expected, f32 t, const char *test_name)
{
    b8 passed = vec3_compare(actual, expected, t);
    if (!passed)
    {
        printf("  %s: expected (%.6f, %.6f, %.6f), got (%.6f, %.6f, %.6f)\n",
               test_name, expected.x, expected.y, expected.z, actual.x,
               actual.y, actual.z);
    }
    return passed;
}

b8 expect_mat4(mat4 actual, mat4 expected, f32 t, const char *test_name)
{
    for (u32 i = 0; i < 16; i++)
    {
        if (m_abs(actual.data[i] - expected.data[i]) > t)
        {
            printf("  %s: element %d differs\n", test_name, i);
            mat4_print_col("Expected", expected);
            mat4_print_col("Actual", actual);
            return false;
        }
    }
    return true;
}

b8 expect_quat(quat actual, quat expected, f32 t, const char *test_name)
{
    b8 passed = m_abs(actual.x - expected.x) <= t &&
                m_abs(actual.y - expected.y) <= t &&
                m_abs(actual.z - expected.z) <= t &&
                m_abs(actual.w - expected.w) <= t;
    if (!passed)
    {
        printf("  %s: expected (%.6f, %.6f, %.6f, %.6f), got (%.6f, %.6f, "
               "%.6f, %.6f)\n",
               test_name, expected.x, expected.y, expected.z, expected.w,
               actual.x, actual.y, actual.z, actual.w);
    }
    return passed;
}

b8 expect_vec4(vec4 actual, vec4 expected, f32 t, const char *test_name)
{
    b8 passed = (m_abs(actual.x - expected.x) <= t) &&
                (m_abs(actual.y - expected.y) <= t) &&
                (m_abs(actual.z - expected.z) <= t) &&
                (m_abs(actual.w - expected.w) <= t);
    if (!passed)
    {
        printf("  %s: expected (%.6f, %.6f, %.6f, %.6f), got (%.6f, %.6f, "
               "%.6f, %.6f)\n",
               test_name, expected.x, expected.y, expected.z, expected.w,
               actual.x, actual.y, actual.z, actual.w);
    }
    return passed;
}

b8 test_vectors(void)
{
    b8 all_passed = true;

    // Test basic operations
    vec3 a = vec3_create(1.0f, 2.0f, 3.0f);
    vec3 b = vec3_create(4.0f, 5.0f, 6.0f);

    all_passed &= expect_vec3(vec3_add(a, b), vec3_create(5.0f, 7.0f, 9.0f),
                              M_EPSILON, "vec3_add");
    all_passed &= expect_vec3(vec3_sub(a, b), vec3_create(-3.0f, -3.0f, -3.0f),
                              M_EPSILON, "vec3_sub");
    all_passed &= expect_f32(vec3_dot(a, b), 32.0f, M_EPSILON, "vec3_dot");

    vec3 cross = vec3_cross(vec3_right(), vec3_up());
    all_passed &= expect_vec3(cross, vec3_back(), M_EPSILON, "vec3_cross");

    // Test normalization
    vec3 norm = vec3_normalize(vec3_create(3.0f, 0.0f, 0.0f));
    all_passed &= expect_vec3(norm, vec3_right(), M_EPSILON, "vec3_normalize");

    return all_passed;
}

b8 test_matrices(void)
{
    b8 all_passed = true;

    // Test identity
    mat4 identity = mat4_identity();
    mat4 should_be_identity = mat4_mul(identity, identity);
    all_passed &= expect_mat4(should_be_identity, identity, M_EPSILON,
                              "identity multiplication");

    // Test translation
    vec3 translation = vec3_create(5.0f, 10.0f, 15.0f);
    mat4 trans_mat = mat4_translation(translation);
    vec3 point = vec3_create(1.0f, 2.0f, 3.0f);
    vec3 translated = mat4_mul_vec3(trans_mat, point);
    all_passed &= expect_vec3(translated, vec3_create(6.0f, 12.0f, 18.0f),
                              M_EPSILON, "translation");

    // Test rotation
    mat4 rot_x = mat4_rotation_x(M_HALF_PI);
    vec3 rotated = mat4_mul_vec3(rot_x, vec3_up());
    all_passed &= expect_vec3(rotated, vec3_create(0.0f, 0.0f, 1.0f), 0.001f,
                              "rotation_x");

    // Test inverse for simple cases
    mat4 trans_inv = mat4_inverse(trans_mat);
    vec3 original = mat4_mul_vec3(trans_inv, translated);
    all_passed &= expect_vec3(original, point, 0.001f, "matrix inverse");

    return all_passed;
}

b8 test_quaternions(void)
{
    b8 all_passed = true;

    // Test identity
    quat id = quat_identity();
    vec3 point = vec3_create(1.0f, 0.0f, 0.0f);
    vec3 rotated = quat_mul_vec3(id, point);
    all_passed &= expect_vec3(rotated, point, M_EPSILON, "quat_identity");

    // Test 90 degree rotation around Y axis
    quat rot_y = quat_from_axis_angle(vec3_up(), M_HALF_PI);
    vec3 result = quat_mul_vec3(rot_y, vec3_right());
    // vec3 result = quat_mul_vec3(rot_y, vec3_left());
    all_passed &= expect_vec3(result, vec3_create(0.0f, 0.0f, -1.0f), 0.001f,
                              "quat_rotation");

    // Test quat to matrix and back
    mat4 rot_mat = quat_to_mat4(rot_y);
    vec3 mat_result = mat4_mul_vec3(rot_mat, vec3_right());
    all_passed &= expect_vec3(mat_result, result, 0.001f, "quat_to_mat4");

    // Test slerp
    quat a = quat_from_axis_angle(vec3_up(), 0.0f);
    quat b = quat_from_axis_angle(vec3_up(), M_HALF_PI);
    quat halfway = quat_slerp(a, b, 0.5f);
    vec3 halfway_result = quat_mul_vec3(halfway, vec3_right());
    all_passed &= expect_vec3(halfway_result,
                              vec3_create(M_SQRT_2 / 2, 0.0f, -M_SQRT_2 / 2),
                              0.001f, "quat_slerp");

    return all_passed;
}

b8 test_transforms(void)
{
    b8 all_passed = true;

    // Test look_at
    vec3 eye = vec3_create(0.0f, 0.0f, 5.0f);
    vec3 target = vec3_create(0.0f, 0.0f, 0.0f);
    vec3 up = vec3_up();
    mat4 view = mat4_look_at(eye, target, up);

    // Origin should be at (0,0,-5) in view space
    vec3 view_point = mat4_mul_vec3(view, vec3_zero());
    all_passed &= expect_vec3(view_point, vec3_create(0.0f, 0.0f, -5.0f),
                              0.001f, "look_at");

    // Test perspective projection
    mat4 proj = mat4_perspective(90.0f, 1.0f, 0.1f, 100.0f);
    all_passed &= !mat4_has_nan(&proj);

    return all_passed;
}

b8 test_matrices_dir(void)
{
    b8 all_passed = true;

    // Test identity matrix directions
    mat4 identity = mat4_identity();
    all_passed &=
        expect_vec3(mat4_forward(identity), vec3_create(0.0f, 0.0f, -1.0f),
                    M_EPSILON, "mat4_forward identity");
    all_passed &=
        expect_vec3(mat4_right(identity), vec3_create(1.0f, 0.0f, 0.0f),
                    M_EPSILON, "mat4_right identity");
    all_passed &= expect_vec3(mat4_up(identity), vec3_create(0.0f, 1.0f, 0.0f),
                              M_EPSILON, "mat4_up identity");

    // Test rotated matrix
    mat4 rotated = mat4_rotation_y(M_PI); // 180 degree rotation
    all_passed &=
        expect_vec3(mat4_forward(rotated), vec3_create(0.0f, 0.0f, 1.0f),
                    0.001f, "mat4_forward rotated");

    return all_passed;
}

b8 test_matrix_inverse(void)
{
    b8 all_passed = true;

    // Test identity matrix
    mat4 identity = mat4_identity();
    mat4 inv_identity = mat4_inverse(identity);
    all_passed &=
        expect_mat4(inv_identity, identity, M_EPSILON, "inverse(identity)");

    // Test simple translation
    mat4 translation = mat4_translation(vec3_create(1.0f, 2.0f, 3.0f));
    mat4 inv_translation = mat4_inverse(translation);
    mat4 should_be = mat4_translation(vec3_create(-1.0f, -2.0f, -3.0f));
    all_passed &= expect_mat4(inv_translation, should_be, 0.001f,
                              "inverse(translation)");

    return all_passed;
}

void math_run_all_tests(void)
{
    printf("\n=== RUN MATH LIBRARY TEST ===\n");

    b8 all_passed = true;
    RUN_TEST(test_vectors);
    RUN_TEST(test_matrices);
    RUN_TEST(test_quaternions);
    RUN_TEST(test_transforms);
    RUN_TEST(test_matrices_dir);
    RUN_TEST(test_matrix_inverse);

    printf("%s\n", all_passed ? "ALL PASSED" : "SOME FAILED");
}

void test_simd_vs_scalar(void)
{
#if MATH_SSE
    printf("\nSIMD test (Matrix Focus)\n");

    const u32 ITERATIONS = 100000;
    mat4 a = mat4_identity();
    mat4 b = mat4_rotation_x(M_PI / 4.0f);

    // Only benchmark matrices since vectors use scalar
    f64 start = timer_get();
    for (u32 i = 0; i < ITERATIONS; i++)
    {
        mat4 result = mat4_mul_simd(a, b);
        (void)result;
    }
    f64 simd_time = timer_get() - start;

    start = timer_get();
    for (u32 i = 0; i < ITERATIONS; i++)
    {
        mat4 result = mat4_mul(a, b); // This will use scalar internally
        (void)result;
    }
    f64 scalar_time = timer_get() - start;

    printf("Matrix Multiplication (100k iterations):\n");
    printf("->SIMD:   %.4f ms\n", simd_time * 1000);
    printf("->Scalar: %.4f ms\n", scalar_time * 1000);
    printf("->Speedup: %.2fx\n", scalar_time / simd_time);
    printf("=== DONE MATH LIBRARY TEST ===\n");
    printf("\n");

#else
    LOG_INFO("SIMD not available");
#endif
}

void benchmark_simd_vs_scalar(math_benchmark *result)
{
#if MATH_SSE
    const u32 ITERATIONS = 10000;

    // Setup test data
    mat4 a = mat4_identity();
    mat4 b = mat4_rotation_x(M_PI / 4.0f);
    vec3 v = vec3_create(1.0f, 2.0f, 3.0f);

    f64 start, end;

    // Benchmark matrix multiplication - SIMD vs Scalar
    start = timer_get();
    for (u32 i = 0; i < ITERATIONS; i++)
    {
        mat4 c = mat4_mul_simd(a, b); // DIRECT SIMD CALL
        (void)c;
    }
    end = timer_get();
    result->matrix_simd_time = end - start;

    start = timer_get();
    for (u32 i = 0; i < ITERATIONS; i++)
    {
        mat4 c = mat4_mul(a, b); // DIRECT SCALAR CALL
        (void)c;
    }
    end = timer_get();
    result->matrix_scalar_time = end - start;

    // Benchmark vector operations (both use scalar since we removed vector
    // SIMD)
    start = timer_get();
    for (u32 i = 0; i < ITERATIONS; i++)
    {
        vec3 r1 = vec3_add(v, v);
        vec3 r2 = vec3_cross(v, r1);
        f32 dot = vec3_dot(r1, r2);
        vec3 r3 = vec3_normalize(r2);
        (void)dot;
        (void)r3;
    }
    end = timer_get();
    result->vector_ops_time = end - start;

    // Benchmark transform operations
    start = timer_get();
    for (u32 i = 0; i < ITERATIONS; i++)
    {
        mat4 view = mat4_look_at(v, vec3_zero(), vec3_up());
        mat4 proj = mat4_perspective(60.0f, 16.0f / 9.0f, 0.1f, 100.0f);
        mat4 mvp = mat4_mul_simd(proj, view); // Use SIMD for matrix part
        vec3 transformed = mat4_mul_vec3(mvp, v);
        (void)transformed;
    }
    end = timer_get();
    result->transform_simd_time = end - start;

    start = timer_get();
    for (u32 i = 0; i < ITERATIONS; i++)
    {
        mat4 view = mat4_look_at(v, vec3_zero(), vec3_up());
        mat4 proj = mat4_perspective(60.0f, 16.0f / 9.0f, 0.1f, 100.0f);
        mat4 mvp = mat4_mul(proj, view); // Use scalar for matrix part
        vec3 transformed = mat4_mul_vec3(mvp, v);
        (void)transformed;
    }
    end = timer_get();
    result->transform_scalar_time = end - start;

    result->iterations = ITERATIONS;
#endif
}
