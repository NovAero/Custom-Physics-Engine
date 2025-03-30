#pragma once
#include "Maths.h"
#include "Colour.h"

class LineRenderer;
class RigidBody;
class CircleCollider;
class Collider;

enum ObjectShape
{
	CIRCLE,
	RECT,
	TRIANGLE,
	LINE,
	POLYGON
};

class Actor {
public:

	/* 
	For actor construction, with the drawSize variable, the y 
	value will determine the size of everything except a Box and circle, (y is the radius)
	Passing in a polygon as the shape will use the x value to
	determine the amount of points to create.
	I know this is hacky but it is more useful to just use the drawSize
	than to make a new per case variable for polygons
	*/

	Actor();
	Actor(Vec2 pos, ObjectShape shape, Vec2 drawSize);
	Actor(RigidBody* rb, Collider* col);

	virtual void Update(float delta, Vec2 cursorPos);
	virtual void Draw(LineRenderer* lines);

	Vec2 GetWorldPosition() const;

	Vec2 GetDrawSize() const;

	//RigidBody Functions
	Vec2 GetCurrentVelocity() const;
	bool GetIsDirty() const;
	Vec2 GetRbSize() const;
	float GetCurrentSpeed() const;
	float GetTerminalVelocity() const;

	void SetInverseMass(float inverseMass);
	void SetPosition(Vec2 pos);

	//Reference access
	RigidBody& GetRigidBody() const;
	Collider& GetCollider() const;

private:

	void SetColliderByEnum(ObjectShape shape);
public:
	Colour colour = Colour::WHITE;

protected:
	RigidBody* rb = nullptr;
	Collider* collider = nullptr;

	ObjectShape shape = ObjectShape::CIRCLE;
	Vec2 drawSize = { 1.f,1.f };

	Vec2 actorPosition = { 0,0 };

	friend class RigidBody;
};