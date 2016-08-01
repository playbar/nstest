#ifndef __MAT4_H_INCLUDED__
#define __MAT4_H_INCLUDED__

#include "utility.h"

struct kmVec3;
struct kmMat3;
struct kmQuaternion;
struct kmPlane;

/*
A 4x4 matrix

        | 0   4   8  12 |
mat =   | 1   5   9  13 |
        | 2   6  10  14 |
        | 3   7  11  15 |
*/

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kmMat4 
{
    float mat[16];
} kmMat4;

kmMat4* const kmMat4Fill(kmMat4* pOut, const float* pMat);


kmMat4* const kmMat4Identity(kmMat4* pOut);

kmMat4* const kmMat4Inverse(kmMat4* pOut, const kmMat4* pM);


const int kmMat4IsIdentity(const kmMat4* pIn);

kmMat4* const kmMat4Transpose(kmMat4* pOut, const kmMat4* pIn);
kmMat4* const kmMat4Multiply(kmMat4* pOut, const kmMat4* pM1, const kmMat4* pM2);

kmMat4* const kmMat4Assign(kmMat4* pOut, const kmMat4* pIn);
const int kmMat4AreEqual(const kmMat4* pM1, const kmMat4* pM2);

kmMat4* const kmMat4RotationX(kmMat4* pOut, const float radians);
kmMat4* const kmMat4RotationY(kmMat4* pOut, const float radians);
kmMat4* const kmMat4RotationZ(kmMat4* pOut, const float radians);
kmMat4* const kmMat4RotationPitchYawRoll(kmMat4* pOut, const float pitch, const float yaw, const float roll);
kmMat4* const kmMat4RotationQuaternion(kmMat4* pOut, const struct kmQuaternion* pQ);
kmMat4* const kmMat4RotationTranslation(kmMat4* pOut, const struct kmMat3* rotation, const struct kmVec3* translation);
kmMat4* const kmMat4Scaling(kmMat4* pOut, const float x, const float y, const float z);
kmMat4* const kmMat4Translation(kmMat4* pOut, const float x, const float y, const float z);

struct kmVec3* const kmMat4GetUpVec3(struct kmVec3* pOut, const kmMat4* pIn);
struct kmVec3* const kmMat4GetRightVec3(struct kmVec3* pOut, const kmMat4* pIn);
struct kmVec3* const kmMat4GetForwardVec3(struct kmVec3* pOut, const kmMat4* pIn);

kmMat4* const kmMat4PerspectiveProjection(kmMat4* pOut, float fovY, float aspect, float zNear, float zFar);
kmMat4* const kmMat4OrthographicProjection(kmMat4* pOut, float left, float right, float bottom, float top, float nearVal, float farVal);
kmMat4* const kmMat4LookAt(kmMat4* pOut, const struct kmVec3* pEye, const struct kmVec3* pCenter, const struct kmVec3* pUp);

kmMat4* const kmMat4RotationAxisAngle(kmMat4* pOut, const struct kmVec3* axis, float radians);
struct kmMat3* const kmMat4ExtractRotation(struct kmMat3* pOut, const kmMat4* pIn);
struct kmPlane* const kmMat4ExtractPlane(struct kmPlane* pOut, const kmMat4* pIn, const kmEnum plane);
struct kmVec3* const kmMat4RotationToAxisAngle(struct kmVec3* pAxis, float* radians, const kmMat4* pIn);

#ifdef __cplusplus
}
#endif

#endif /* MAT4_H_INCLUDED */
