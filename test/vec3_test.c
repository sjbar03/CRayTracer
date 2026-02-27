#include "../lib/Vec3.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// ---- test infrastructure ----

static int tests_run    = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define PASS(name) do { tests_run++; tests_passed++; printf("  PASS  %s\n", name); } while(0)
#define FAIL(name, fmt, ...) do { tests_run++; tests_failed++; printf("  FAIL  %s: " fmt "\n", name, __VA_ARGS__); } while(0)

#define PRINT_VEC(v) do { \
    printf("( %f, %f, %f )\n", fix2float(v.x), fix2float(v.y), fix2float(v.z)); \
} while(0);

#define ASSERT_NEAR(name, actual, expected, tol) do { \
    float _a = (actual), _e = (expected), _t = (tol); \
    if (fabsf(_a - _e) <= _t) { PASS(name); } \
    else { FAIL(name, "got %f, expected %f (tol %f)", _a, _e, _t); } \
} while(0)

#define ASSERT_VEC_NEAR(name, v, ex, ey, ez, tol) do { \
    char _l[80]; \
    snprintf(_l, sizeof(_l), "%s .x", name); ASSERT_NEAR(_l, fix2float((v).x), ex, tol); \
    snprintf(_l, sizeof(_l), "%s .y", name); ASSERT_NEAR(_l, fix2float((v).y), ey, tol); \
    snprintf(_l, sizeof(_l), "%s .z", name); ASSERT_NEAR(_l, fix2float((v).z), ez, tol); \
} while(0)

// helper to build a Vec3 from floats
static Vec3 v3(float x, float y, float z)
{
    Vec3 v;
    v.x = float2fix(x);
    v.y = float2fix(y);
    v.z = float2fix(z);
    return v;
}

// ---- addVec ----

void test_addVec()
{
    printf("\n-- addVec --\n");

    Vec3 a, b, r;

    // basic addition
    a = v3(1.0f, 2.0f, 3.0f);
    b = v3(4.0f, 5.0f, 6.0f);
    addVec(&r, &a, &b);
    ASSERT_VEC_NEAR("(1,2,3)+(4,5,6)", r, 5.0f, 7.0f, 9.0f, 0.01f);

    // addition with negatives
    a = v3(-1.0f, 0.0f, 3.5f);
    b = v3(1.0f, -2.0f, -3.5f);
    addVec(&r, &a, &b);
    ASSERT_VEC_NEAR("(-1,0,3.5)+(1,-2,-3.5)", r, 0.0f, -2.0f, 0.0f, 0.01f);

    // addition with zeros
    a = v3(0.0f, 0.0f, 0.0f);
    b = v3(7.0f, -3.0f, 0.5f);
    addVec(&r, &a, &b);
    ASSERT_VEC_NEAR("(0,0,0)+(7,-3,0.5)", r, 7.0f, -3.0f, 0.5f, 0.01f);
}

// ---- accVec ----

void test_accVec()
{
    printf("\n-- accVec --\n");

    Vec3 a, b;

    // basic accumulation
    a = v3(1.0f, 2.0f, 3.0f);
    b = v3(10.0f, 20.0f, 30.0f);
    accVec(&a, &b);
    ASSERT_VEC_NEAR("(1,2,3)+=(10,20,30)", a, 11.0f, 22.0f, 33.0f, 0.01f);

    // accumulate negative
    a = v3(5.0f, 5.0f, 5.0f);
    b = v3(-5.0f, -5.0f, -5.0f);
    accVec(&a, &b);
    ASSERT_VEC_NEAR("(5,5,5)+=(-5,-5,-5)", a, 0.0f, 0.0f, 0.0f, 0.01f);

    // double accumulate
    a = v3(1.0f, 1.0f, 1.0f);
    b = v3(0.5f, 0.5f, 0.5f);
    accVec(&a, &b);
    accVec(&a, &b);
    ASSERT_VEC_NEAR("(1,1,1)+=2*(0.5,0.5,0.5)", a, 2.0f, 2.0f, 2.0f, 0.01f);
}

// ---- dot ----

