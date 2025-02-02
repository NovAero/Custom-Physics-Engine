#pragma once

#include "Maths.h"
class Collider;

class CollisionInfo {
public:
	bool IsOverlapping() const { return (overlapAmount > 0.f); }
	void Resolve();

public:
	Collider* colliderA;
	Collider* colliderB;

	float overlapAmount = -1.f;
	Vec2 collisionNormal;//Points FROM a TO b

};

