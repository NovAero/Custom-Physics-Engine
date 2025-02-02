#pragma once

#include "Application.h"
#include "Maths.h"
#include <vector>

class RigidBody;

class PhysicsApp: public Application
{
private:
	//Any data that should persist for the duration of your program,
	//declare it here.
	RigidBody* player = nullptr;

	float launchMagnitude = 1.0f;

public:
	
	PhysicsApp();
	~PhysicsApp();
	void Initialise() override;

	void Update(float delta) override;

	void OnLeftClick() override;
	void OnRightClick() override;
	void OnRightRelease() override;
	void OnKeyPress(Key key) override;

};