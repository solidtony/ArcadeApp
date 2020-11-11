#pragma once

#include "Shapes/AARectangle.h"
#include <stdint.h>

class Screen;
struct BoundaryEdge;

class Ball
{
public:
	Ball();
	Ball(const Vec2D& pos, float radius);

	void Update(uint32_t dt);
	void Draw(Screen& screen);
	void MakeFlushWithEdge(const BoundaryEdge& edge, Vec2D& pointOnEdge, bool limitToEdge);
	inline void Stop() { mVelocity = Vec2D::Zero(); }
	void MoveTo(const Vec2D& point);

	void Bounce(const BoundaryEdge& edge);

	inline const AARectangle& GetBoundingRect() const { return mBBox; }
	inline void SetVelocity(const Vec2D& vel) { mVelocity = vel; }
	inline const Vec2D GetVelocity() const { return mVelocity; }
	inline float GetRadius() const { return mBBox.GetWidth() / 2.f; }
	inline const Vec2D GetPosition() const { return mBBox.GetCenterPoint(); }

private:
	static const float RADIUS;

	AARectangle mBBox;
	Vec2D mVelocity;
};