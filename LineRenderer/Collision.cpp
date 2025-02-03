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
	colliderB->parent->SetPosition(colliderB->parent->GetWorldPosition() + collisionNormal * overlapAmount * colliderB->invMass / totalInvMass);
	colliderA->parent->SetPosition(colliderA->parent->GetWorldPosition() - collisionNormal * overlapAmount * colliderA->invMass / totalInvMass);

	//If speed is higher than 2.f (not bouncing)
	if (totalSpeed >= 2.f) {
		//Apply inverse forces if inverse mass is not 0
		if (colliderB->invMass != 0.f) {
			colliderB->parent->GetRigidBody().ApplyImpulse(collisionNormal, totalSpeed / colliderB->invMass);
		}
		if (colliderA->invMass != 0.f) {
			colliderA->parent->GetRigidBody().ApplyImpulse(-collisionNormal, totalSpeed / colliderA->invMass);
		}
	}
}
