#pragma once
#define PI 3.141592654f
#define PI_HALF 1.570796327f
#define PI_QUART 0.7853981635f
inline float lerp(float fMin, float fMax, float value)
{
	return fMin + value*(fMax - fMin);
}

inline float QunticCurve(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

inline string TwinRemove(const string &line, char c = ' ')
{
	string res;
	const size_t size = line.size();
	if(size < (size_t)2)
		return line;
	res.push_back(line[0]);
	for(size_t i = 1; i < size; ++i)
		if(line[i] != c || line[i - 1] != c)
			res.push_back(line[i]);
	return res;
}

inline string trim(const string &line, const string &keys = " ")
{
	string res;
	const size_t size = line.size();
	const size_t cn = keys.size();
	if(size == 0 || cn == 0)
		return res;
	size_t start = 0;
	size_t end = 0;
	for(size_t i = 0; i < size; ++i)
	{
		bool is = false;
		for(size_t k = 0; k != cn; ++k)
			if(line[i] == keys[cn])
				is = true;
		if(!is)
		{
			start = i;
			break;
		}
	}
	for(size_t i = size - 1; i >= 0; --i)
	{
		bool is = false;
		for(size_t k = 0; k != cn; ++k)
			if(line[i] == keys[cn])
				is = true;
		if(!is)
		{
			end = i;
			break;
		}
	}
	for(size_t i = start; i <= end; ++i)
		res.push_back(line[i]);
	return res;
}

inline string param(const string &line, int num = 1, char key = '`')
{
	string res;
	int start = 1;
	const size_t size = line.size();
	for(size_t i = 0; i != size; ++i)
	{
		if(line[i] == key)
		{
			start++;
			if(start > num)
				return res;
		}
		else if(start == num)
			res += line[i];
	}
	return res;
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

inline float operator "" _r(long double v)
{
	return Rand(0.0f, (float)v);
}

inline int operator "" _r(unsigned long long v)
{
	return Rand(0, (int)v);
}

inline float operator "" _rn(long double v)
{
	return Rand((float)-v, (float)v);
}