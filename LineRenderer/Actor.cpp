#include "Actor.h"
#include "LineRenderer.h"
#include "RigidBody.h"
#include "Collider.h"

Actor::Actor()
{
	rb = new RigidBody();
	rb->parent = this;
	collider = new CircleCollider(actorPosition, drawSize.y/2, drawSize.y);
	collider->parent = this;
}

Actor::Actor(Vec2 pos, ObjectShape shape, Vec2 drawSize)
{
	actorPosition = pos;
	this->shape = shape;
	this->drawSize = drawSize;

	rb = new RigidBody();
	rb->parent = this;
	rb->objectSize = drawSize/2;

	collider = new CircleCollider(actorPosition, drawSize.y/2, drawSize.y);
	collider->parent = this;
}

Actor::Actor(RigidBody* rb, Collider* col)
{
}

void Actor::Update(float delta, Vec2 cursorPos)
{
	rb->Update(delta, cursorPos);
	collider->position = actorPosition;
}

void Actor::Draw(LineRenderer* lines)
{
	switch (shape) {
	case CIRCLE:
		lines->DrawCircle(actorPosition, drawSize.y / 2, colour);
		break;
	case SQUARE:
		//Bottom
		lines->DrawLineSegment(Vec2{ actorPosition.x - (drawSize.x / 2), actorPosition.y - (drawSize.y / 2) }, Vec2{ actorPosition.x + (drawSize.x / 2), actorPosition.y - (drawSize.y / 2) }, colour);
		//Left
		lines->DrawLineSegment(Vec2{ actorPosition.x - (drawSize.x / 2), actorPosition.y - (drawSize.y / 2) }, Vec2{ actorPosition.x - (drawSize.x / 2), actorPosition.y + (drawSize.y / 2) }, colour);
		//Top
		lines->DrawLineSegment(Vec2{ actorPosition.x - (drawSize.x / 2), actorPosition.y + (drawSize.y / 2) }, Vec2{ actorPosition.x + (drawSize.x / 2), actorPosition.y + (drawSize.y / 2) }, colour);
		//Right
		lines->DrawLineSegment(Vec2{ actorPosition.x + (drawSize.x / 2), actorPosition.y + (drawSize.y / 2) }, Vec2{ actorPosition.x + (drawSize.x / 2), actorPosition.y - (drawSize.y / 2) }, colour);
		break;
	case TRIANGLE:
		//Bottom
		lines->DrawLineSegment(Vec2{ actorPosition.x - (drawSize.x / 2), actorPosition.y - (drawSize.y / 2) }, Vec2{ actorPosition.x + (drawSize.x / 2), actorPosition.y - (drawSize.y / 2) }, colour);
		//Left
		lines->DrawLineSegment(Vec2{ actorPosition.x - (drawSize.x / 2), actorPosition.y - (drawSize.y / 2) }, Vec2{ actorPosition.x, actorPosition.y + (drawSize.y / 2) }, colour);
		//Right
		lines->DrawLineSegment(Vec2{ actorPosition.x + (drawSize.x / 2), actorPosition.y - (drawSize.y / 2) }, Vec2{ actorPosition.x , actorPosition.y + (drawSize.y / 2) }, colour);
		break;
	case LINE:
		//Single Line
		lines->DrawLineSegment(Vec2{ actorPosition.x - (drawSize.x / 2), actorPosition.y }, Vec2{ actorPosition.x + (drawSize.x / 2), actorPosition.y }, colour);
		break;
	}
}

Vec2 Actor::GetWorldPosition() const
{
	return actorPosition;
}

float Actor::GetCurrentSpeed() const
{
	return rb->currentSpeed;
}

Vec2 Actor::GetCurrentVelocity() const
{
	return rb->currentVelocity;
}

float Actor::GetTerminalVelocity() const
{
	return rb->GetTerminalVelocity();
}

bool Actor::GetIsGrounded() const
{
	return rb->isGrounded;
}

bool Actor::GetIsDirty() const
{
	return rb->isDirty;
}

Vec2 Actor::GetRbSize() const
{
	return rb->objectSize;
}


void Actor::SetPosition(Vec2 pos)
{
	actorPosition = pos;
	rb->SetPosition(actorPosition);
	collider->position = pos;
}

RigidBody& Actor::GetRigidBody() const
{
	return *rb;
}

CircleCollider& Actor::GetCollider() const
{
	return *collider;
}
