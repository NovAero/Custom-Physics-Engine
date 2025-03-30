#pragma once
#include "Maths.h"
#include <vector>

class LineRenderer;
class Actor;

class Collider {
public:
	Collider() = default;
	Collider(Actor* parent,  float inverseMass);
	virtual ~Collider();

	virtual Vec2 UpdatePos(Vec2 pos);
	virtual void Rotate(float deg);

	float elasticity = 0.6f;

	Actor* parent = nullptr;
	Vec2 position = { 0,0 };
	float invMass = 0.f;
};

class CircleCollider : public Collider{
public:
	CircleCollider(Actor* parent, Vec2 position = { 0,0 }, float radius = 1.f, float inverseMass = 0.f);
	virtual ~CircleCollider() override { Collider::~Collider(); }

public:
	float radius;
};

class PolygonCollider : public Collider {
public:
	PolygonCollider(Actor* parent, float inverseMass);
	PolygonCollider(Actor* parent, Vec2 position, int numPoints, float inverseMass);
	PolygonCollider(Actor* parent, Vec2 position, std::vector<Vec2> points, float inverseMass);
	virtual ~PolygonCollider() override { Collider::~Collider(); }

	virtual Vec2 UpdatePos(Vec2 pos) override;
	void UpdatePoints();

	virtual void Rotate(float deg) override;

	std::vector<Vec2> GetPoints();
	std::vector<Vec2> GetEdgeNormals();

protected:
	void ConstructPoints(std::vector<Vec2> points);

protected:
	std::vector<Vec2> cachedWorldPoints;
	std::vector<Vec2> vecsFromParent;
	std::vector<Vec2> edgeNormals;
};

class BoxCollider : public PolygonCollider {
public:
	BoxCollider(Actor* parent, Vec2 position = { 0,0 }, Vec2 dimensions = { 0,0 }, float inverseMass = 0.f);
	virtual ~BoxCollider() override { PolygonCollider::~PolygonCollider(); }
	virtual Vec2 UpdatePos(Vec2 pos) override;

public:
	Vec2 dimensions;
};