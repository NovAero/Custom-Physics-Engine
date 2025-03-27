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
	void Stop();

	void HandleResistances(float delta);

	bool GetIsActive()const { return isActive; }
	Vec2 GetSize() const { return objectSize; }
	Vec2 GetCurrentVelocity() const { return currentVelocity; }
	float GetTerminalVelocity() const { return maxMagnitude; }
	bool GetIsStatic() const { return isStatic; }

	void SetActive(bool active) { isActive = active; }
	void SetIsStatic(bool _static) { isStatic = _static; }

protected:

	Actor* parent = nullptr;
	
	Colour colour = Colour::WHITE;

	Vec2 currentVelocity = { 0,0 };
	float maxMagnitude = 10.f;
	Vec2 objectSize = { 1.f, 1.f };
	
	float drag = 0.5f;

	bool isActive = false;

	bool isStatic = false;

	friend class Actor;
};
