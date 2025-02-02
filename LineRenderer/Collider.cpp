#include "Collider.h"

Collider::Collider(float inverseMass) : invMass(inverseMass)
{
}

BoxCollider::BoxCollider(Vec2 position, Vec2 dimensions, float inverseMass) : Collider(inverseMass)
{
	this->position = position;
	this->dimensions = dimensions;

	//Corners go clockwise from bottom left to bottom right
	points[0] = { position.x - (dimensions.x / 2), position.y - (dimensions.y / 2) }; //Bottom Left
	points[1] = { position.x - (dimensions.x / 2), position.y + (dimensions.y / 2) }; //Top Left
	points[2] = { position.x + (dimensions.x / 2), position.y + (dimensions.y / 2) }; //Top Right
	points[3] = { position.x + (dimensions.x / 2), position.y - (dimensions.y / 2) }; //Bottom Right
}

CircleCollider::CircleCollider(Vec2 position, float radius, float inverseMass) : Collider(inverseMass)
{
	this->position = position;
	this->radius = radius;
}
