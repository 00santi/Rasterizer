#include "types.hpp"

float Dot(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.w * b.w;
}

Vec3 Multiply(const Mat3& mat, const Vec3& vec) {
    const float a = Dot(mat.a, vec);
    const float bb = Dot(mat.b, vec);
    const float c = Dot(mat.c, vec);

    return {a, bb, c};
}

Mat3 Multiply(const Mat3& A, const Mat3& B) {
    const float c00 = A.a.x * B.a.x + A.a.y * B.b.x + A.a.w * B.c.x;
    const float c01 = A.a.x * B.a.y + A.a.y * B.b.y + A.a.w * B.c.y;
    const float c02 = A.a.x * B.a.w + A.a.y * B.b.w + A.a.w * B.c.w;

    const float c10 = A.b.x * B.a.x + A.b.y * B.b.x + A.b.w * B.c.x;
    const float c11 = A.b.x * B.a.y + A.b.y * B.b.y + A.b.w * B.c.y;
    const float c12 = A.b.x * B.a.w + A.b.y * B.b.w + A.b.w * B.c.w;

    const float c20 = A.c.x * B.a.x + A.c.y * B.b.x + A.c.w * B.c.x;
    const float c21 = A.c.x * B.a.y + A.c.y * B.b.y + A.c.w * B.c.y;
    const float c22 = A.c.x * B.a.w + A.c.y * B.b.w + A.c.w * B.c.w;

    return {
        {c00, c01, c02},
        {c10, c11, c12},
        {c20, c21, c22}
    };
}

float Dot(const Vec4& a, const Vec4& b) {
    return a.x * b.x +
           a.y * b.y +
           a.z * b.z +
           a.w * b.w;
}

Vec4 Multiply(const Mat4& mat, const Vec4& vec) {
    const float a = Dot(mat.a, vec);
    const float b = Dot(mat.b, vec);
    const float c = Dot(mat.c, vec);
    const float d = Dot(mat.d, vec);

    return {a, b, c, d};
}

Mat4 Multiply(const Mat4& A, const Mat4& B) {
    const float c00 = A.a.x * B.a.x + A.a.y * B.b.x + A.a.z * B.c.x + A.a.w * B.d.x;
    const float c01 = A.a.x * B.a.y + A.a.y * B.b.y + A.a.z * B.c.y + A.a.w * B.d.y;
    const float c02 = A.a.x * B.a.z + A.a.y * B.b.z + A.a.z * B.c.z + A.a.w * B.d.z;
    const float c03 = A.a.x * B.a.w + A.a.y * B.b.w + A.a.z * B.c.w + A.a.w * B.d.w;

    const float c10 = A.b.x * B.a.x + A.b.y * B.b.x + A.b.z * B.c.x + A.b.w * B.d.x;
    const float c11 = A.b.x * B.a.y + A.b.y * B.b.y + A.b.z * B.c.y + A.b.w * B.d.y;
    const float c12 = A.b.x * B.a.z + A.b.y * B.b.z + A.b.z * B.c.z + A.b.w * B.d.z;
    const float c13 = A.b.x * B.a.w + A.b.y * B.b.w + A.b.z * B.c.w + A.b.w * B.d.w;

    const float c20 = A.c.x * B.a.x + A.c.y * B.b.x + A.c.z * B.c.x + A.c.w * B.d.x;
    const float c21 = A.c.x * B.a.y + A.c.y * B.b.y + A.c.z * B.c.y + A.c.w * B.d.y;
    const float c22 = A.c.x * B.a.z + A.c.y * B.b.z + A.c.z * B.c.z + A.c.w * B.d.z;
    const float c23 = A.c.x * B.a.w + A.c.y * B.b.w + A.c.z * B.c.w + A.c.w * B.d.w;

    const float c30 = A.d.x * B.a.x + A.d.y * B.b.x + A.d.z * B.c.x + A.d.w * B.d.x;
    const float c31 = A.d.x * B.a.y + A.d.y * B.b.y + A.d.z * B.c.y + A.d.w * B.d.y;
    const float c32 = A.d.x * B.a.z + A.d.y * B.b.z + A.d.z * B.c.z + A.d.w * B.d.z;
    const float c33 = A.d.x * B.a.w + A.d.y * B.b.w + A.d.z * B.c.w + A.d.w * B.d.w;

    return {
        {c00, c01, c02, c03},
        {c10, c11, c12, c13},
        {c20, c21, c22, c23},
        {c30, c31, c32, c33}
    };
}
