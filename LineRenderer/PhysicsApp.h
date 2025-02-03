#pragma once

#include "Application.h"
#include "Maths.h"
#include <vector>
#include "Actor.h"

class CollisionInfo;

class PhysicsApp: public Application
{
private:
	//Any data that should persist for the duration of your program,
	//declare it here.
	Actor* player = nullptr;

	std::vector<Actor*> objects;
	std::vector<CollisionInfo> collisions;

	float launchMagnitude = 1.0f;

public:
	
	PhysicsApp();
	~PhysicsApp();
	void Initialise() override;

	void Update(float delta) override;

	void QueueCollision(Collider* a, Collider* b);

	void OnLeftClick() override;
	void OnRightClick() override;
	void OnRightRelease() override;
	void OnKeyPress(Key key) override;

};