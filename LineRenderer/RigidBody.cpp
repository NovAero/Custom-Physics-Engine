#include "RigidBody.h"
#include "Actor.h"
#include "Collider.h"

RigidBody::RigidBody()
{

}

RigidBody::RigidBody(Actor* parent) : parent(parent)
{

}

RigidBody::RigidBody(Actor* parent, Vec2 size) : parent(parent), objectSize(size)
{

}

RigidBody::RigidBody(Actor* parent, float drag) : parent(parent), drag(drag)
{
}

RigidBody::RigidBody(Actor* parent, float drag, Vec2 size) : parent(parent), drag(drag),objectSize(size)
{
}

RigidBody::~RigidBody()
{
	parent = nullptr;
}

void RigidBody::Update(float delta, Vec2 cursorPos)
{ 
	if (isStatic) return;
	if (isDirty) {

		HandleResistances(delta);

		if (currentVelocity.GetMagnitude() >= maxMagnitude) {
			currentVelocity.SetMagnitude(maxMagnitude);
			currentSpeed = maxMagnitude;
		}

		parent->actorPosition += currentVelocity * currentSpeed * delta;
	}
	else {
		currentVelocity = Vec2{ 0.f,0.f };
	}

	shouldApplyFriction = false;
	parent->GetCollider().lastCollisionNormal = { 1,0 };
}

void RigidBody::ApplyImpulse(Vec2 direction, float magnitude)
{
	currentVelocity += direction * magnitude;
	SetIsDirty(true);
}

void RigidBody::ApplyImpulse(Vec2 impulse)
{
	currentVelocity += impulse;
	SetIsDirty(true);
}

void RigidBody::ApplyConstantForce(Vec2 direction, float magnitude, float& timer, float& currentTime, float delta)
{
	if (currentTime >= timer) {
		//if force would go beyond terminal currentVelocity, set to max magnitude
		currentVelocity = direction * magnitude;
	}
	else {
		currentTime += delta;
	}
}

void RigidBody::SetVelocity(Vec2 vel)
{
	currentVelocity = vel;
}

void RigidBody::HandleResistances(float delta)
{

	if (shouldApplyFriction) {
		Vec2 tan = HandleSurfaceFriction( (parent->GetCollider().lastCollided->surfaceFriction + parent->GetCollider().surfaceFriction) * 0.5f, delta);
		Bounce(tan);
		
	} else {
		if (currentSpeed > 0.5f) {
			ApplyImpulse(-currentVelocity.GetNormalised(), drag * delta);
		}
	}
	
	currentVelocity.y -= GRAVITY * delta;

	currentSpeed = currentVelocity.GetMagnitude();
}

Vec2 RigidBody::HandleSurfaceFriction(float friction, float delta)
{
	//Set as the collision normal for code readability and lowering calls to parent->GetCollider()
	Vec2 tangentNormal = parent->GetCollider().lastCollisionNormal;
	float fs = friction * Dot(tangentNormal, GRAVITY_VEC2);
	float dir = PseudoCross(currentVelocity.GetNormalised(), tangentNormal);
	
	if (dir > 0.f) {
		tangentNormal.RotateBy90(); //Coming from left to right, apply to left
	}
	else if (dir < 0.f) {
		tangentNormal.RotateBy270(); //Coming from right to left, apply to right
	}

	ApplyImpulse(-tangentNormal, fs * delta);

	//Debug
	frictionDirection = tangentNormal;
	
	return tangentNormal;
}

void RigidBody::SetElasticityPerc(float ePercent)
{
	parent->GetCollider().elasticity = ePercent;
}

void RigidBody::Bounce(Vec2 tangent)
{

	Vec2 cNormal = parent->GetCollider().lastCollisionNormal;
	float elasticity = 0.5f * (parent->GetCollider().elasticity + parent->GetCollider().lastCollided->elasticity);
	float totalInvMass = parent->GetCollider().invMass + parent->GetCollider().lastCollided->invMass;

	float relNormVel = Dot(currentVelocity, cNormal);

	ApplyImpulse(cNormal, (-(1 + elasticity) * relNormVel) / totalInvMass );
}