#include "../lib/Fix.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// ---- test infrastructure ----

static int tests_run    = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define PASS(name) do { tests_run++; tests_passed++; printf("  PASS  %s\n", name); } while(0)
#define FAIL(name, fmt, ...) do { tests_run++; tests_failed++; printf("  FAIL  %s: " fmt "\n", name, __VA_ARGS__); } while(0)

// Check that two floats are within a tolerance (absolute)
#define ASSERT_NEAR(name, actual, expected, tol) do { \
    float _a = (actual), _e = (expected), _t = (tol); \
    if (fabsf(_a - _e) <= _t) { PASS(name); } \
    else { FAIL(name, "got %f, expected %f (tol %f)", _a, _e, _t); } \
} while(0)

// Check that two ints are equal
#define ASSERT_EQ(name, actual, expected) do { \
    int _a = (actual), _e = (expected); \
    if (_a == _e) { PASS(name); } \
    else { FAIL(name, "got %d, expected %d", _a, _e); } \
} while(0)

// Check max-error sweep is below a percent threshold
#define ASSERT_MAX_ERR(name, max_err, threshold) do { \
    float _m = fabsf(max_err), _t = (threshold); \
    if (_m <= _t) { PASS(name); } \
    else { FAIL(name, "max error %.4f%% exceeds threshold %.4f%%", _m, _t); } \
} while(0)

// ---- float2fix / fix2float round-trip ----

void test_float_conversion()
{
    printf("\n-- float2fix / fix2float --\n");

    float vals[] = { 0.0f, 1.0f, -1.0f, 0.5f, -0.5f, 100.0f, -100.0f, 0.001f, 12345.678f };
    int n = sizeof(vals) / sizeof(vals[0]);

    for (int i = 0; i < n; i++)
    {
        fix15 fx = float2fix(vals[i]);
        float back = fix2float(fx);
        char label[64];
        snprintf(label, sizeof(label), "float roundtrip %.4f", vals[i]);
        // 17.15 resolution is 1/32768 ≈ 3.05e-5
        ASSERT_NEAR(label, back, vals[i], 0.001f);
    }
}

// ---- int2fix / fix2int round-trip ----

void test_int_conversion()
{
    printf("\n-- int2fix / fix2int --\n");

    int vals[] = { 0, 1, -1, 42, -42, 1000, -1000, 32767 };
    int n = sizeof(vals) / sizeof(vals[0]);

    for (int i = 0; i < n; i++)
    {
        fix15 fx = int2fix(vals[i]);
        int back = fix2int(fx);
        char label[64];
        snprintf(label, sizeof(label), "int roundtrip %d", vals[i]);
        ASSERT_EQ(label, back, vals[i]);
    }
}

// ---- multfix ----

void test_multfix()
{
    printf("\n-- multfix --\n");

    struct { float a; float b; } cases[] = {
        {  2.0f,   3.0f  },
        {  0.5f,   0.5f  },
        { -1.0f,   5.0f  },
        { -3.0f,  -4.0f  },
        {  0.0f, 100.0f  },
        {  1.0f,   1.0f  },
        { 10.5f,   3.2f  },
        {100.0f,   0.01f },
    };
    int n = sizeof(cases) / sizeof(cases[0]);

    for (int i = 0; i < n; i++)
    {
        fix15 fa = float2fix(cases[i].a);
        fix15 fb = float2fix(cases[i].b);
        float result = fix2float(multfix(fa, fb));
        float expected = cases[i].a * cases[i].b;
        char label[64];
        snprintf(label, sizeof(label), "multfix %.2f * %.2f", cases[i].a, cases[i].b);
        ASSERT_NEAR(label, result, expected, fabsf(expected) * 0.001f + 0.001f);
    }
}

// ---- divfix ----

void test_divfix()
{
    printf("\n-- divfix --\n");

    struct { float a; float b; } cases[] = {
        {  6.0f,   3.0f },
        {  1.0f,   2.0f },
        { -8.0f,   4.0f },
        { -6.0f,  -3.0f },
        { 10.0f,   0.1f },
        {  0.0f,   5.0f },
        {  1.0f,   1.0f },
        {100.0f,  10.0f },
    };
    int n = sizeof(cases) / sizeof(cases[0]);

    for (int i = 0; i < n; i++)
    {
        fix15 fa = float2fix(cases[i].a);
        fix15 fb = float2fix(cases[i].b);
        float result = fix2float(divfix(fa, fb));
        float expected = cases[i].a / cases[i].b;
        char label[64];
        snprintf(label, sizeof(label), "divfix %.2f / %.2f", cases[i].a, cases[i].b);
        ASSERT_NEAR(label, result, expected, fabsf(expected) * 0.01f + 0.01f);
    }
}

// ---- square ----

void test_square()
{
    printf("\n-- square --\n");

    float vals[] = { 0.0f, 1.0f, -1.0f, 2.0f, -3.0f, 0.5f, 10.0f, 0.1f, 50.0f };
    int n = sizeof(vals) / sizeof(vals[0]);

    for (int i = 0; i < n; i++)
    {
        fix15 fx = float2fix(vals[i]);
        float result = fix2float(square(fx));
        float expected = vals[i] * vals[i];
        char label[64];
        snprintf(label, sizeof(label), "square(%.2f)", vals[i]);
        ASSERT_NEAR(label, result, expected, fabsf(expected) * 0.001f + 0.001f);
    }
}

