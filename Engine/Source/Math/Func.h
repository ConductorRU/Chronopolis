#pragma once
#define PI 3.141592654f
#define PI_HALF 1.570796327f
#define PI_QUART 0.7853981635f
inline float lerp(float fMin, float fMax, float value)
{
	return fMin + value*(fMax - fMin);
}

inline float clerp(float p_1, float p, float p1, float p2, float f)
{
	if(f <= 0.0f)
		return p;
	else if(f >= 1.0f)
		return p1;
	float a = (p - p1)*1.5f + (p2 - p_1)*0.5f;
	float b = p*-2.5f + p1*2.0f - p2*0.5f + p_1;
	float c = (p1 - p_1)*0.5f;
	return ((a*f + b)*f + c)*f + p;
}

inline int Clamp(int val, int minVal, int maxVal)
{
	return max(minVal, min(val, maxVal));
}

inline float Clamp(float val, float minVal, float maxVal)
{
	return max(minVal, min(val, maxVal));
}

inline int toInt(float f)
{
	if(f < 0.0f)
		return (int)(f - 0.5f);
	return (int)(f + 0.5f);
}

inline float AngleToRad(float angle)
{
	return (3.141592654f / 180.0f)*angle;
}

inline float operator "" _deg(long double angle)
{
	return (float)((3.141592654f / 180.0f)*angle);
}

inline float operator "" _km(long double km)
{
	return (float)(km*1000.0);
}

inline float operator "" _m(long double m)
{
	return (float)m;
}

inline float operator "" _sm(long double sm)
{
	return (float)(sm*0.01);
}

inline float operator "" _mm(long double mm)
{
	return (float)(mm*0.001);
}

inline int Rand(int min, int max)
{
	if(min == max)
		return min;
	if(min > max)
		return int(rand() % (min - max + 1) + max);
	return int(rand() % (max - min + 1) + min);
}

inline float Rand(float min, float max)
{
	return (max - min)*((float)rand() / (float)(0x7fff)) + min;
}