#pragma once

#include "common/Standard.h"

typedef struct Vec3
{
	float x;
	float y;
	float z;
} Vec3;
static const Vec3 Vec3_zero = { 0, 0, 0 };
static const Vec3 Vec3_one = { 1, 1, 1 };
static const Vec3 Vec3_unitX = { 1, 0, 0 };
static const Vec3 Vec3_unitY = { 0, 1, 0 };
static const Vec3 Vec3_unitZ = { 0, 0, 1 };

typedef struct Vec4
{
	float x;
	float y;
	float z;
	float w;
} Vec4;
static const Vec4 Vec4_zero = { 0, 0, 0, 0 };
static const Vec4 Vec4_one = { 1, 1, 1, 1 };

typedef struct Vec2
{
	float x;
	float y;
} Vec2;
static const Vec2 Vec2_zero = { 0, 0 };
static const Vec2 Vec2_one = { 1, 1 };

typedef struct IVec3
{
	int x;
	int y;
	int z;
} IVec3;
static const IVec3 IVec3_zero = { 0, 0, 0 };
static const IVec3 IVec3_one = { 1, 1, 1 };

typedef struct IVec2
{
	int x;
	int y;
} IVec2;
static const IVec2 IVec2_zero = { 0, 0 };
static const IVec2 IVec2_one = { 1, 1 };

typedef union Matrix3
{
	float values[3][3];
	struct
	{
		Vec3 x;
		Vec3 y;
		Vec3 z;
	};
} Matrix3;
static const Matrix3 Matrix3_zero = {
	{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}
};
static const Matrix3 Matrix3_identity = {
	{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}
};

typedef union Matrix4
{
	float values[4][4];
	struct
	{
		Vec4 x;
		Vec4 y;
		Vec4 z;
		Vec4 w;
	};
} Matrix4;
static const Matrix4 Matrix4_zero = {
	{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
};
static const Matrix4 Matrix4_identity = {
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}
};

typedef struct Transformer
{
	Matrix3 mat;
	Vec3 pos;
} Transformer;
static const Transformer Transformer_zero = {
	{
		{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}
	},
	{0, 0, 0}
};
static const Transformer Transformer_identity = {
	{
		{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}
	},
	{0, 0, 0}
};

typedef struct Plane
{
	Vec3 dir;
	float dist;
} Plane;

static inline Vec3 Vec3_New(float x, float y, float z)
{
	return (Vec3) { x, y, z };
}

static inline Vec4 Vec4_New(float x, float y, float z, float w)
{
	return (Vec4) { x, y, z, w };
}

static inline Vec2 Vec2_New(float x, float y)
{
	return (Vec2) { x, y };
}

static inline IVec3 IVec3_New(int x, int y, int z)
{
	return (IVec3) { x, y, z };
}

static inline IVec2 IVec2_New(int x, int y)
{
	return (IVec2) { x, y };
}

Vec3 Vec3_Add(const Vec3 v0, const Vec3 v1);
Vec3 Vec3_Sub(const Vec3 v0, const Vec3 v1);
Vec3 Vec3_Mul(const Vec3 v0, const Vec3 v1);
Vec3 Vec3_MulF(const Vec3 v0, float s);
Vec3 Vec3_Div(const Vec3 v0, const Vec3 v1);
Vec3 Vec3_DivF(const Vec3 v0, float s);
Vec3 Vec3_Negate(const Vec3 v);
Vec3 Vec3_Lerp(const Vec3 from, const Vec3 to, float factor);

float Vec3_Dot(const Vec3 v0, const Vec3 v1);
Vec3 Vec3_Cross(const Vec3 v0, const Vec3 v1);
float Vec3_SqrLength(const Vec3 v);
float Vec3_Length(const Vec3 v);
Vec3 Vec3_NormalizeGetLength(const Vec3 v, float* outLength);
Vec3 Vec3_Normalize(const Vec3 v);
Vec3 Vec3_Project(const Vec3 v, const Vec3 normal);

// creates unit vector pointing in a direction specified by yaw and pitch.
// yaw is rotated clockwise viewed from the top.
// pitch rotates up with positive values.
// x forward, y right, z up.
Vec3 Vec3_FromAnglesYawPitch(float yaw, float pitch);

Plane Plane_New(const Vec3 dir, float dist);
Plane Plane_NewFromPoint(const Vec3 dir, const Vec3 pos);
Plane Plane_NewFromPoints(const Vec3 pos0, const Vec3 pos1, const Vec3 pos2);
float Plane_DistToPoint(const Plane self, const Vec3 pos);
bool Plane_RayIntersect(const Plane plane, const Vec3 rayOrigin, const Vec3 rayDir, float* outDist);

Matrix3 Matrix3_FromAxisAngle(const Vec3 axis, float angle);
Matrix3 Matrix3_Transpose(const Matrix3* matrix);
Matrix3 Matrix3_Inverse(const Matrix3* matrix);
Vec3 Matrix3_ApplyPoint(const Matrix3* matrix, const Vec3 point);
Vec3 Matrix3_ApplyPointTransposed(const Matrix3* matrix, const Vec3 point);
Matrix3 Matrix3_Multiply(const Matrix3* a, const Matrix3* b);

Matrix4 Matrix4_Transpose(const Matrix4* matrix);
Matrix4 Matrix4_Inverse(const Matrix4* matrix);
Vec3 Matrix4_ApplyPoint(const Matrix4* mat, const Vec3 point);
Vec4 Matrix4_ApplyPoint4(const Matrix4* mat, const Vec4 point);
Matrix4 Matrix4_Multiply(const Matrix4* a, const Matrix4* b);
Matrix4 Matrix4_CreatePerspectiveMatrix(float fov, float aspect, float, float);

Transformer Transformer_Inverse(const Transformer* transformer);
Vec3 Transformer_ApplyPoint(const Transformer* transformer, const Vec3 point);
Vec3 Transformer_ApplyPointTransposed(const Transformer* transformer, const Vec3 point);
Transformer Transformer_Multiply(const Transformer* a, const Transformer* b);
Matrix4 Transformer_ToMatrix4(const Transformer* transformer);
Transformer Transformer_FromMatrix4(const Matrix4* matrix);
