#include "Collider.h"
#include "Actor.h"

Collider::Collider(float inverseMass) : invMass(inverseMass)
{

}

Collider::~Collider()
{
	parent = nullptr;
}

Vec2 Collider::UpdatePos(float delta)
{
	Vec2 lastPos;
	position = parent->GetWorldPosition();
	return (position - lastPos);
}


CircleCollider::CircleCollider(Vec2 position, float radius, float inverseMass) : Collider(inverseMass)
{
	this->position = position;
	this->radius = radius;
}

BoxCollider::BoxCollider(Vec2 position, Vec2 dimensions, float inverseMass) : PolygonCollider(inverseMass)
{
	this->position = position;
	this->dimensions = dimensions;

	//Corners go clockwise from bottom left to bottom right
	points.push_back(Vec2{position.x - (dimensions.x / 2), position.y - (dimensions.y / 2) }); //Bottom Left
	points.push_back(Vec2{position.x - (dimensions.x / 2), position.y + (dimensions.y / 2) }); //Top Left
	points.push_back(Vec2{position.x + (dimensions.x / 2), position.y + (dimensions.y / 2) }); //Top Right
	points.push_back(Vec2{position.x + (dimensions.x / 2), position.y - (dimensions.y / 2) }); //Bottom Right
}

Vec2 BoxCollider::UpdatePos(float delta)
{
	Collider::UpdatePos(delta);

	points[0] = { position.x - (dimensions.x / 2), position.y - (dimensions.y / 2) };
	points[1] = { position.x - (dimensions.x / 2), position.y + (dimensions.y / 2) };
	points[2] = { position.x + (dimensions.x / 2), position.y + (dimensions.y / 2) };
	points[3] = { position.x + (dimensions.x / 2), position.y - (dimensions.y / 2) };

	return position;
}

PolygonCollider::PolygonCollider(float inverseMass) : Collider(inverseMass)
{
}

PolygonCollider::PolygonCollider(Vec2 position, int numPoints, float inverseMass)
{
	Vec2 dir = { 0.f, -1.f };

	for (int i = 0; i <= numPoints; ++i) {
		points.push_back(dir * parent->GetRbSize().x);
		dir = dir.GetRotatedBy(numPoints / i);
	}
}

PolygonCollider::PolygonCollider(Vec2 position, std::vector<Vec2> points, float inverseMass) : Collider(inverseMass)
{
	this->position = position;
	this->points = points;
	invMass = inverseMass;
}

Vec2 PolygonCollider::UpdatePos(float delta)
{
	Vec2 translation = Collider::UpdatePos(delta);
	UpdatePoints(translation * delta);
	return translation;
}

void PolygonCollider::UpdatePoints(Vec2 translation)
{
	for (Vec2& point : points) {
		point += translation;
	}
}

std::vector<Vec2> PolygonCollider::GetPoints()
{
	return points;
}