void test_dot()
{
    printf("\n-- dot --\n");

    Vec3 a, b;

    // basic dot
    a = v3(1.0f, 2.0f, 3.0f);
    b = v3(4.0f, 5.0f, 6.0f);
    // 1*4 + 2*5 + 3*6 = 32
    ASSERT_NEAR("(1,2,3).(4,5,6)", fix2float(dot(&a, &b)), 32.0f, 0.1f);

    // orthogonal vectors -> 0
    a = v3(1.0f, 0.0f, 0.0f);
    b = v3(0.0f, 1.0f, 0.0f);
    ASSERT_NEAR("x.y orthogonal", fix2float(dot(&a, &b)), 0.0f, 0.001f);

    // self-dot = magnitude squared
    a = v3(3.0f, 4.0f, 0.0f);
    ASSERT_NEAR("(3,4,0).(3,4,0)=25", fix2float(dot(&a, &a)), 25.0f, 0.1f);

    // anti-parallel
    a = v3(1.0f, 1.0f, 1.0f);
    b = v3(-1.0f, -1.0f, -1.0f);
    ASSERT_NEAR("parallel opposite = -3", fix2float(dot(&a, &b)), -3.0f, 0.1f);

    // fractional
    a = v3(0.5f, 0.5f, 0.5f);
    b = v3(2.0f, 2.0f, 2.0f);
    ASSERT_NEAR("(0.5,0.5,0.5).(2,2,2)=3", fix2float(dot(&a, &b)), 3.0f, 0.1f);
}

// ---- cross ----

void test_cross()
{
    printf("\n-- cross --\n");

    Vec3 a, b, r;

    // x cross y = z
    a = v3(1.0f, 0.0f, 0.0f);
    b = v3(0.0f, 1.0f, 0.0f);
    cross(&r, &a, &b);
    ASSERT_VEC_NEAR("x cross y = z", r, 0.0f, 0.0f, 1.0f, 0.01f);

    // y cross x = -z
    cross(&r, &b, &a);
    ASSERT_VEC_NEAR("y cross x = -z", r, 0.0f, 0.0f, -1.0f, 0.01f);

    // y cross z = x
    a = v3(0.0f, 1.0f, 0.0f);
    b = v3(0.0f, 0.0f, 1.0f);
    cross(&r, &a, &b);
    ASSERT_VEC_NEAR("y cross z = x", r, 1.0f, 0.0f, 0.0f, 0.01f);

    // parallel vectors -> zero
    a = v3(2.0f, 2.0f, 2.0f);
    b = v3(4.0f, 4.0f, 4.0f);
    cross(&r, &a, &b);
    ASSERT_VEC_NEAR("parallel cross = 0", r, 0.0f, 0.0f, 0.0f, 0.01f);

    // general case: (1,2,3) x (4,5,6) = (-3, 6, -3)
    a = v3(1.0f, 2.0f, 3.0f);
    b = v3(4.0f, 5.0f, 6.0f);
    cross(&r, &a, &b);
    ASSERT_VEC_NEAR("(1,2,3)x(4,5,6)", r, -3.0f, 6.0f, -3.0f, 0.1f);
}

// ---- L2 ----

void test_L2()
{
    printf("\n-- L2 --\n");

    Vec3 a;

    // 3-4-5 triangle
    a = v3(3.0f, 4.0f, 0.0f);
    ASSERT_NEAR("|(3,4,0)| = 5", fix2float(L2(&a)), 5.0f, 0.5f);

    // unit vector
    a = v3(1.0f, 0.0f, 0.0f);
    ASSERT_NEAR("|(1,0,0)| = 1", fix2float(L2(&a)), 1.0f, 0.1f);

    // (1,1,1) -> sqrt(3) ≈ 1.732
    a = v3(1.0f, 1.0f, 1.0f);
    ASSERT_NEAR("|(1,1,1)| = sqrt(3)", fix2float(L2(&a)), 1.732f, 0.2f);

    // zero vector
    a = v3(0.0f, 0.0f, 0.0f);
    ASSERT_NEAR("|(0,0,0)| = 0", fix2float(L2(&a)), 0.0f, 0.01f);

    // larger vector
    a = v3(10.0f, 20.0f, 30.0f);
    float expected = sqrtf(10*10 + 20*20 + 30*30); // 37.417
    ASSERT_NEAR("|(10,20,30)|", fix2float(L2(&a)), expected, expected * 0.05f);
}

// ---- normalize (in-place) ----

