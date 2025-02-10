#include "Collider.h"
#include "Actor.h"

Collider::Collider(Actor* parent, float inverseMass)
{
	this->parent = parent;
	this->invMass = inverseMass;
}

Collider::~Collider()
{
	parent = nullptr;
}

Vec2 Collider::UpdatePos()
{
	Vec2 lastPos;
	position = parent->GetWorldPosition();
	return (position - lastPos);
}


CircleCollider::CircleCollider(Actor* parent, Vec2 position, float radius, float inverseMass) : Collider(parent, inverseMass)
{
	this->position = position;
	this->radius = radius;
}

BoxCollider::BoxCollider(Actor* parent, Vec2 position, Vec2 dimensions, float inverseMass) : PolygonCollider(parent, inverseMass)
{
	this->position = position;
	this->dimensions = dimensions;

	//Corners go clockwise from bottom left to bottom right
	points.push_back(Vec2{position.x - (dimensions.x / 2), position.y - (dimensions.y / 2) }); //Bottom Left
	points.push_back(Vec2{position.x - (dimensions.x / 2), position.y + (dimensions.y / 2) }); //Top Left
	points.push_back(Vec2{position.x + (dimensions.x / 2), position.y + (dimensions.y / 2) }); //Top Right
	points.push_back(Vec2{position.x + (dimensions.x / 2), position.y - (dimensions.y / 2) }); //Bottom Right
}

Vec2 BoxCollider::UpdatePos()
{
	Collider::UpdatePos();

	points[0] = { position.x - (dimensions.x / 2), position.y - (dimensions.y / 2) };
	points[1] = { position.x - (dimensions.x / 2), position.y + (dimensions.y / 2) };
	points[2] = { position.x + (dimensions.x / 2), position.y + (dimensions.y / 2) };
	points[3] = { position.x + (dimensions.x / 2), position.y - (dimensions.y / 2) };

	return position;
}

PolygonCollider::PolygonCollider(Actor* parent, float inverseMass) : Collider(parent, inverseMass)
{
}

PolygonCollider::PolygonCollider(Actor* parent, Vec2 position, int numPoints, float inverseMass) : PolygonCollider(parent, inverseMass)
{
	//Angles to rotate around for a perfect shape
	float cosAngle = cos(2 * PI / numPoints);
	float sinAngle = sin(2 * PI / numPoints);

	Vec2 plotPoint(0, -(parent->GetDrawSize().y / 2));

	for (int i = 0; i < numPoints; i++)
	{
		//Rotate, Add point
		plotPoint.RotateBy(cosAngle, sinAngle);
		points.push_back(position + plotPoint);
	}
}

PolygonCollider::PolygonCollider(Actor* parent, Vec2 position, std::vector<Vec2> points, float inverseMass) : Collider(parent, inverseMass)
{
	this->position = position;
	this->points = points;
	invMass = inverseMass;
}

Vec2 PolygonCollider::UpdatePos()
{
	Collider::UpdatePos();
	UpdatePoints();
	return position;
}

void PolygonCollider::UpdatePoints()
{
	//Angles to rotate around for a perfect shape
	float cosAngle = cos(2 * PI / points.size());
	float sinAngle = -sin(2 * PI / points.size());

	Vec2 plotPoint(0, -(parent->GetDrawSize().y / 2));

	for (int i = 0; i < points.size(); i++)
	{
		//Rotate, Add point
		points[i] = position + plotPoint;
		plotPoint.RotateBy(cosAngle, sinAngle);
	}
}

std::vector<Vec2> PolygonCollider::GetPoints()
{
	return points;
}
