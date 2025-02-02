#include "CollisionFunctions.h"
#include "Collider.h"

CollisionInfo GetCircleOverlapAmount(CircleCollider& a, CircleCollider& b)
{
	CollisionInfo info;
	info.colliderA = &a;
	info.colliderB = &b;
	Vec2 centreDisplacement = b.position - a.position;
	float distance = centreDisplacement.GetMagnitude();
	float separationOfSurfaces = distance - a.radius - b.radius;
	info.overlapAmount = -separationOfSurfaces;

	info.collisionNormal = centreDisplacement / distance;

	return info;
}
