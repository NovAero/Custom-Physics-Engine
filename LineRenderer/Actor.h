#pragma once
#include "Maths.h"

class RigidBody;
class Collider;

class Actor {
public:
	Actor();
	Actor(Vec2 pos, );
	Actor(RigidBody* rb, Collider* col);

protected:
	RigidBody* rb;
	Collider* collider;

private:
	Vec2 actorPosition = { 0,0 };

};