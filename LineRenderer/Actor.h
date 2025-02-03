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
	SQUARE,
	TRIANGLE,
	LINE
};

class Actor {
public:
	Actor();
	Actor(Vec2 pos, ObjectShape shape, Vec2 drawSize);
	Actor(RigidBody* rb, Collider* col);

	virtual void Update(float delta, Vec2 cursorPos);
	virtual void Draw(LineRenderer* lines);

	Vec2 GetWorldPosition() const;

	//RigidBody Functions
	Vec2 GetCurrentVelocity() const;
	bool GetIsGrounded() const;
	bool GetIsDirty() const;
	Vec2 GetRbSize() const;
	float GetCurrentSpeed() const;
	float GetTerminalVelocity() const;

	void SetPosition(Vec2 pos);

	//Reference access
	RigidBody& GetRigidBody() const;
	Collider& GetCollider() const;

private:

	void SetColliderByEnum(ObjectShape shape);

protected:
	RigidBody* rb = nullptr;
	Collider* collider = nullptr;

	ObjectShape shape = ObjectShape::CIRCLE;
	Vec2 drawSize = { 1.f,1.f };
	Colour colour = Colour::WHITE;

	Vec2 actorPosition = { 0,0 };

	friend class RigidBody;
};