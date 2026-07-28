#include "types.hpp"

Vec3 Multiply(const Mat3& A, const Vec3& b) {
    float x = Dot(A.a, b);
    float y = Dot(A.b, b);
    float z = Dot(A.c, b);

    return {x, y, z};
}

Mat3 Multiply(const Mat3& A, const Mat3& B) {
    float c00 = A.a.x * B.a.x + A.a.y * B.b.x + A.a.z * B.c.x;
    float c01 = A.a.x * B.a.y + A.a.y * B.b.y + A.a.z * B.c.y;
    float c02 = A.a.x * B.a.z + A.a.y * B.b.z + A.a.z * B.c.z;

    float c10 = A.b.x * B.a.x + A.b.y * B.b.x + A.b.z * B.c.x;
    float c11 = A.b.x * B.a.y + A.b.y * B.b.y + A.b.z * B.c.y;
    float c12 = A.b.x * B.a.z + A.b.y * B.b.z + A.b.z * B.c.z;

    float c20 = A.c.x * B.a.x + A.c.y * B.b.x + A.c.z * B.c.x;
    float c21 = A.c.x * B.a.y + A.c.y * B.b.y + A.c.z * B.c.y;
    float c22 = A.c.x * B.a.z + A.c.y * B.b.z + A.c.z * B.c.z;

    return {
            {c00, c01, c02},
            {c10, c11, c12},
            {c20, c21, c22}
    };
}

float Dot(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