void test_normalize()
{
    printf("\n-- normalize --\n");

    Vec3 a;
    float len;

    // 3-4-0 -> (0.6, 0.8, 0)
    a = v3(3.0f, 4.0f, 0.0f);
    normalize(&a);
    ASSERT_VEC_NEAR("norm(3,4,0)", a, 0.6f, 0.8f, 0.0f, 0.05f);

    // unit vector stays unit
    a = v3(1.0f, 0.0f, 0.0f);
    normalize(&a);
    ASSERT_VEC_NEAR("norm(1,0,0)", a, 1.0f, 0.0f, 0.0f, 0.05f);

    // (1,1,1) -> (1/sqrt3, 1/sqrt3, 1/sqrt3)
    a = v3(1.0f, 1.0f, 1.0f);
    normalize(&a);
    float c = 1.0f / sqrtf(3.0f);
    ASSERT_VEC_NEAR("norm(1,1,1)", a, c, c, c, 0.05f);

    // negative components
    a = v3(-3.0f, 4.0f, 0.0f);
    normalize(&a);
    ASSERT_VEC_NEAR("norm(-3,4,0)", a, -0.6f, 0.8f, 0.0f, 0.05f);

    // verify length ≈ 1 after normalize
    a = v3(5.0f, 12.0f, 0.0f);
    normalize(&a);
    len = fix2float(dot(&a, &a));
    ASSERT_NEAR("norm(5,12,0) len=1", len, 1.0f, 0.1f);
}

// ---- normalizeTo ----

void test_normalizeTo()
{
    printf("\n-- normalizeTo --\n");

    Vec3 a, r;
    float len;

    // 3-4-0
    a = v3(3.0f, 4.0f, 0.0f);
    normalizeTo(&a, &r);
    ASSERT_VEC_NEAR("normTo(3,4,0)", r, 0.6f, 0.8f, 0.0f, 0.05f);

    // verify original unchanged
    ASSERT_VEC_NEAR("orig unchanged", a, 3.0f, 4.0f, 0.0f, 0.01f);

    // (1,1,1)
    a = v3(1.0f, 1.0f, 1.0f);
    normalizeTo(&a, &r);
    float c = 1.0f / sqrtf(3.0f);
    ASSERT_VEC_NEAR("normTo(1,1,1)", r, c, c, c, 0.05f);

    // verify length ≈ 1
    len = fix2float(dot(&r, &r));
    ASSERT_NEAR("normTo len=1", len, 1.0f, 0.1f);
}

void test_findOrthogonal()
{
    printf("\n-- findOrthogonal --\n");

    Vec3 a, r;
    fix15 res;

    a = v3(1.0f, 0.0f, 0.0f);
    findOrthogonal(&r, &a);
    res = dot(&a, &r);
    if (res == 0) {PASS("Orthogonal (1.0, 0, 0)"); }
    else { FAIL("Orthogonal (1.0, 0, 0)", "got %f expected %f", fix2float(res), 0.0f); }

    a = v3(0.0f, 1.0f, 0.0f);
    findOrthogonal(&r, &a);
    res = dot(&a, &r);
    if (res == 0) {PASS("Orthogonal (0, 1.0, 0)"); }
    else { FAIL("Orthogonal (0, 1.0, 0)", "got %f expected %f", fix2float(res), 0.0f); }

    a = v3(0.0f, 0.0f, 1.0f);
    findOrthogonal(&r, &a);
    res = dot(&a, &r);
    if (res == 0) {PASS("Orthogonal (0, 0, 1.0)"); }
    else { FAIL("Orthogonal (0, 0, 1.0)", "got %f expected %f", fix2float(res), 0.0f); }

    a = v3( 0.512909f, 8620.460938f, 49519.351562f );
    findOrthogonal(&r, &a);
    res = dot(&a, &r);
    if (res == 0) { PASS("Orthogonal (0.512909f, 8620.460938f, 49519.351562f)"); }
    else { 
        FAIL("Orthogonal (0.512909f, 8620.460938f, 49519.351562f)", "got (%f, %f, %f) with dot %f", fix2float(r.x), fix2float(r.y), fix2float(r.z), fix2float(res));
    }

    int passed = 0;
    int failed = 0;
    Vec3 failed_arr[100];

    for(int i = 0; i < 100; i++)
    {
        a = (Vec3) {randomFixAll(), randomFixAll(), randomFixAll()};
        findOrthogonal(&r, &a);
        res = dot(&a, &r);
        if (abs(res) <= float2fix(0.0005)) { passed ++ ; }
        else { failed_arr[failed] = a; failed++ ; };
    }

    if(failed == 0) {
        PASS("Random Orthogonal");
    }
    else {
        FAIL("Random Orthogonal", "Failed %d random tests. ", failed); 
        for (int i = 0; i < failed; i++)
        {
            PRINT_VEC(failed_arr[i]);
        }
    }
}

// ---- main ----

int main()
{
    printf("=== Vec3 Test Bench ===\n");

    test_addVec();
    test_accVec();
    test_dot();
    test_cross();
    test_L2();
    test_normalize();
    test_normalizeTo();
    test_findOrthogonal();

    printf("\n========================\n");
    printf("Results: %d passed, %d failed, %d total\n", tests_passed, tests_failed, tests_run);
    return tests_failed > 0 ? 1 : 0;
}
