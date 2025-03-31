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

		ApplyDrag();
		currentVelocity += forceAccumulator * delta * invMass;
		forceAccumulator = Vec2();

		currentVelocity.y -= GRAVITY * delta;
		parent->actorPosition += currentVelocity * delta;
	}
}

void RigidBody::ApplyImpulse(Vec2 direction, float magnitude)
{
	currentVelocity += direction * magnitude * invMass;
	SetActive(true);
}

void RigidBody::ApplyImpulse(Vec2 impulse)
{
	currentVelocity += impulse * invMass; //TODO move mass properties to rigid body class
	SetActive(true);
}

void RigidBody::ApplyForce(Vec2 force)
{
	forceAccumulator += force;
}


void RigidBody::SetVelocity(Vec2 vel)
{
	currentVelocity = vel;
}

void RigidBody::ApplyDrag()
{
	ApplyForce(-currentVelocity * drag);

}

void RigidBody::Stop()
{
	currentVelocity = Vec2{ 0,0 };
}
