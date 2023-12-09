#include "common/Space.h"

#include "common/Math.h"

Vec3 Vec3_Add(const Vec3 v0, const Vec3 v1)
{
	return Vec3_New(v0.x+v1.x, v0.y+v1.y, v0.z+v1.z);
}

Vec3 Vec3_Sub(const Vec3 v0, const Vec3 v1)
{
	return Vec3_New(v0.x-v1.x, v0.y-v1.y, v0.z-v1.z);
}

Vec3 Vec3_Mul(const Vec3 v0, const Vec3 v1)
{
	return Vec3_New(v0.x*v1.x, v0.y*v1.y, v0.z*v1.z);
}

Vec3 Vec3_MulF(const Vec3 v0, float s)
{
	return Vec3_New(v0.x*s, v0.y*s, v0.z*s);
}

Vec3 Vec3_Div(const Vec3 v0, const Vec3 v1)
{
	return Vec3_New(v0.x/v1.x, v0.y/v1.y, v0.z/v1.z);
}

Vec3 Vec3_DivF(const Vec3 v0, float s)
{
	return Vec3_New(v0.x/s, v0.y/s, v0.z/s);
}

Vec3 Vec3_Negate(const Vec3 v)
{
	return Vec3_New(-v.x, -v.y, -v.z);
}

Vec3 Vec3_Lerp(const Vec3 from, const Vec3 to, float factor)
{
	return (Vec3) {
		Lerp(from.x, to.x, factor),
		Lerp(from.y, to.y, factor),
		Lerp(from.z, to.z, factor),
	};
}

float Vec3_Dot(const Vec3 v0, const Vec3 v1)
{
	return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z;
}

Vec3 Vec3_Cross(const Vec3 v0, const Vec3 v1)
{
	return Vec3_New(
		v0.y*v1.z-v0.z*v1.y,
		v0.z*v1.x-v0.x*v1.z,
		v0.x*v1.y-v0.y*v1.x
	);
}

float Vec3_SqrLength(const Vec3 v)
{
	return v.x*v.x+v.y*v.y+v.z*v.z;
}

float Vec3_Length(const Vec3 v)
{
	return Sqrt(Vec3_SqrLength(v));
}

Vec3 Vec3_NormalizeGetLength(const Vec3 v, float* outLength)
{
	Vec3 temp = { 0 };
	float len = Vec3_Length(v);
	if (len > 0)
	{
		temp = Vec3_DivF(v, len);
	}
	*outLength = len;
	return temp;
}

Vec3 Vec3_Normalize(const Vec3 v)
{
	Vec3 temp = {0};
	float len = Vec3_Length(v);
	if (len > 0)
	{
		temp = Vec3_DivF(v, len);
	}
	return temp;
}

Vec3 Vec3_Project(const Vec3 v, const Vec3 normal)
{
	float dot = Vec3_Dot(v, normal);
	Vec3 result = Vec3_MulF(normal, dot);
	return result;
}

Vec3 Vec3_FromAnglesYawPitch(float yaw, float pitch)
{
	Vec3 result;
	float cp = Cos(pitch);
	result.x = Cos(yaw)*cp;
	result.y = Sin(yaw)*cp;
	result.z = Sin(pitch);
	return result;
}

Plane Plane_New(const Vec3 dir, float dist)
{
	Plane self = { dir, dist };
	return self;
}

Plane Plane_NewFromPoint(const Vec3 dir, const Vec3 pos)
{
	return Plane_New(dir, -Vec3_Dot(dir, pos));
}

Plane Plane_NewFromPoints(const Vec3 pos0, const Vec3 pos1, const Vec3 pos2)
{
	Vec3 diff0 = pos1;
	diff0 = Vec3_Sub(diff0, pos0);

	Vec3 diff1 = pos2;
	diff1 = Vec3_Sub(diff1, pos0);

	diff0 = Vec3_Cross(diff1, diff0);

	Plane self = { 0 };
	self.dir = Vec3_Normalize(diff0);
	self.dist = Vec3_Dot(self.dir, pos0);
	return self;
}

float Plane_DistToPoint(const Plane self, const Vec3 pos)
{
	return Vec3_Dot(self.dir, pos)-self.dist;
}

bool Plane_RayIntersect(const Plane plane, const Vec3 rayOrigin, const Vec3 rayDir, float* outDist)
{
	float denom = Vec3_Dot(plane.dir, rayDir);
	if (Abs(denom) < 0.000001f)
	{
		return false;
	}
	*outDist = (-Vec3_Dot(plane.dir, rayOrigin)+plane.dist)/denom;
	return true;
}

