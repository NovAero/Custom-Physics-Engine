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
		if (currentVelocity.GetMagnitude() >= maxMagnitude) {
			currentVelocity.SetMagnitude(maxMagnitude);
			currentSpeed = maxMagnitude;
		}
		parent->actorPosition += currentVelocity * currentSpeed * delta;
	}
	HandleResistances(delta);
	shouldApplyFriction = false;
}

void RigidBody::ApplyImpulse(Vec2 direction, float magnitude)
{
	currentVelocity += direction * magnitude;
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
		HandleSurfaceFriction(parent->GetCollider().lastCollided->surfaceFriction, delta);
	} else {
		if (currentVelocity.x > 0.f) {
			currentVelocity.x -= drag * delta;
		}
		else {
			currentVelocity.x += drag * delta;
		}
	}
	
	if (abs(currentVelocity.x) <= 0.25f) {
		currentVelocity.x = 0.f;
	}

	if (isDirty && !shouldApplyFriction) {
		currentVelocity.y -= gravity * delta;
	}

	currentSpeed = currentVelocity.GetMagnitude();
}

void RigidBody::HandleSurfaceFriction(float friction, float delta)
{
	ApplyImpulse(-currentVelocity.GetNormalised(), friction * delta);
}

void RigidBody::Bounce()
{
	currentVelocity.y = -currentVelocity.y;
}
