#include "Utils/Utils.h"
#include <cmath>
#include <cctype>

static constexpr float EPSILON = 0.0001f;

bool IsEqual(float x, float y, float epsilon)
{
	return fabsf(x - y) < epsilon;
}
bool IsEqual(float x, float y)
{
	return IsEqual(x, y, EPSILON);
}

bool IsLessThanOrEqual(float x, float y, float epsilon)
{
	return (x < y || IsEqual(x, y, epsilon));
}
bool IsLessThanOrEqual(float x, float y)
{
	return IsLessThanOrEqual(x, y, EPSILON);
}

bool IsGreaterThanOrEqual(float x, float y, float epsilon)
{
	return (x > y || IsEqual(x, y, epsilon));
}
bool IsGreaterThanOrEqual(float x, float y)
{
	return IsGreaterThanOrEqual(x, y, EPSILON);
}

float MillisecondsToSeconds(unsigned int miliseconds)
{
	return static_cast<float>(miliseconds) / 1000.f;
}

unsigned int GetIndex(unsigned int width, unsigned int row, unsigned int col)
{
	return row * width + col;
}

bool StringCompare(const std::string& a, const std::string& b)
{
	if (a.length() == b.length())
	{
		return std::equal(b.begin(), b.end(), a.begin(), [](unsigned char a, unsigned char b)
		{
			return std::tolower(a) == std::tolower(b);
		});
	}
	
	return false;
}