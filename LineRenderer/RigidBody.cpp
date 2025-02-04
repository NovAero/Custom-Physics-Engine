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

RigidBody::RigidBody(Actor* parent, float drag, float friction) : parent(parent), drag(drag), friction(friction)
{
}

RigidBody::RigidBody(Actor* parent, float drag, float friction, Vec2 size) : parent(parent), drag(drag), friction(friction), objectSize(size)
{
}

RigidBody::~RigidBody()
{
	parent = nullptr;
}

void RigidBody::Update(float delta, Vec2 cursorPos)
{ 
	if (isDirty) {
		if (currentVelocity.GetMagnitude() >= maxMagnitude) {
			currentVelocity.SetMagnitude(maxMagnitude);
			currentSpeed = maxMagnitude;
		}
		parent->actorPosition += currentVelocity * currentSpeed * delta;
	}

	if (parent->actorPosition.y <= -10.f + objectSize.y) {
		if (bounce && currentVelocity.GetMagnitude() > 2.5f) {
			parent->actorPosition.y = -10.f + objectSize.y;
			Bounce();
		}
		else {
			parent->actorPosition.y = -10.f + objectSize.y;
			isGrounded = true;
		}
	}
	else {
		isGrounded = false;
	}
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
	if (currentVelocity.x >= 0) {
		currentVelocity.x -= drag * delta;

		if (isGrounded) {
			currentVelocity.x -= friction * delta;
		}
	}
	else {
		currentVelocity.x += drag * delta;

		if (isGrounded) {
			currentVelocity.x += friction * delta;
		}
	}

	if (isGrounded == false && isDirty == true) {
		currentVelocity.y -= gravity;
	}

	currentSpeed = currentVelocity.GetMagnitude();

}

void RigidBody::Bounce()
{
	currentVelocity.y = -currentVelocity.y;
	currentVelocity *= friction;
}
