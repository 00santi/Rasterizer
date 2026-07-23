#include "types.hpp"

Vec3 Multiply(Mat3 a, Vec3 b) {
    float x = a[0][0] * b.x + a[0][1] * b.y + a[0][2] * b.z;
    float y = a[1][0] * b.x + a[1][1] * b.y + a[1][2] * b.z;
    float z = a[2][0] * b.x + a[2][1] * b.y + a[2][2] * b.z;

    return {x, y, z};
}
