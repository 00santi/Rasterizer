#include "types.hpp"

Vec3 Multiply(Mat3 A, Vec3 b) {
    float x = A.a.x * b.x + A.a.y * b.y + A.a.z * b.z;
    float y = A.b.x * b.x + A.b.y * b.y + A.b.z * b.z;
    float z = A.c.x * b.x + A.c.y * b.y + A.c.z * b.z;

    return {x, y, z};
}

Mat3 Multiply(Mat3 a, Mat3 b) {
    float c00 = a.a.x * b.a.x + a.a.y * b.b.x + a.a.z * b.c.x;
    float c01 = a.a.x * b.a.y + a.a.y * b.b.y + a.a.z * b.c.y;
    float c02 = a.a.x * b.a.z + a.a.y * b.b.z + a.a.z * b.c.z;

    float c10 = a.b.x * b.a.x + a.b.y * b.b.x + a.b.z * b.c.x;
    float c11 = a.b.x * b.a.y + a.b.y * b.b.y + a.b.z * b.c.y;
    float c12 = a.b.x * b.a.z + a.b.y * b.b.z + a.b.z * b.c.z;

    float c20 = a.c.x * b.a.x + a.c.y * b.b.x + a.c.z * b.c.x;
    float c21 = a.c.x * b.a.y + a.c.y * b.b.y + a.c.z * b.c.y;
    float c22 = a.c.x * b.a.z + a.c.y * b.b.z + a.c.z * b.c.z;

    return {
            {c00, c01, c02},
            {c10, c11, c12},
            {c20, c21, c22}
    };
}
