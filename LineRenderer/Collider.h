#pragma once
#include "Maths.h"
#include <vector>

class LineRenderer;
class Actor;

class Collider {
public:
	Collider() = default;
	Collider(float inverseMass);
	virtual ~Collider();
	
	Actor* parent = nullptr;
	Vec2 position = { 0,0 };
	float invMass = 0.f;
};

class CircleCollider : public Collider{
public:
	CircleCollider(Vec2 position = { 0,0 }, float radius = 1.f, float inverseMass = 0.f);
	virtual ~CircleCollider() override { Collider::~Collider(); }

public:
	float radius;
};

class PolygonCollider : public Collider {
public:
	PolygonCollider() {};
	PolygonCollider(Vec2 position, std::vector<Vec2> points, float inverseMass);
	virtual ~PolygonCollider() override { Collider::~Collider(); }

	std::vector<Vec2> GetPoints();

protected:
	std::vector<Vec2> points;
};

class BoxCollider : public PolygonCollider {
public:
	BoxCollider(Vec2 position = { 0,0 }, Vec2 dimensions = { 0,0 }, float inverseMass = 0.f);
	virtual ~BoxCollider() override { PolygonCollider::~PolygonCollider(); }

public:
	Vec2 dimensions;
};