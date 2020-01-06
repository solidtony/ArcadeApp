#include "Utils/Vec2D.h"

#include <cassert>

#include "Utils/Utils.h"

const Vec2D Vec2D::Zero()
{
	return Vec2D(0.f, 0.f);
}

std::ostream& operator<<(std::ostream& consoleOut, const Vec2D& vec)
{
	consoleOut << "X: " << vec.mX << ", Y: " << vec.mY;
	return consoleOut;
}

bool Vec2D::operator==(const Vec2D& vecRHS) const
{
	return IsEqual(this->mX, vecRHS.mX) && IsEqual(this->mY, vecRHS.mY);
}

bool Vec2D::operator!=(const Vec2D& vecRHS) const
{
	return !(*this == vecRHS);
}

Vec2D Vec2D::operator-() const
{
	return Vec2D(-mX, -mY);
}

Vec2D Vec2D::operator+(const Vec2D& vecRHS) const
{
	return Vec2D(this->mX + vecRHS.mX, this->mY + vecRHS.mY);
}

Vec2D& Vec2D::operator+=(const Vec2D& vecRHS)
{
	*this = *this + vecRHS;
	return *this;
}

Vec2D Vec2D::operator-(const Vec2D& vecRHS) const
{
	return *this + (-vecRHS);
}

Vec2D& Vec2D::operator-=(const Vec2D& vecRHS)
{
	*this = *this - vecRHS;
	return *this;
}

Vec2D Vec2D::Vec2D::operator*(float scalar) const
{
	return Vec2D(scalar * mX, scalar * mY);
}

Vec2D Vec2D::operator/(float scalar) const
{
	if (IsEqual(scalar, 0.f)) { Vec2D(INFINITY, INFINITY); }
	return (*this * (1 / scalar));
}

Vec2D& Vec2D::operator*=(float scalar)
{
	*this = *this * scalar;
	return *this;
}

Vec2D& Vec2D::operator/=(float scalar)
{
	if (IsEqual(scalar, 0.f)) { Vec2D(INFINITY, INFINITY); }
	return ((*this) *= (1.f / scalar));
}

// Friend functions
Vec2D operator*(float scalar, const Vec2D& vec)
{
	return vec * scalar;
}

float Vec2D::Mag2() const
{
	return Dot(*this);
}

float Vec2D::Mag() const
{
	return sqrt(Mag2());
}

Vec2D Vec2D::GetUnitVector() const
{
	float mag = Mag();
	if (IsEqual(mag, 0.f)) { return *this; }
	
	return *this / mag;
}

Vec2D& Vec2D::Normalize()
{
	float mag = Mag();
	if (IsEqual(mag, 0.f)) { return *this; }

	return *this /= mag;
}

float Vec2D::Distance(const Vec2D& vec) const
{
	return (vec - *this).Mag();
}

float Vec2D::Dot(const Vec2D& vec) const
{
	return (mX * vec.mX + mY * vec.mY);
}

Vec2D Vec2D::ProjectOnto(const Vec2D& vec) const
{
	Vec2D unitVec = vec.GetUnitVector();
	float dot = Dot(unitVec);
	
	return dot * unitVec;
}

float Vec2D::AngleBetween(const Vec2D& vec) const
{
	return acosf( GetUnitVector().Dot(vec.GetUnitVector()) );
}

Vec2D Vec2D::Reflect(const Vec2D& normal) const
{
	return *this - 2 * ProjectOnto(normal);
}

void Vec2D::Rotate(const Vec2D& aroundPoint, float angle)
{
	*this = RotationResult(aroundPoint, angle);
}

Vec2D Vec2D::RotationResult(const Vec2D& aroundPoint, float angle) const
{
	float cosine = cosf(angle);
	float sine = sinf(angle);

	Vec2D thisVec(mX, mY);

	thisVec -= aroundPoint;
	float xRot = thisVec.mX * cosine - thisVec.mY * sine;
	float yRot = thisVec.mX * sine + thisVec.mY * cosine;

	Vec2D rot = Vec2D(xRot, yRot);

	return rot + aroundPoint;
}