//
// Created by David.Dai on 2018/10/31.
//

#ifndef ANDORID_GLMATH_HPP
#define ANDORID_GLMATH_HPP

#include <math.h>

struct _Size {
    float width;
    float height;

    _Size(float w = 0, float h = 0){
        width  = w;
        height = h;
    }
};

union _GLVector2  {
    struct { float x, y; };
    struct { float r, g; };
    struct { float s, t; };
    float v[2];
};
typedef union _GLVector2 GLVector2;

union _GLVector3  {
    struct { float x, y, z; };
    struct { float r, g, b; };
    struct { float s, t, p; };
    float v[3];
};
typedef union _GLVector3 GLVector3;

union _GLVector4  {
    struct { float x, y, z, w; };
    struct { float r, g, b, a; };
    struct { float s, t, p, q; };
    float v[4];
};
typedef union _GLVector4 GLVector4;

union _GLMatrix4 {
    struct {
        float m00, m01, m02, m03;
        float m10, m11, m12, m13;
        float m20, m21, m22, m23;
        float m30, m31, m32, m33;
    };
    float m[16];
};
typedef union _GLMatrix4 GLMatrix4;

static GLMatrix4 GLMatrix4Make(float m00, float m01, float m02, float m03,
                          float m10, float m11, float m12, float m13,
                          float m20, float m21, float m22, float m23,
                          float m30, float m31, float m32, float m33) {
    GLMatrix4 m = { m00, m01, m02, m03,
                     m10, m11, m12, m13,
                     m20, m21, m22, m23,
                     m30, m31, m32, m33 };
    return m;
};