Matrix3 Matrix3_FromAxisAngle(const Vec3 axis, float angle)
{
	float cos = Cos(angle);
	float sin = Sin(angle);
	float t = 1.0f-cos;

	float tXX = t*axis.x*axis.x;
	float tXY = t*axis.x*axis.y;
	float tXZ = t*axis.x*axis.z;
	float tYY = t*axis.y*axis.y;
	float tYZ = t*axis.y*axis.z;
	float tZZ = t*axis.z*axis.z;

	float sinX = sin*axis.x;
	float sinY = sin*axis.y;
	float sinZ = sin*axis.z;

	Matrix3 result;
	result.values[0][0] = tXX+cos;
	result.values[0][1] = tXY-sinZ;
	result.values[0][2] = tXZ+sinY;
	result.values[1][0] = tXY+sinZ;
	result.values[1][1] = tYY+cos;
	result.values[1][2] = tYZ-sinX;
	result.values[2][0] = tXZ-sinY;
	result.values[2][1] = tYZ+sinX;
	result.values[2][2] = tZZ+cos;

	return result;
}

Matrix3 Matrix3_Transpose(const Matrix3* matrix)
{
	Matrix3 result;
	for (int32 y = 0; y < 3; y++)
	{
		for (int32 x = 0; x < 3; x++)
		{
			result.values[x][y] = matrix->values[y][x];
		}
	}
	return result;
}

Matrix3 Matrix3_Inverse(const Matrix3* matrix)
{
	Matrix3 result;

	float invRow0X = (+matrix->y.y * matrix->z.z) - (matrix->y.z * matrix->z.y);
	float invRow1X = (-matrix->y.x * matrix->z.z) + (matrix->y.z * matrix->z.x);
	float invRow2X = (+matrix->y.x * matrix->z.y) - (matrix->y.y * matrix->z.x);

	float det = (matrix->x.x * invRow0X) + (matrix->x.y * invRow1X) + (matrix->x.z * invRow2X);

	if (det == 0.0f)
	{
		MemSet(&result, 0, sizeof(Matrix3));
		return result;
	}

	result.x.x = invRow0X;
	result.x.y = (-matrix->x.y * matrix->z.z) + (matrix->x.z * matrix->z.y);
	result.x.z = (+matrix->x.y * matrix->y.z) - (matrix->x.z * matrix->y.y);
	result.y.x = invRow1X;
	result.y.y = (+matrix->x.x * matrix->z.z) - (matrix->x.z * matrix->z.x);
	result.y.z = (-matrix->x.x * matrix->y.z) + (matrix->x.z * matrix->y.x);
	result.z.x = invRow2X;
	result.z.y = (-matrix->x.x * matrix->z.y) + (matrix->x.y * matrix->z.x);
	result.z.z = (+matrix->x.x * matrix->y.y) - (matrix->x.y * matrix->y.x);

	det = 1.0f/det;

	result.x.x *= det;
	result.x.y *= det;
	result.x.z *= det;
	result.y.x *= det;
	result.y.y *= det;
	result.y.z *= det;
	result.z.x *= det;
	result.z.y *= det;
	result.z.z *= det;

	return result;
}

Vec3 Matrix3_ApplyPoint(const Matrix3* matrix, const Vec3 point)
{
	Vec3 result;
	result.x = point.x*matrix->values[0][0]+point.y*matrix->values[1][0]+point.z*matrix->values[2][0];
	result.y = point.x*matrix->values[0][1]+point.y*matrix->values[1][1]+point.z*matrix->values[2][1];
	result.z = point.x*matrix->values[0][2]+point.y*matrix->values[1][2]+point.z*matrix->values[2][2];
	return result;
}

Vec3 Matrix3_ApplyPointTransposed(const Matrix3* matrix, const Vec3 point)
{
	Vec3 result;
	result.x = point.x*matrix->values[0][0]+point.y*matrix->values[0][1]+point.z*matrix->values[0][2];
	result.y = point.x*matrix->values[1][0]+point.y*matrix->values[1][1]+point.z*matrix->values[1][2];
	result.z = point.x*matrix->values[2][0]+point.y*matrix->values[2][1]+point.z*matrix->values[2][2];
	return result;
}

