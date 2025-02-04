#include "Actor.h"
#include "LineRenderer.h"
#include "RigidBody.h"
#include "Collider.h"

Actor::Actor()
{
	rb = new RigidBody();
	rb->parent = this;
	collider = new Collider(0.f);
	collider->parent = this;
}

Actor::Actor(Vec2 pos, ObjectShape shape, Vec2 drawSize)
{
	this->actorPosition = pos;
	this->shape = shape;
	this->drawSize = drawSize;

	rb = new RigidBody();
	rb->parent = this;
	rb->objectSize = drawSize/2;

	SetColliderByEnum(shape);

	collider->parent = this;
}

Actor::Actor(RigidBody* rb, Collider* col)
{
	this->rb = rb;
	rb->parent = this;
	collider = col;
	collider->parent = this;
}

void Actor::Update(float delta, Vec2 cursorPos)
{
	Vec2 savePos = actorPosition;
	rb->Update(delta, cursorPos);
	
	collider->UpdatePos(delta);
	
	rb->HandleResistances(delta);
}

void Actor::Draw(LineRenderer* lines)
{
	switch (shape) {
	case CIRCLE:
		lines->DrawCircle(actorPosition, drawSize.y / 2, colour);
		lines->DrawCircle(actorPosition, drawSize.y, Colour::CYAN);
		break;
	case SQUARE:
	{
		//Bottom
		lines->DrawLineSegment(Vec2{ actorPosition.x - (drawSize.x / 2), actorPosition.y - (drawSize.y / 2) }, Vec2{ actorPosition.x + (drawSize.x / 2), actorPosition.y - (drawSize.y / 2) }, colour);
		//Left
		lines->DrawLineSegment(Vec2{ actorPosition.x - (drawSize.x / 2), actorPosition.y - (drawSize.y / 2) }, Vec2{ actorPosition.x - (drawSize.x / 2), actorPosition.y + (drawSize.y / 2) }, colour);
		//Top
		lines->DrawLineSegment(Vec2{ actorPosition.x - (drawSize.x / 2), actorPosition.y + (drawSize.y / 2) }, Vec2{ actorPosition.x + (drawSize.x / 2), actorPosition.y + (drawSize.y / 2) }, colour);
		//Right
		lines->DrawLineSegment(Vec2{ actorPosition.x + (drawSize.x / 2), actorPosition.y + (drawSize.y / 2) }, Vec2{ actorPosition.x + (drawSize.x / 2), actorPosition.y - (drawSize.y / 2) }, colour);

		BoxCollider* pts = dynamic_cast<BoxCollider*>(collider);

		if (pts != nullptr) {
			//Bottom
			lines->DrawCircle(pts->position, 0.2f, Colour::ORANGE);
			lines->DrawCircle(pts->GetPoints()[0], 0.2f, Colour::GREEN);
			lines->DrawCircle(pts->GetPoints()[1], 0.2f, Colour::RED);
			lines->DrawCircle(pts->GetPoints()[2], 0.2f, Colour::CYAN);
			lines->DrawCircle(pts->GetPoints()[3], 0.2f, Colour::YELLOW);
		}
		break;
	}
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


void Actor::SetInverseMass(float inverseMass)
{
	collider->invMass = inverseMass;
}

void Actor::SetPosition(Vec2 pos)
{
	actorPosition = pos;
	collider->position = pos;
}

RigidBody& Actor::GetRigidBody() const
{
	return *rb;
}

Collider& Actor::GetCollider() const
{
	return *collider;
}

void Actor::SetColliderByEnum(ObjectShape shape)
{
	switch (shape) {
	case CIRCLE:
		collider = new CircleCollider(actorPosition, drawSize.y / 2, drawSize.y * PI);
		break;

	case SQUARE:
		collider = new BoxCollider(actorPosition, drawSize, drawSize.x * drawSize.y);
		break;

	case TRIANGLE:
	{
		std::vector<Vec2> temp;

		temp.push_back(Vec2(actorPosition.x - drawSize.x / 2, actorPosition.y - drawSize.y / 2));
		temp.push_back(Vec2(actorPosition.x, actorPosition.y + drawSize.y / 2));
		temp.push_back(Vec2(actorPosition.x + drawSize.x / 2, actorPosition.y + drawSize.y / 2));

		collider = new PolygonCollider(actorPosition, temp, (drawSize.x*drawSize.y) / 2);
		break;
	}
	case LINE:
	{
		std::vector<Vec2> temp;
		temp.push_back(Vec2(actorPosition.x - drawSize.x / 2, actorPosition.y));
		temp.push_back(Vec2(actorPosition.x + drawSize.x / 2, actorPosition.y));

		drawSize.y = 0.1f;

		collider = new PolygonCollider(actorPosition, temp, drawSize.x);
		break;
	}
	}
}