static const GLMatrix4 GLMatrix4Identity = GLMatrix4Make(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

static

static GLVector3 GLVector3Make(float x, float y, float z) {
    GLVector3 v = { x, y, z };
    return v;
};

static GLVector4 GLVector4Make(float x, float y, float z, float w) {
    GLVector4 v = { x, y, z, w };
    return v;
}

static float GLVector3Length(GLVector3 vector)  {
    return sqrt(vector.v[0] * vector.v[0] + vector.v[1] * vector.v[1] + vector.v[2] * vector.v[2]);
};

static GLVector3 GLVector3Normalize(GLVector3 vector) {
    float scale = 1.0f / GLVector3Length(vector);
    GLVector3 v = { vector.v[0] * scale, vector.v[1] * scale, vector.v[2] * scale };
    return v;
};

static GLMatrix4 GLMatrix4MakeRotation(float radians, float x, float y, float z) {
    GLVector3 v = GLVector3Normalize(GLVector3Make(x, y, z));
    float cos = cosf(radians);
    float cosp = 1.0f - cos;
    float sin = sinf(radians);

    GLMatrix4 m = { cos + cosp * v.v[0] * v.v[0],
                     cosp * v.v[0] * v.v[1] + v.v[2] * sin,
                     cosp * v.v[0] * v.v[2] - v.v[1] * sin,
                     0.0f,
                     cosp * v.v[0] * v.v[1] - v.v[2] * sin,
                     cos + cosp * v.v[1] * v.v[1],
                     cosp * v.v[1] * v.v[2] + v.v[0] * sin,
                     0.0f,
                     cosp * v.v[0] * v.v[2] + v.v[1] * sin,
                     cosp * v.v[1] * v.v[2] - v.v[0] * sin,
                     cos + cosp * v.v[2] * v.v[2],
                     0.0f,
                     0.0f,
                     0.0f,
                     0.0f,
                     1.0f };

    return m;
};

static GLMatrix4 GLMatrix4MakeScale(float sx, float sy, float sz) {
    GLMatrix4 m = GLMatrix4Identity;
    m.m[0] = sx;
    m.m[5] = sy;
    m.m[10] = sz;
    return m;
};

static GLMatrix4 GLMatrix4MakeTranslation(float tx, float ty, float tz)  {
    GLMatrix4 m = GLMatrix4Identity;
    m.m[12] = tx;
    m.m[13] = ty;
    m.m[14] = tz;
    return m;
};

static GLMatrix4 GLMatrix4MakeOrtho(float left, float right,
                                    float bottom, float top,
                                    float nearZ, float farZ) {
    float ral = right + left;
    float rsl = right - left;
    float tab = top + bottom;
    float tsb = top - bottom;
    float fan = farZ + nearZ;
    float fsn = farZ - nearZ;

    GLMatrix4 m = { 2.0f / rsl, 0.0f, 0.0f, 0.0f,
                    0.0f, 2.0f / tsb, 0.0f, 0.0f,
                    0.0f, 0.0f, -2.0f / fsn, 0.0f,
                    -ral / rsl, -tab / tsb, -fan / fsn, 1.0f };

    return m;
};

static GLMatrix4 GLMatrix4Multiply(GLMatrix4 matrixLeft, GLMatrix4 matrixRight) {
    GLMatrix4 m;

    m.m[0]  = matrixLeft.m[0] * matrixRight.m[0]  + matrixLeft.m[4] * matrixRight.m[1]  + matrixLeft.m[8] * matrixRight.m[2]   + matrixLeft.m[12] * matrixRight.m[3];
    m.m[4]  = matrixLeft.m[0] * matrixRight.m[4]  + matrixLeft.m[4] * matrixRight.m[5]  + matrixLeft.m[8] * matrixRight.m[6]   + matrixLeft.m[12] * matrixRight.m[7];
    m.m[8]  = matrixLeft.m[0] * matrixRight.m[8]  + matrixLeft.m[4] * matrixRight.m[9]  + matrixLeft.m[8] * matrixRight.m[10]  + matrixLeft.m[12] * matrixRight.m[11];
    m.m[12] = matrixLeft.m[0] * matrixRight.m[12] + matrixLeft.m[4] * matrixRight.m[13] + matrixLeft.m[8] * matrixRight.m[14]  + matrixLeft.m[12] * matrixRight.m[15];

    m.m[1]  = matrixLeft.m[1] * matrixRight.m[0]  + matrixLeft.m[5] * matrixRight.m[1]  + matrixLeft.m[9] * matrixRight.m[2]   + matrixLeft.m[13] * matrixRight.m[3];
    m.m[5]  = matrixLeft.m[1] * matrixRight.m[4]  + matrixLeft.m[5] * matrixRight.m[5]  + matrixLeft.m[9] * matrixRight.m[6]   + matrixLeft.m[13] * matrixRight.m[7];
    m.m[9]  = matrixLeft.m[1] * matrixRight.m[8]  + matrixLeft.m[5] * matrixRight.m[9]  + matrixLeft.m[9] * matrixRight.m[10]  + matrixLeft.m[13] * matrixRight.m[11];
    m.m[13] = matrixLeft.m[1] * matrixRight.m[12] + matrixLeft.m[5] * matrixRight.m[13] + matrixLeft.m[9] * matrixRight.m[14]  + matrixLeft.m[13] * matrixRight.m[15];

    m.m[2]  = matrixLeft.m[2] * matrixRight.m[0]  + matrixLeft.m[6] * matrixRight.m[1]  + matrixLeft.m[10] * matrixRight.m[2]  + matrixLeft.m[14] * matrixRight.m[3];
    m.m[6]  = matrixLeft.m[2] * matrixRight.m[4]  + matrixLeft.m[6] * matrixRight.m[5]  + matrixLeft.m[10] * matrixRight.m[6]  + matrixLeft.m[14] * matrixRight.m[7];
    m.m[10] = matrixLeft.m[2] * matrixRight.m[8]  + matrixLeft.m[6] * matrixRight.m[9]  + matrixLeft.m[10] * matrixRight.m[10] + matrixLeft.m[14] * matrixRight.m[11];
    m.m[14] = matrixLeft.m[2] * matrixRight.m[12] + matrixLeft.m[6] * matrixRight.m[13] + matrixLeft.m[10] * matrixRight.m[14] + matrixLeft.m[14] * matrixRight.m[15];

    m.m[3]  = matrixLeft.m[3] * matrixRight.m[0]  + matrixLeft.m[7] * matrixRight.m[1]  + matrixLeft.m[11] * matrixRight.m[2]  + matrixLeft.m[15] * matrixRight.m[3];
    m.m[7]  = matrixLeft.m[3] * matrixRight.m[4]  + matrixLeft.m[7] * matrixRight.m[5]  + matrixLeft.m[11] * matrixRight.m[6]  + matrixLeft.m[15] * matrixRight.m[7];
    m.m[11] = matrixLeft.m[3] * matrixRight.m[8]  + matrixLeft.m[7] * matrixRight.m[9]  + matrixLeft.m[11] * matrixRight.m[10] + matrixLeft.m[15] * matrixRight.m[11];
    m.m[15] = matrixLeft.m[3] * matrixRight.m[12] + matrixLeft.m[7] * matrixRight.m[13] + matrixLeft.m[11] * matrixRight.m[14] + matrixLeft.m[15] * matrixRight.m[15];

    return m;
};

static GLVector4 GLMatrix4MultiplyVector4(GLMatrix4 matrixLeft, GLVector4 vectorRight) {
    GLVector4 v = { matrixLeft.m[0] * vectorRight.v[0] + matrixLeft.m[4] * vectorRight.v[1] + matrixLeft.m[8] * vectorRight.v[2] + matrixLeft.m[12] * vectorRight.v[3],
                    matrixLeft.m[1] * vectorRight.v[0] + matrixLeft.m[5] * vectorRight.v[1] + matrixLeft.m[9] * vectorRight.v[2] + matrixLeft.m[13] * vectorRight.v[3],
                    matrixLeft.m[2] * vectorRight.v[0] + matrixLeft.m[6] * vectorRight.v[1] + matrixLeft.m[10] * vectorRight.v[2] + matrixLeft.m[14] * vectorRight.v[3],
                    matrixLeft.m[3] * vectorRight.v[0] + matrixLeft.m[7] * vectorRight.v[1] + matrixLeft.m[11] * vectorRight.v[2] + matrixLeft.m[15] * vectorRight.v[3] };
    return v;
};

static GLVector3 GLMatrix4MultiplyVector3WithTranslation(GLMatrix4 matrixLeft, GLVector3 vectorRight) {
    GLVector4 v4 = GLMatrix4MultiplyVector4(matrixLeft, GLVector4Make(vectorRight.v[0], vectorRight.v[1], vectorRight.v[2], 1.0f));
    return GLVector3Make(v4.v[0], v4.v[1], v4.v[2]);
};

#endif //ANDORID_GLMATH_HPP