Matrix3 Matrix3_Multiply(const Matrix3* a, const Matrix3* b)
{
	Matrix3 result;
	for (int32 y = 0; y < 3; y++)
	{
		for (int32 x = 0; x < 3; x++)
		{
			result.values[y][x] =
				a->values[y][0]*b->values[0][x]+
				a->values[y][1]*b->values[1][x]+
				a->values[y][2]*b->values[2][x];
		}
	}
	return result;
}

Matrix4 Matrix4_Transpose(const Matrix4* matrix)
{
	Matrix4 result;
	for (int32 y = 0; y < 4; y++)
	{
		for (int32 x = 0; x < 4; x++)
		{
			result.values[x][y] = matrix->values[y][x];
		}
	}
	return result;
}

Matrix4 Matrix4_Inverse(const Matrix4* matrix)
{
	float s[6];
	float c[6];
	s[0] = matrix->values[0][0]*matrix->values[1][1]-matrix->values[1][0]*matrix->values[0][1];
	s[1] = matrix->values[0][0]*matrix->values[1][2]-matrix->values[1][0]*matrix->values[0][2];
	s[2] = matrix->values[0][0]*matrix->values[1][3]-matrix->values[1][0]*matrix->values[0][3];
	s[3] = matrix->values[0][1]*matrix->values[1][2]-matrix->values[1][1]*matrix->values[0][2];
	s[4] = matrix->values[0][1]*matrix->values[1][3]-matrix->values[1][1]*matrix->values[0][3];
	s[5] = matrix->values[0][2]*matrix->values[1][3]-matrix->values[1][2]*matrix->values[0][3];

	c[0] = matrix->values[2][0]*matrix->values[3][1]-matrix->values[3][0]*matrix->values[2][1];
	c[1] = matrix->values[2][0]*matrix->values[3][2]-matrix->values[3][0]*matrix->values[2][2];
	c[2] = matrix->values[2][0]*matrix->values[3][3]-matrix->values[3][0]*matrix->values[2][3];
	c[3] = matrix->values[2][1]*matrix->values[3][2]-matrix->values[3][1]*matrix->values[2][2];
	c[4] = matrix->values[2][1]*matrix->values[3][3]-matrix->values[3][1]*matrix->values[2][3];
	c[5] = matrix->values[2][2]*matrix->values[3][3]-matrix->values[3][2]*matrix->values[2][3];

	float det = s[0]*c[5]-s[1]*c[4]+s[2]*c[3]+s[3]*c[2]-s[4]*c[1]+s[5]*c[0];
	float invDet = 1.0f/det;

	Matrix4 result;
	result.values[0][0] = (matrix->values[1][1]*c[5]-matrix->values[1][2]*c[4]+matrix->values[1][3]*c[3])*invDet;
	result.values[0][1] = (-matrix->values[0][1]*c[5]+matrix->values[0][2]*c[4]-matrix->values[0][3]*c[3])*invDet;
	result.values[0][2] = (matrix->values[3][1]*s[5]-matrix->values[3][2]*s[4]+matrix->values[3][3]*s[3])*invDet;
	result.values[0][3] = (-matrix->values[2][1]*s[5]+matrix->values[2][2]*s[4]-matrix->values[2][3]*s[3])*invDet;

	result.values[1][0] = (-matrix->values[1][0]*c[5]+matrix->values[1][2]*c[2]-matrix->values[1][3]*c[1])*invDet;
	result.values[1][1] = (matrix->values[0][0]*c[5]-matrix->values[0][2]*c[2]+matrix->values[0][3]*c[1])*invDet;
	result.values[1][2] = (-matrix->values[3][0]*s[5]+matrix->values[3][2]*s[2]-matrix->values[3][3]*s[1])*invDet;
	result.values[1][3] = (matrix->values[2][0]*s[5]-matrix->values[2][2]*s[2]+matrix->values[2][3]*s[1])*invDet;

	result.values[2][0] = (matrix->values[1][0]*c[4]-matrix->values[1][1]*c[2]+matrix->values[1][3]*c[0])*invDet;
	result.values[2][1] = (-matrix->values[0][0]*c[4]+matrix->values[0][1]*c[2]-matrix->values[0][3]*c[0])*invDet;
	result.values[2][2] = (matrix->values[3][0]*s[4]-matrix->values[3][1]*s[2]+matrix->values[3][3]*s[0])*invDet;
	result.values[2][3] = (-matrix->values[2][0]*s[4]+matrix->values[2][1]*s[2]-matrix->values[2][3]*s[0])*invDet;

	result.values[3][0] = (-matrix->values[1][0]*c[3]+matrix->values[1][1]*c[1]-matrix->values[1][2]*c[0])*invDet;
	result.values[3][1] = (matrix->values[0][0]*c[3]-matrix->values[0][1]*c[1]+matrix->values[0][2]*c[0])*invDet;
	result.values[3][2] = (-matrix->values[3][0]*s[3]+matrix->values[3][1]*s[1]-matrix->values[3][2]*s[0])*invDet;
	result.values[3][3] = (matrix->values[2][0]*s[3]-matrix->values[2][1]*s[1]+matrix->values[2][2]*s[0])*invDet;

	return result;
}

