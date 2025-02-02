#include "Collision.h"
#include "Collider.h"

void CollisionInfo::Resolve()
{
	if (!IsOverlapping()) return;

	float totalinvMass = colliderA->invMass + colliderB->invMass;

	colliderB->position += collisionNormal * overlapAmount * colliderB->invMass / totalinvMass;
	colliderA->position -= collisionNormal * overlapAmount * colliderA->invMass / totalinvMass;

}
