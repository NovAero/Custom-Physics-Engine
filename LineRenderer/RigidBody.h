#pragma once
#include "Maths.h"
#include "LineRenderer.h"

class Actor;

const int GRAVITY = 0.098f;

class RigidBody {
public:
	RigidBody();
	//Dynamic Constructors
	RigidBody(Actor* parent);
	RigidBody(Actor* parent, Vec2 size);
	RigidBody(Actor* parent, float drag, float friction);
	RigidBody(Actor* parent, float drag, float friction, Vec2 size);
	
	~RigidBody();

	virtual void Update(float delta, Vec2 cursorPos);

	void ApplyImpulse(Vec2 direction, float magnitude);
	void ApplyConstantForce(Vec2 direction, float magnitude, float& timer, float& currentTime, float delta);
	void SetVelocity(Vec2 velocity);
	void Bounce();

	void HandleResistances(float delta);

	bool GetIsGrounded() const { return isGrounded; }
	bool GetIsDirty()const { return isDirty; }
	Vec2 GetSize() const { return objectSize; }
	float GetCurrentSpeed() const { return currentSpeed; }
	Vec2 GetCurrentVelocity() const { return currentVelocity; }
	float GetTerminalVelocity() const { return maxMagnitude; }
	bool GetIsBouncy() const { return isBouncy; }
	bool GetIsStatic() const { return isStatic; }

	void SetIsDirty(bool dirty) { isDirty = dirty; }
	void SetIsStatic(bool _static) { isStatic = _static; }

protected:

	Actor* parent = nullptr;
	
	Colour colour = Colour::WHITE;

	Vec2 currentVelocity = { 0,0 };
	Vec2 objectSize = { 1.f, 1.f };

	float maxMagnitude = 5.f;
	
	float currentSpeed = 1.0f;
	float gravity = 0.098f;
	float drag = 0.7f;
	float friction = 0.65f;

	bool isGrounded = true;
	bool isBouncy = true;
	bool isDirty = false;

	bool isStatic = false;

	friend class Actor;
};