Vec3 Matrix4_ApplyPoint(const Matrix4* mat, const Vec3 point)
{
	Vec3 result;

	result.x = (point.x*mat->values[0][0])+(point.y*mat->values[1][0])+(point.z*mat->values[2][0])+mat->values[3][0];
	result.y = (point.x*mat->values[0][1])+(point.y*mat->values[1][1])+(point.z*mat->values[2][1])+mat->values[3][1];
	result.z = (point.x*mat->values[0][2])+(point.y*mat->values[1][2])+(point.z*mat->values[2][2])+mat->values[3][2];

	return result;
}

Vec4 Matrix4_ApplyPoint4(const Matrix4* mat, const Vec4 point)
{
	Vec4 result;

	result.x = (point.x*mat->values[0][0])+(point.y*mat->values[1][0])+(point.z*mat->values[2][0])+(point.w*mat->values[3][0]);
	result.y = (point.x*mat->values[0][1])+(point.y*mat->values[1][1])+(point.z*mat->values[2][1])+(point.w*mat->values[3][1]);
	result.z = (point.x*mat->values[0][2])+(point.y*mat->values[1][2])+(point.z*mat->values[2][2])+(point.w*mat->values[3][2]);
	result.w = (point.x*mat->values[0][3])+(point.y*mat->values[1][3])+(point.z*mat->values[2][3])+(point.w*mat->values[3][3]);

	return result;
}

Matrix4 Matrix4_Multiply(const Matrix4* a, const Matrix4* b)
{
	float a11 = a->values[0][0];
	float a12 = a->values[0][1];
	float a13 = a->values[0][2];
	float a14 = a->values[0][3];
	float a21 = a->values[1][0];
	float a22 = a->values[1][1];
	float a23 = a->values[1][2];
	float a24 = a->values[1][3];
	float a31 = a->values[2][0];
	float a32 = a->values[2][1];
	float a33 = a->values[2][2];
	float a34 = a->values[2][3];
	float a41 = a->values[3][0];
	float a42 = a->values[3][1];
	float a43 = a->values[3][2];
	float a44 = a->values[3][3];
	float b11 = b->values[0][0];
	float b12 = b->values[0][1];
	float b13 = b->values[0][2];
	float b14 = b->values[0][3];
	float b21 = b->values[1][0];
	float b22 = b->values[1][1];
	float b23 = b->values[1][2];
	float b24 = b->values[1][3];
	float b31 = b->values[2][0];
	float b32 = b->values[2][1];
	float b33 = b->values[2][2];
	float b34 = b->values[2][3];
	float b41 = b->values[3][0];
	float b42 = b->values[3][1];
	float b43 = b->values[3][2];
	float b44 = b->values[3][3];

	Matrix4 result;
	result.values[0][0] = (a11 * b11) + (a12 * b21) + (a13 * b31) + (a14 * b41);
	result.values[0][1] = (a11 * b12) + (a12 * b22) + (a13 * b32) + (a14 * b42);
	result.values[0][2] = (a11 * b13) + (a12 * b23) + (a13 * b33) + (a14 * b43);
	result.values[0][3] = (a11 * b14) + (a12 * b24) + (a13 * b34) + (a14 * b44);
	result.values[1][0] = (a21 * b11) + (a22 * b21) + (a23 * b31) + (a24 * b41);
	result.values[1][1] = (a21 * b12) + (a22 * b22) + (a23 * b32) + (a24 * b42);
	result.values[1][2] = (a21 * b13) + (a22 * b23) + (a23 * b33) + (a24 * b43);
	result.values[1][3] = (a21 * b14) + (a22 * b24) + (a23 * b34) + (a24 * b44);
	result.values[2][0] = (a31 * b11) + (a32 * b21) + (a33 * b31) + (a34 * b41);
	result.values[2][1] = (a31 * b12) + (a32 * b22) + (a33 * b32) + (a34 * b42);
	result.values[2][2] = (a31 * b13) + (a32 * b23) + (a33 * b33) + (a34 * b43);
	result.values[2][3] = (a31 * b14) + (a32 * b24) + (a33 * b34) + (a34 * b44);
	result.values[3][0] = (a41 * b11) + (a42 * b21) + (a43 * b31) + (a44 * b41);
	result.values[3][1] = (a41 * b12) + (a42 * b22) + (a43 * b32) + (a44 * b42);
	result.values[3][2] = (a41 * b13) + (a42 * b23) + (a43 * b33) + (a44 * b43);
	result.values[3][3] = (a41 * b14) + (a42 * b24) + (a43 * b34) + (a44 * b44);

	return result;
}

