#pragma once

namespace math
{
	namespace constants
	{
		constexpr float PI_F = 3.14159265358979323846f;
		constexpr float TWO_PI_F = 2.0f * PI_F;
	}
}

bool IsEqual(float x, float y);
bool IsEqual(float x, float y, float epsilon);

bool IsLessThanOrEqual(float x, float y, float epsilon);
bool IsLessThanOrEqual(float x, float y);

bool IsGreaterThanOrEqual(float x, float y, float epsilon);
bool IsGreaterThanOrEqual(float x, float y);