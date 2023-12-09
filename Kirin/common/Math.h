#pragma once

#include "common/Standard.h"

#define PI32 ((float)3.14159265358979323846)
#define PI64 (3.14159265358979323846)

#define Int8Max (127)
#define Int8Min ((-127 - 1))
#define UInt8Max (0xff)
#define UInt8Min (0)

#define Int16Max (32767)
#define Int16Min ((-32767 - 1))
#define UInt16Max (0xffff)
#define UInt16Min (0)

#define Int32Max (2147483647)
#define Int32Min ((-2147483647 - 1))
#define UInt32Max (0xffffffff)
#define UInt3Min (0)

#define Int64Max (9223372036854775807)
#define Int64Min ((-9223372036854775807 - 1))
#define UInt64Max (0xffffffffffffffff)
#define UInt64Min (0)

#define FloatMin (1.175494351e-38F)
#define FloatMax (3.402823466e+38F)
#define FloatInfinity (((float)(1e+300*1e+300)))

#define FloatNegMin (-1.175494351e-38F)
#define FloatNegMax (-3.402823466e+38F)
#define FloatNegInfinity (-((float)(1e+300*1e+300)))

#define DoubleMin (2.2250738585072014e-308)
#define DoubleMax (1.7976931348623158e+308)
#define DoubleInfinity (((double)(1e+300*1e+300)))

#define DoubleNegMin (-2.2250738585072014e-308)
#define DoubleNegMax (-1.7976931348623158e+308)
#define DoubleNegInfinity (-((double)(1e+300*1e+300)))

float Floor(float v);
double FloorD(double v);
float Ceil(float v);
double CeilD(double v);
float Round(float v);
double RoundD(double v);
float Abs(float v);
double AbsD(double v);
int32 AbsI(int32 v);
int64 AbsI64(int64 v);
float Sign(float v);
double SignD(double v);
float SignBinary(float v);
double SignBinaryD(double v);
int32 SignI(int32 v);
int64 SignI64(int64 v);
float Pow(float a, float b);
double PowD(double a, double b);
float Log(float v);
double LogD(double v);
float Log2(float v);
double Log2D(double v);
float Log10(float v);
double Log10D(double v);
float Exp(float v);
double ExpD(double v);
float Sin(float v);
double SinD(double v);
float Cos(float v);
double CosD(double v);
float ACos(float v);
double ACosD(double v);
float Tan(float v);
double TanD(double v);
float Atan(float v);
double AtanD(double v);
float Atan2(float v0, float v1);
double Atan2D(double v0, double v1);
float FMod(float v0, float v1);
double FModD(double v0, double v1);
float Sqrt(float v);
double SqrtD(double v);
float Max(float a, float b);
double MaxD(double a, double b);
int32 MaxI(int32 a, int32 b);
uint32 MaxUI(uint32 a, uint32 b);
int64 MaxI64(int64 a, int64 b);
uint64 MaxUI64(uint64 a, uint64 b);
float Min(float a, float b);
double MinD(double a, double b);
int32 MinI(int32 a, int32 b);
uint32 MinUI(uint32 a, uint32 b);
int64 MinI64(int64 a, int64 b);
uint64 MinUI64(uint64 a, uint64 b);
float Clamp(float v, float min, float max);
double ClampD(double v, double min, double max);
int32 ClampI(int32 v, int32 min, int32 max);
uint32 ClampUI(uint32 v, uint32 min, uint32 max);
int64 ClampI64(int64 v, int64 min, int64 max);
uint64 ClampUI64(uint64 v, uint64 min, uint64 max);
float Wrap(float v, float max);
double WrapD(double v, double max);
float WrapAngle(float v);
double WrapAngleD(double v);
float MoveTowards(float from, float to, float amount);
double MoveTowardsD(double from, double to, double amount);
float DeltaAngle(float a, float b);
double DeltaAngleD(double a, double b);
float MoveTowardsAngle(float from, float to, float amount);
double MoveTowardsAngleD(double from, double to, double amount);
float LerpAngle(float current, float target, float amount);
double LerpAngleD(double current, double target, double amount);
float Lerp(float from, float to, float factor);
double LerpD(double from, double to, double factor);
float LerpUnclamped(float from, float to, float factor);
double LerpUnclampedD(double from, double to, double factor);
float RadToDeg(float v);
double RadToDegD(double v);
float DegToRad(float v);
double DegToRadD(double v);
