#include "Collider.h"
#include "Actor.h"

Collider::Collider(float inverseMass) : invMass(inverseMass)
{
	parent = nullptr;
}

Collider::~Collider()
{
	parent = nullptr;
}


CircleCollider::CircleCollider(Vec2 position, float radius, float inverseMass) : Collider(inverseMass)
{
	this->position = position;
	this->radius = radius;
}

BoxCollider::BoxCollider(Vec2 position, Vec2 dimensions, float inverseMass)
{
	this->position = position;
	this->dimensions = dimensions;

	//Corners go clockwise from bottom left to bottom right
	points.push_back(Vec2{position.x - (dimensions.x / 2), position.y - (dimensions.y / 2)}); //Bottom Left
	points.push_back(Vec2{position.x - (dimensions.x / 2), position.y + (dimensions.y / 2) }); //Top Left
	points.push_back(Vec2{position.x + (dimensions.x / 2), position.y + (dimensions.y / 2) }); //Top Right
	points.push_back(Vec2{position.x + (dimensions.x / 2), position.y - (dimensions.y / 2) }); //Bottom Right
}

PolygonCollider::PolygonCollider(Vec2 position, std::vector<Vec2> points, float inverseMass)
{
	this->position = position;
	this->points = points;
	invMass = inverseMass;
}

std::vector<Vec2> PolygonCollider::GetPoints()
{
	return points;
}
