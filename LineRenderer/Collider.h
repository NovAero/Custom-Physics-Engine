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

	virtual Vec2 UpdatePos(float delta);

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
	PolygonCollider(float inverseMass);
	PolygonCollider(Vec2 position, int numPoints, float inverseMass);
	PolygonCollider(Vec2 position, std::vector<Vec2> points, float inverseMass);
	virtual ~PolygonCollider() override { Collider::~Collider(); }

	virtual Vec2 UpdatePos(float delta) override;
	void UpdatePoints(Vec2 translation);

	std::vector<Vec2> GetPoints();

protected:
	std::vector<Vec2> points;
};

class BoxCollider : public PolygonCollider {
public:
	BoxCollider(Vec2 position = { 0,0 }, Vec2 dimensions = { 0,0 }, float inverseMass = 0.f);
	virtual ~BoxCollider() override { PolygonCollider::~PolygonCollider(); }
	virtual Vec2 UpdatePos(float delta) override;

public:
	Vec2 dimensions;
};