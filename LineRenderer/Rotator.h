#pragma once
#include "Vec2.h"
class Rotator
{
private:
	//We make these private because the rotator can become broken if you change just one of them directly.
	float cosAngle = 1.0f;
	float sinAngle = 0.0f;

public:
	Rotator() = default;
	Rotator(float angle);

	float GetAngle() const;

	Vec2 GetXAxis() const;
	Vec2 GetYAxis() const;

	//These are friend functions because they need to directly mess with the cos/sin angles.
	friend Vec2 operator* (Vec2 v, Rotator r);
	friend Vec2& operator*= (Vec2& v, Rotator r);
	friend Rotator operator* (Rotator r1, Rotator r2);
	friend Rotator& operator*= (Rotator& thisR, Rotator otherR);

	friend Rotator operator- (Rotator& r);
};
