#include "Collision.h"
#include "Collider.h"
#include "Actor.h"
#include "RigidBody.h"

void CollisionInfo::Resolve()
{
	if (!IsOverlapping()) return;

	float aInvMass = colliderA->parent->GetRigidBody().GetInverseMass();
	float bInvMass = colliderB->parent->GetRigidBody().GetInverseMass();

	float totalInvMass = aInvMass + bInvMass;
	Vec2 relVelocity = colliderA->parent->GetCurrentVelocity() - colliderB->parent->GetCurrentVelocity();

	colliderB->parent->SetPosition((colliderB->position) + collisionNormal * overlapAmount * bInvMass / totalInvMass);
	colliderA->parent->SetPosition((colliderA->position) - collisionNormal * overlapAmount * aInvMass / totalInvMass);
	
	float j = (-(1 + 0.5) * Dot(relVelocity, collisionNormal) / totalInvMass);

	//Depenetrate
	//Collider B
	colliderB->parent->GetRigidBody().ApplyImpulse(-collisionNormal, j);
	
	//Collider A
	colliderA->parent->GetRigidBody().ApplyImpulse(collisionNormal, j);
	
}
