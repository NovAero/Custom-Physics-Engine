#pragma once

#include "Maths.h"
class Collider;

class CollisionInfo {
public:
	bool IsOverlapping() const { return (overlapAmount > 0.f); }
	void Resolve();

public:
	Collider* colliderA = nullptr;
	Collider* colliderB = nullptr;

	float overlapAmount = -1.f;
	Vec2 collisionNormal = { 0,1 };//Points FROM a TO b
};