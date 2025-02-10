#include "Collision.h"
#include "Collider.h"
#include "Actor.h"
#include "RigidBody.h"

void CollisionInfo::Resolve()
{
	if (!IsOverlapping()) return;

	float totalInvMass = colliderA->invMass + colliderB->invMass;
	float totalSpeed = colliderA->parent->GetCurrentSpeed() + colliderB->parent->GetCurrentSpeed();

	//Depenetrate
	//Apply inverse forces if inverse mass is not 0
	if (colliderB->invMass != 0.f) {
		colliderB->parent->SetPosition((colliderB->position) + collisionNormal * overlapAmount * colliderB->invMass / totalInvMass);
		colliderB->parent->GetRigidBody().ApplyImpulse(collisionNormal, (totalSpeed / 2) / colliderB->invMass);
	}
	if (colliderA->invMass != 0.f) {
		colliderA->parent->SetPosition((colliderA->position) - collisionNormal * overlapAmount * colliderA->invMass / totalInvMass);
		colliderA->parent->GetRigidBody().ApplyImpulse(-collisionNormal, (totalSpeed / 2) / colliderA->invMass);
	}

	
}
