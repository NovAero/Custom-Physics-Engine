#pragma once
#include "Maths.h"
#include "LineRenderer.h"

const int GRAVITY = 0.098f;

enum ObjectShape
{
	CIRCLE,
	SQUARE,
	TRIANGLE,
	LINE,
	VOID
};

class RigidBody {
public:
	RigidBody() = default;
	
	//Dynamic Constructors
	RigidBody(Vec2 position);
	RigidBody(Vec2 position, ObjectShape shape);
	RigidBody(Vec2 position, ObjectShape shape, Colour colour);
	RigidBody(Vec2 position, ObjectShape shape, Colour colour, Vec2 size);
	RigidBody(Vec2 position, float drag, float friction);
	RigidBody(Vec2 position, ObjectShape shape, Colour colour, float drag, float friction);
	RigidBody(Vec2 position, ObjectShape shape, Colour colour, float drag, float friction, Vec2 size);
	
	~RigidBody() = default;

	virtual void Update(float delta, Vec2 cursorPos);
	virtual void Draw(LineRenderer* lines);

	void ApplyImpulse(Vec2 direction, float magnitude);
	void ApplyConstantForce(Vec2 direction, float magnitude, float& timer, float& currentTime, float delta);
	void SetVelocity(Vec2 velocity);
	void SetPosition(Vec2 position);
	void Bounce();

	void HandleResistances(float delta);

	bool GetIsGrounded() const { return isGrounded; }
	bool GetIsDirty()const { return isDirty; }
	Vec2 GetSize() const { return objectSize; }
	Vec2 GetCurrentPosition() const { return currentPosition; }
	Vec2 GetCurrentVelocity() const { return currentVelocity; }
	float GetTerminalVelocity() const { return maxMagnitude; }

	void SetIsDirty(bool dirty) { isDirty = dirty; }

protected:
	
	ObjectShape shape = ObjectShape::VOID;
	Colour colour = Colour::WHITE;

	Vec2 currentPosition;
	Vec2 currentVelocity;
	Vec2 objectSize = { 1.f, 1.f };

	float maxMagnitude = 15.f;
	
	float currentSpeed = 0.f;
	float gravity = 0.098f;
	float drag = 0.7f;
	float friction = 0.5f;

	bool isGrounded = true;
	bool bounce = true;
	bool isDirty = false;
};