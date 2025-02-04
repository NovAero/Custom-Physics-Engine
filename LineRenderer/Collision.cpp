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
	if (colliderB->invMass != 0.f) {
		colliderB->parent->SetPosition((colliderB->position) + collisionNormal * overlapAmount * colliderB->invMass / totalInvMass);
	}
	if (colliderA->invMass != 0.f) {
	colliderA->parent->SetPosition((colliderA->position) - collisionNormal * overlapAmount * colliderA->invMass / totalInvMass);
	}

	//Apply inverse forces if inverse mass is not 0
	if (colliderB->invMass != 0.f) {
		colliderB->parent->GetRigidBody().ApplyImpulse(collisionNormal, totalSpeed / colliderB->invMass);
	}
	if (colliderA->invMass != 0.f) {
		colliderA->parent->GetRigidBody().ApplyImpulse(-collisionNormal, totalSpeed / colliderA->invMass);
	}
	
}