Matrix4 Matrix4_CreatePerspectiveMatrix(float fov, float aspect, float near, float far)
{
	Matrix4 result = { 0 };

	float t = Tan(fov*0.5f);
	result.values[0][0] = 1.0f/(t*aspect);
	result.values[1][1] = 1.0f/t;
	result.values[2][2] = (far + near) / (far - near);
	result.values[2][3] = 1;
	result.values[3][2] = -(2 * far * near) / (far - near);

	return result;
}


Transformer Transformer_Inverse(const Transformer* transformer)
{
	Transformer result;
#if 1
	// HACK:
	Matrix4 temp = Transformer_ToMatrix4(transformer);
	temp = Matrix4_Inverse(&temp);
	result = Transformer_FromMatrix4(&temp);
#else
	result.mat = Matrix3_Inverse(&transformer->mat);

	result.pos.x = -(result.mat.values[0][0]*transformer->pos.x+result.mat.values[0][1]*transformer->pos.y+result.mat.values[0][2]*transformer->pos.z);
	result.pos.y = -(result.mat.values[1][0]*transformer->pos.x+result.mat.values[1][1]*transformer->pos.y+result.mat.values[1][2]*transformer->pos.z);
	result.pos.z = -(result.mat.values[2][0]*transformer->pos.x+result.mat.values[2][1]*transformer->pos.y+result.mat.values[2][2]*transformer->pos.z);
#endif
	return result;
}

Vec3 Transformer_ApplyPoint(const Transformer* transformer, const Vec3 point)
{
	Vec3 result = Matrix3_ApplyPoint(&transformer->mat, point);
	result.x += transformer->pos.x;
	result.y += transformer->pos.y;
	result.z += transformer->pos.z;
	return result;
}

Vec3 Transformer_ApplyPointTransposed(const Transformer* transformer, const Vec3 point)
{
	Vec3 result = Matrix3_ApplyPointTransposed(&transformer->mat, point);
	result.x -= transformer->pos.x;
	result.y -= transformer->pos.y;
	result.z -= transformer->pos.z;
	return result;
}

Transformer Transformer_Multiply(const Transformer* a, const Transformer* b)
{
	Transformer result;
	result.mat = Matrix3_Multiply(&a->mat, &b->mat);

	// transform b's positional vector using a's rotation matrix and then add a's positional vector.
	Vec3 temp = Matrix3_ApplyPoint(&b->mat, a->pos);
	//res.pos = temp;
	result.pos.x = temp.x + b->pos.x;
	result.pos.y = temp.y + b->pos.y;
	result.pos.z = temp.z + b->pos.z;

	return result;
}

Matrix4 Transformer_ToMatrix4(const Transformer* transformer)
{
	Matrix4 result;
	for (int32 y = 0; y < 3; y++)
	{
		for (int32 x = 0; x < 3; x++)
		{
			result.values[y][x] = transformer->mat.values[y][x];
		}
	}

	result.values[0][3] = 0;
	result.values[1][3] = 0;
	result.values[2][3] = 0;

	result.values[3][0] = transformer->pos.x;
	result.values[3][1] = transformer->pos.y;
	result.values[3][2] = transformer->pos.z;
	result.values[3][3] = 1;

	return result;
}

Transformer Transformer_FromMatrix4(const Matrix4* matrix)
{
	Transformer result;
	for (int32 y = 0; y < 3; y++)
	{
		for (int32 x = 0; x < 3; x++)
		{
			result.mat.values[y][x] = matrix->values[y][x];
		}
	}

	result.pos.x = matrix->values[3][0];
	result.pos.y = matrix->values[3][1];
	result.pos.z = matrix->values[3][2];

	return result;
}
