#include "Utils/Utils.h"
#include <cmath>

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