
#include "common/Math.h"

#include <math.h>

float Floor(float v)
{
	return floorf(v);
}

double FloorD(double v)
{
	return floor(v);
}

float Ceil(float v)
{
	return ceilf(v);
}

double CeilD(double v)
{
	return ceil(v);
}

float Round(float v)
{
	return roundf(v);
}

double RoundD(double v)
{
	return round(v);
}

float Abs(float v)
{
	return v >= 0.0f ? v : -v;
}

double AbsD(double v)
{
	return v >= 0.0 ? v : -v;
}

int32 AbsI(int32 v)
{
	return v >= 0 ? v : -v;
}

int64 AbsI64(int64 v)
{
	return v >= 0 ? v : -v;
}

float Sign(float v)
{
	return v == 0 ? 0.0f : v > 0 ? 1.0f : -1.0f;
}

double SignD(double v)
{
	return v == 0 ? 0.0 : v > 0 ? 1.0 : -1.0;
}

float SignBinary(float v)
{
	return v >= 0 ? 1.0f : -1.0f;
}

double SignBinaryD(double v)
{
	return v >= 0 ? 1.0 : -1.0;
}

int32 SignI(int32 v)
{
	return v == 0 ? 0 : v > 0 ? 1 : -1;
}

int64 SignI64(int64 v)
{
	return v == 0 ? 0 : v > 0 ? 1 : -1;
}

float Pow(float a, float b)
{
	return powf(a, b);
}

double PowD(double a, double b)
{
	return pow(a, b);
}

float Log(float v)
{
	return logf(v);
}

double LogD(double v)
{
	return log(v);
}

float Log2(float v)
{
	return log2f(v);
}

double Log2D(double v)
{
	return log2(v);
}

float Log10(float v)
{
	return log10f(v);
}

double Log10D(double v)
{
	return log10(v);
}

float Exp(float v)
{
	return expf(v);
}

double ExpD(double v)
{
	return exp(v);
}

float Sin(float v)
{
	return sinf(v);
}

double SinD(double v)
{
	return sin(v);
}

float Cos(float v)
{
	return cosf(v);
}

double CosD(double v)
{
	return cos(v);
}

float ACos(float v)
{
	return acosf(v);
}

double ACosD(double v)
{
	return acos(v);
}

float Tan(float v)
{
	return tanf(v);
}

double TanD(double v)
{
	return tan(v);
}

float Atan(float v)
{
	return atanf(v);
}

double AtanD(double v)
{
	return atan(v);
}

float Atan2(float v0, float v1)
{
	return atan2f(v0, v1);
}

double Atan2D(double v0, double v1)
{
	return atan2(v0, v1);
}

float FMod(float v0, float v1)
{
	return fmodf(v0, v1);
}

double FModD(double v0, double v1)
{
	return fmod(v0, v1);
}

float Sqrt(float v)
{
	return sqrtf(v);
}

double SqrtD(double v)
{
	return sqrt(v);
}

float Max(float a, float b)
{
	return a > b ? a : b;
}

double MaxD(double a, double b)
{
	return a > b ? a : b;
}

int32 MaxI(int32 a, int32 b)
{
	return a > b ? a : b;
}

uint32 MaxUI(uint32 a, uint32 b)
{
	return a > b ? a : b;
}

int64 MaxI64(int64 a, int64 b)
{
	return a > b ? a : b;
}

uint64 MaxUI64(uint64 a, uint64 b)
{
	return a > b ? a : b;
}

float Min(float a, float b)
{
	return a < b ? a : b;
}

double MinD(double a, double b)
{
	return a < b ? a : b;
}

int32 MinI(int32 a, int32 b)
{
	return a < b ? a : b;
}

uint32 MinUI(uint32 a, uint32 b)
{
	return a < b ? a : b;
}

int64 MinI64(int64 a, int64 b)
{
	return a < b ? a : b;
}

uint64 MinUI64(uint64 a, uint64 b)
{
	return a < b ? a : b;
}

float Clamp(float v, float min, float max)
{
	return Min(Max(v, min), max);
}

double ClampD(double v, double min, double max)
{
	return MinD(MaxD(v, min), max);
}

int32 ClampI(int32 v, int32 min, int32 max)
{
	return MinI(MaxI(v, min), max);
}

uint32 ClampUI(uint32 v, uint32 min, uint32 max)
{
	return MinUI(MaxUI(v, min), max);
}

int64 ClampI64(int64 v, int64 min, int64 max)
{
	return MinI64(MaxI64(v, min), max);
}

uint64 ClampUI64(uint64 v, uint64 min, uint64 max)
{
	return MinUI64(MaxUI64(v, min), max);
}

float Wrap(float v, float max)
{
	return Clamp(v-Floor(v/max)*max, 0, max);
}

double WrapD(double v, double max)
{
	return ClampD(v-FloorD(v/max)*max, 0, max);
}

float WrapAngle(float v)
{
	return Wrap(v, PI32*2);
}

double WrapAngleD(double v)
{
	return WrapD(v, PI64*2);
}

float MoveTowards(float from, float to, float amount)
{
	float delta = to-from;
	if (Abs(delta) <= amount)
	{
		return to;
	}
	return from+Sign(delta)*amount;
}

double MoveTowardsD(double from, double to, double amount)
{
	double delta = to-from;
	if (AbsD(delta) <= amount)
	{
		return to;
	}
	return from+SignD(delta)*amount;
}

float DeltaAngle(float a, float b)
{
	return FMod((b-a)+PI32, PI32*2)-PI32;
}

double DeltaAngleD(double a, double b)
{
	return FModD((b-a)+PI64, PI64*2)-PI64;
}

float MoveTowardsAngle(float from, float to, float amount)
{
	float delta = DeltaAngle(from, to);
	if (Abs(delta) <= amount)
	{
		return to;
	}
	return WrapAngle(MoveTowards(from, from+delta, amount));
}

double MoveTowardsAngleD(double from, double to, double amount)
{
	double delta = DeltaAngleD(from, to);
	if (AbsD(delta) <= amount)
	{
		return to;
	}
	return WrapAngleD(MoveTowardsD(from, from+delta, amount));
}

float LerpAngle(float current, float target, float amount)
{
	float delta = DeltaAngle(current, target);
	return current+delta*Clamp(amount, 0, 1);
}

double LerpAngleD(double current, double target, double amount)
{
	double delta = DeltaAngleD(current, target);
	return current+delta*ClampD(amount, 0, 1);
}

float Lerp(float from, float to, float factor)
{
	factor = Clamp(factor, 0, 1);
	return from+(to-from)*factor;
}

double LerpD(double from, double to, double factor)
{
	factor = ClampD(factor, 0, 1);
	return from+(to-from)*factor;
}

float LerpUnclamped(float from, float to, float factor)
{
	return from+(to-from)*factor;
}

double LerpUnclampedD(double from, double to, double factor)
{
	return from+(to-from)*factor;
}

float RadToDeg(float v)
{
	static const float scaler = 180.0f/PI32;
	return v*scaler;
}

double RadToDegD(double v)
{
	static const double scaler = 180.0l/PI64;
	return v*scaler;
}

float DegToRad(float v)
{
	static const float scaler = PI32/180.0f;
	return v*scaler;
}

double DegToRadD(double v)
{
	static const double scaler = PI64/180.0l;
	return v*scaler;
}
