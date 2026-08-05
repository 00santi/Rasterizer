#pragma once
#include <cstdint>

using u8 = std::uint8_t;

struct Color {
    u8 r = 0;
    u8 g = 0;
    u8 b = 0;
    u8 a = 0;
};

inline Color operator*(const float weight, const Color& c) {
    return {
        static_cast<u8>(c.r * weight),
        static_cast<u8>(c.g * weight),
        static_cast<u8>(c.b * weight),
        static_cast<u8>(c.a * weight),
    };
}

inline Color operator*(const Color& c, const float weight) {
    return weight * c;
}

inline Color operator+(const Color& c1, const Color& c2) {
    return {
        static_cast<u8>(c1.r + c2.r),
        static_cast<u8>(c1.g + c2.g),
        static_cast<u8>(c1.b + c2.b),
        static_cast<u8>(c1.a + c2.a),
    };
}

struct Point2 {
    float x = 0;
    float y = 0;
};

struct Point3 {
    float x = 0;
    float y = 0;
    float z = 0;
};

struct Vec3 {
    float x = 0;
    float y = 0;
    float w = 1;
};

struct Vec4 {
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 1;
};

struct Vertex3 {
    Vec3 position {};
    Color color {};
};

struct Vertex4 {
    Vec4 position {};
    Color color {};
};

struct Triangle {
    Vertex3 a {};
    Vertex3 b {};
    Vertex3 c {};
};

struct Mat3 {
    Vec3 a {};
    Vec3 b {};
    Vec3 c {};
};

struct Mat4 {
    Vec4 a {};
    Vec4 b {};
    Vec4 c {};
    Vec4 d {};
};

using Transform2D = Mat3;
using Transform3D = Mat4;

Vec3 Multiply(const Mat3& mat, const Vec3& vec);
Mat3 Multiply(const Mat3& A, const Mat3& B);
float Dot(const Vec3& a, const Vec3& b);

inline Mat3 operator*(const Mat3& A, const Mat3& B) {
    return Multiply(A, B);
}

inline Vec3 operator*(const Mat3& A, const Vec3& b) {
    return Multiply(A, b);
}

Vec4 Multiply(const Mat4& mat, const Vec4& vec);
Mat4 Multiply(const Mat4& A, const Mat4& B);
float Dot(const Vec4& a, const Vec4& b);

inline Mat4 operator*(const Mat4& A, const Mat4& B) {
    return Multiply(A, B);
}

inline Vec4 operator*(const Mat4& A, const Vec4& b) {
    return Multiply(A, b);
}

struct Camera {
    Point3 position {};
};
