#pragma once

#include <iostream>

class Vec2D
{
public:
	static const Vec2D Zero();

	Vec2D(): Vec2D(0.f, 0.f) {}
	Vec2D(float x, float y): mX(x), mY(y) {}

	inline void SetX(float x) { mX = x; }
	inline void SetY(float y) { mY = y; }
	inline float GetX() const { return mX; }
	inline float GetY() const { return mY; }

	friend std::ostream& operator<<(std::ostream& consoleOut, const Vec2D& vec);

	bool operator==(const Vec2D& vecRHS) const;
	bool operator!=(const Vec2D& vecRHS) const;
	Vec2D operator-() const;
	Vec2D operator+(const Vec2D& vecRHS) const;
	Vec2D& operator+=(const Vec2D& vecRHS);
	Vec2D operator-(const Vec2D& vecRHS) const;
	Vec2D& operator-=(const Vec2D& vecRHS);
	Vec2D operator*(float scalar) const;
	Vec2D operator/(float scalar) const;
	Vec2D& operator*=(float scalar);
	Vec2D& operator/=(float scalar);
	friend Vec2D operator*(float scalar, const Vec2D& vec);
	
	float Mag2() const;
	float Mag() const;
	Vec2D GetUnitVector() const;
	Vec2D& Normalize();

	float Distance(const Vec2D& vec) const;
	float Dot(const Vec2D& vec) const;

	Vec2D ProjectOnto(const Vec2D& vec) const;
	float AngleBetween(const Vec2D& vec) const;
	Vec2D Reflect(const Vec2D& normal) const;
	void Rotate(const Vec2D& aroundPoint, float angle);
	Vec2D RotationResult(const Vec2D& aroundPoint, float angle) const;
private:
	float mX, mY;
};