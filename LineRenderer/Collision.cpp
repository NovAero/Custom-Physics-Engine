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

	colliderA->parent->GetRigidBody().frictionDirection = contactPoints[0];

	//TODO fix friction and other resistances to account for moveable objects not just -10.f

	//TODO rewrite collision actions, rigidbody velocity and other class action handling
	
	//Depenetrate
	//Apply inverse forces if inverse mass is not 0

	//Collider B
	if (colliderB->invMass != 0.f) {
		colliderB->parent->SetPosition((colliderB->position) + collisionNormal * overlapAmount * colliderB->invMass / totalInvMass);
		colliderB->parent->GetRigidBody().ApplyImpulse(collisionNormal, totalSpeed / colliderB->invMass);
		colliderB->parent->GetRigidBody().SetShouldApplyFriction(true);
	}
	//Collider A
	if (colliderA->invMass != 0.f) {
		colliderA->parent->SetPosition((colliderA->position) - collisionNormal * overlapAmount * colliderA->invMass / totalInvMass);
		colliderA->parent->GetRigidBody().ApplyImpulse(-collisionNormal, totalSpeed / colliderA->invMass);
		colliderA->parent->GetRigidBody().SetShouldApplyFriction(true);
		
	}
}