// ---- rfixSqrt sweep ----

float test_rfixSqrt_sweep()
{
    printf("\n-- rfixSqrt sweep --\n");

    fix15 curr = float2fix(0.01);
    float max_err = 0;
    float worst_val = 0;

    while (curr < int2fix(65535))
    {
        fix15 inc = (fix15) rand() >> 20;
        if (inc <= 0) inc = 1;

        fix15 res = rfixSqrt(curr);
        float fval = fix2float(curr);
        float expected = 1.0f / sqrtf(fval);
        float err = (expected - fix2float(res)) / expected * 100.0f;

        if (fabsf(err) > fabsf(max_err))
        {
            max_err = err;
            worst_val = fval;
        }
        curr = curr + inc;
    }

    printf("  Worst error: %.4f%% at value %.6f\n", max_err, worst_val);
    ASSERT_MAX_ERR("rfixSqrt sweep max error < 5%", max_err, 5.0f);
    return max_err;
}

// ---- rfixSqrt spot checks ----

void test_rfixSqrt_spot()
{
    printf("\n-- rfixSqrt spot checks --\n");

    struct { float input; float expected; } cases[] = {
        {   1.0f, 1.0f      },  // 1/sqrt(1) = 1
        {   4.0f, 0.5f      },  // 1/sqrt(4) = 0.5
        {  16.0f, 0.25f     },  // 1/sqrt(16) = 0.25
        { 100.0f, 0.1f      },  // 1/sqrt(100) = 0.1
        {   0.25f, 2.0f     },  // 1/sqrt(0.25) = 2
        {   0.01f, 10.0f    },  // 1/sqrt(0.01) = 10
        {   2.0f, 0.70711f  },  // 1/sqrt(2)
    };
    int n = sizeof(cases) / sizeof(cases[0]);

    for (int i = 0; i < n; i++)
    {
        fix15 fx = float2fix(cases[i].input);
        float result = fix2float(rfixSqrt(fx));
        char label[64];
        snprintf(label, sizeof(label), "rfixSqrt(%.4f)", cases[i].input);
        ASSERT_NEAR(label, result, cases[i].expected, fabsf(cases[i].expected) * 0.05f + 0.001f);
    }

    // Edge case: 0 or negative
    ASSERT_EQ("rfixSqrt(0) == 0", rfixSqrt(0), 0);
    ASSERT_EQ("rfixSqrt(-1) == 0", rfixSqrt(float2fix(-1.0)), 0);
}

// ---- fixSqrt spot checks ----

void test_fixSqrt_spot()
{
    printf("\n-- fixSqrt spot checks --\n");

    struct { float input; float expected; } cases[] = {
        {   1.0f,  1.0f   },
        {   4.0f,  2.0f   },
        {  16.0f,  4.0f   },
        { 100.0f, 10.0f   },
        {   0.25f, 0.5f   },
        {   2.0f,  1.4142f},
        {   9.0f,  3.0f   },
    };
    int n = sizeof(cases) / sizeof(cases[0]);

    for (int i = 0; i < n; i++)
    {
        fix15 fx = float2fix(cases[i].input);
        float result = fix2float(fixSqrt(fx));
        char label[64];
        snprintf(label, sizeof(label), "fixSqrt(%.4f)", cases[i].input);
        ASSERT_NEAR(label, result, cases[i].expected, fabsf(cases[i].expected) * 0.05f + 0.01f);
    }
}

// ---- fixSqrt sweep ----

float test_fixSqrt_sweep()
{
    printf("\n-- fixSqrt sweep --\n");

    fix15 curr = float2fix(0.01);
    float max_err = 0;
    float worst_val = 0;

    while (curr < int2fix(65535))
    {
        fix15 inc = (fix15) rand() >> 20;
        if (inc <= 0) inc = 1;

        fix15 res = fixSqrt(curr);
        float fval = fix2float(curr);
        float expected = sqrtf(fval);
        float err = (expected - fix2float(res)) / expected * 100.0f;

        if (fabsf(err) > fabsf(max_err))
        {
            max_err = err;
            worst_val = fval;
        }
        curr = curr + inc;
    }

    printf("  Worst error: %.4f%% at value %.6f\n", max_err, worst_val);
    ASSERT_MAX_ERR("fixSqrt sweep max error < 5%", max_err, 5.0f);
    return max_err;
}

// ---- main ----

int main()
{
    printf("=== Fix.h Test Bench ===\n");

    test_float_conversion();
    test_int_conversion();
    test_multfix();
    test_divfix();
    test_square();
    test_rfixSqrt_spot();
    test_rfixSqrt_sweep();
    test_fixSqrt_spot();
    test_fixSqrt_sweep();

    printf("\n========================\n");
    printf("Results: %d passed, %d failed, %d total\n", tests_passed, tests_failed, tests_run);
    return tests_failed > 0 ? 1 : 0;
}