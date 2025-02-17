#pragma once
#include "Maths.h"
#include "LineRenderer.h"

class Actor;

const float GRAVITY = 9.8f;
const Vec2 GRAVITY_VEC2 = { 0, -GRAVITY };

class RigidBody {
public:
	RigidBody();
	//Dynamic Constructors
	RigidBody(Actor* parent);
	RigidBody(Actor* parent, Vec2 size);
	RigidBody(Actor* parent, float drag);
	RigidBody(Actor* parent, float drag, Vec2 size);
	
	~RigidBody();

	virtual void Update(float delta, Vec2 cursorPos);

	void ApplyImpulse(Vec2 direction, float magnitude);
	void ApplyImpulse(Vec2 impulse);
	void ApplyConstantForce(Vec2 direction, float magnitude, float& timer, float& currentTime, float delta);
	void SetVelocity(Vec2 velocity);
	void Bounce(Vec2 tangent);

	void HandleResistances(float delta);
	Vec2 HandleSurfaceFriction(float friction, float delta);

	bool GetIsDirty()const { return isDirty; }
	Vec2 GetSize() const { return objectSize; }
	float GetCurrentSpeed() const { return currentSpeed; }
	Vec2 GetCurrentVelocity() const { return currentVelocity; }
	float GetTerminalVelocity() const { return maxMagnitude; }
	bool GetIsStatic() const { return isStatic; }

	void SetIsDirty(bool dirty) { isDirty = dirty; }
	void SetIsStatic(bool _static) { isStatic = _static; }
	void SetShouldApplyFriction(bool b) { shouldApplyFriction = b; }
	void SetElasticityPerc(float ePercent);


	//Debug
	Vec2 frictionDirection = { 1,0 };

protected:

	Actor* parent = nullptr;
	
	Colour colour = Colour::WHITE;

	Vec2 currentVelocity = { 0,0 };
	Vec2 objectSize = { 1.f, 1.f };

	float maxMagnitude = 10.f;
	
	float currentSpeed = 1.0f;
	float drag = 0.5f;

	bool shouldApplyFriction = false;
	bool isDirty = false;

	bool isStatic = false;

	friend class Actor;
};
