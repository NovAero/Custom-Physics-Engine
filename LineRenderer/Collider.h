#pragma once
#include "Maths.h"

class LineRenderer;
class Actor;

class Collider {
public:
	Collider() = default;
	Collider(float inverseMass);
	~Collider();
	
	Actor* parent = nullptr;
	Vec2 position = { 0,0 };
	float invMass = 0.f;
};

class CircleCollider : public Collider {
public:
	CircleCollider();
	CircleCollider(Vec2 position = { 0,0 }, float radius = 1.f, float inverseMass = 0.f);

public:
	float radius;
};

class BoxCollider : public Collider {
public:
	BoxCollider(Vec2 position = { 0,0 }, Vec2 dimensions = { 0,0 }, float inverseMass = 0.f);

public:
	Vec2 points[4];
	Vec2 dimensions;
};