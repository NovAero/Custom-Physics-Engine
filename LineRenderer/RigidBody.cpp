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
	if (isActive) {
		HandleResistances(delta);

		if (currentVelocity.GetMagnitude() >= maxMagnitude) {
			currentVelocity.SetMagnitude(maxMagnitude);
		}
		parent->actorPosition += currentVelocity * currentVelocity.GetMagnitude() * delta;
	}
}

void RigidBody::ApplyImpulse(Vec2 direction, float magnitude)
{
	currentVelocity += direction * magnitude;
	SetActive(true);
}

void RigidBody::ApplyImpulse(Vec2 impulse)
{
	currentVelocity += impulse;
	SetActive(true);
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

	if (currentVelocity.x > 0.25) {
		ApplyImpulse(-currentVelocity.GetNormalised() * drag * delta);
	}
	currentVelocity.y -= GRAVITY * delta;
}

void RigidBody::Stop()
{
	currentVelocity = Vec2{ 0,0 };
}
