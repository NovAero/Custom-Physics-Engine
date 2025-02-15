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
	

	shouldApplyFriction = false;
	parent->GetCollider().lastCollisionNormal = { 1,0 };
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
		Bounce();
		HandleSurfaceFriction( (parent->GetCollider().lastCollided->surfaceFriction + parent->GetCollider().surfaceFriction) * 0.5, delta);
		
	} else {
		if (currentSpeed > 0.5f) {
			ApplyImpulse(-currentVelocity.GetNormalised(), drag * delta);
		}
	}
	
	currentVelocity.y -= gravity * delta;

	currentSpeed = currentVelocity.GetMagnitude();
}

void RigidBody::HandleSurfaceFriction(float friction, float delta)
{
	//Set as the collision normal for code readability and lowering calls to parent->GetCollider()
	Vec2 tangentNormal = parent->GetCollider().lastCollisionNormal;
	float dir = PseudoCross(currentVelocity.GetNormalised(), tangentNormal);
	
	if (dir > 0.f) {
		tangentNormal.RotateBy90(); //Coming from left to right, apply to left
		ApplyImpulse(tangentNormal, friction * gravity * delta);
	}
	else if (dir < 0.f) {
		tangentNormal.RotateBy270(); //Coming from right to left, apply to right
		ApplyImpulse(tangentNormal, friction * gravity * delta);
	}
	frictionDirection = tangentNormal;
}

Vec2 RigidBody::Bounce()
{
	Vec2 reflection = currentVelocity.GetNormalised();
	Vec2 cNormal = parent->GetCollider().lastCollisionNormal;

	//Calculate reflection angle as rotated by the angle between the normal and the current velocity
	float dir = PseudoCross(cNormal, reflection);
	float angle = AngleBetween(-cNormal, reflection); //Angle between normal and velocity 

	if (abs(dir) < 0.9f) { //Dont apply force if the cross is nearly a perfect 90 degrees, which turns it into a tangent force

		if (dir > 0.f) { //Rotate to left
			reflection = cNormal.RotateBy(angle);
		}
		else if (dir < 0.f) { //Rotate to right
			reflection = cNormal.RotateBy(-angle);
		}

		SetVelocity(reflection * currentSpeed * elasticity);
	}
	
	return -reflection;
}