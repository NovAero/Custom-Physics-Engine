#include "Collision.h"
#include "Collider.h"
#include "Actor.h"
#include "RigidBody.h"

void CollisionInfo::Resolve()
{
	if (!IsOverlapping()) return;

	float totalInvMass = colliderA->invMass + colliderB->invMass;
	float totalSpeed = colliderA->parent->GetCurrentSpeed() + colliderB->parent->GetCurrentSpeed();

	colliderA->lastCollided = colliderB;
	colliderB->lastCollided = colliderA;

	colliderA->lastCollisionNormal = -collisionNormal;
	colliderB->lastCollisionNormal = collisionNormal;

	//TODO fix friction and other resistances to account for moveable objects not just -10.f

	//TODO rewrite collision actions, rigidbody velocity and other class action handling
	
	//Depenetrate
	//Apply inverse forces if inverse mass is not 0

	//Collider A
	if (colliderA->invMass != 0.f) {
		float depenCalc = colliderB->invMass == 0 ? overlapAmount : overlapAmount * colliderA->invMass / totalInvMass;
		colliderA->parent->SetPosition((colliderA->position) - collisionNormal * depenCalc);
		colliderA->parent->GetRigidBody().ApplyImpulse(-collisionNormal, totalSpeed / colliderA->invMass);
		colliderA->parent->GetRigidBody().SetShouldApplyFriction(true);
		
	}
	//Collider B
	if (colliderB->invMass != 0.f) {
		float depenCalc = colliderA->invMass == 0 ? overlapAmount : overlapAmount * colliderB->invMass / totalInvMass;
		colliderB->parent->SetPosition((colliderB->position) + collisionNormal * depenCalc);
		colliderB->parent->GetRigidBody().ApplyImpulse(collisionNormal, totalSpeed/colliderB->invMass);
		colliderB->parent->GetRigidBody().SetShouldApplyFriction(true);
	}
}
