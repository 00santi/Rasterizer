#include "types.hpp"

Vec3 Multiply(Mat3 a, Vec3 b) {
    float x = a[0][0] * b.x + a[0][1] * b.y + a[0][2] * b.z;
    float y = a[1][0] * b.x + a[1][1] * b.y + a[1][2] * b.z;
    float z = a[2][0] * b.x + a[2][1] * b.y + a[2][2] * b.z;

    return {x, y, z};
}

Mat3 id = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
};

Mat3 scale2 = {
    {2, 0, 0},
    {0, 2, 0},
    {0, 0, 2},
};

// to move point p = (p1, p2) by vector t = (t1, t2)
float t1 = 10;
float t2 = 20;
Mat3 transformation = {
    {1, 0, 10},
    {0, 1, 20},
    {0, 0, 1},
};
// (p1, p2, 1) * transformation
