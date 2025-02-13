#include "Collider.h"
#include "Actor.h"

Collider::Collider(Actor* parent, float inverseMass)
{
	this->parent = parent;
	position = parent->GetWorldPosition();
	this->invMass = inverseMass;
}

Collider::~Collider()
{
	parent = nullptr;
}

Vec2 Collider::UpdatePos(Vec2 pos)
{
	Vec2 lastPos;
	position = pos;
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
	vecPoints.push_back(Vec2{ position.x - dimensions.x / 2, position.y - dimensions.y / 2 }); //Bottom Left
	vecPoints.push_back(Vec2{ position.x - dimensions.x / 2, position.y + dimensions.y / 2 }); //Top Left
	vecPoints.push_back(Vec2{ position.x + dimensions.x / 2, position.y + dimensions.y / 2 }); //Top Right
	vecPoints.push_back(Vec2{ position.x + dimensions.x / 2, position.y - dimensions.y / 2 }); //Bottom Right

	ConstructPoints(vecPoints);
}

Vec2 BoxCollider::UpdatePos(Vec2 pos)
{
	PolygonCollider::UpdatePos(pos);

	return position;
}

PolygonCollider::PolygonCollider(Actor* parent, float inverseMass) : Collider(parent, inverseMass)
{
}

PolygonCollider::PolygonCollider(Actor* parent, Vec2 position, int numPoints, float inverseMass) : PolygonCollider(parent, inverseMass)
{
	//Angles to rotate around for a perfect shape
	float cosAngle = cos(2 * PI / numPoints);
	float sinAngle = -sin(2 * PI / numPoints);

	std::vector<Vec2> plotPoints;

	Vec2 plotPoint(0, (parent->GetDrawSize().y / 2));

	for (int i = 0; i < numPoints; i++)
	{
		//Rotate, Add point
		plotPoint.RotateBy(cosAngle, sinAngle);
		plotPoints.push_back(position + plotPoint);
	}
	//Create points for update
	ConstructPoints(plotPoints);
}

PolygonCollider::PolygonCollider(Actor* parent, Vec2 position, std::vector<Vec2> points, float inverseMass) : Collider(parent, inverseMass)
{
	this->position = position;
	ConstructPoints(points);
	invMass = inverseMass;
}

Vec2 PolygonCollider::UpdatePos(Vec2 pos)
{
	Collider::UpdatePos(pos);
	UpdatePoints();
	return position;
}

void PolygonCollider::UpdatePoints()
{
	for (int i = 0; i < points.size(); ++i)
	{ //Convert points from direction and magnitude to Vec2 position
		vecPoints[i] = position + (-points[i].dirFromParent * points[i].magnitude);
	}
}

std::vector<Vec2> PolygonCollider::GetPoints()
{
	return vecPoints;
}

void PolygonCollider::ConstructPoints(std::vector<Vec2> points)
{
	vecPoints = points;

	for (Vec2 currentPoint : points) {
		Point calcPoint = Point{ (position - currentPoint).GetNormalised(),(position - currentPoint).GetMagnitude() };
		this->points.push_back(calcPoint);
	}
}
