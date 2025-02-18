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

void Collider::Rotate(float deg)
{
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
	cachedWorldPoints.push_back(Vec2{ position.x - dimensions.x / 2, position.y - dimensions.y / 2 }); //Bottom Left
	cachedWorldPoints.push_back(Vec2{ position.x - dimensions.x / 2, position.y + dimensions.y / 2 }); //Top Left
	cachedWorldPoints.push_back(Vec2{ position.x + dimensions.x / 2, position.y + dimensions.y / 2 }); //Top Right
	cachedWorldPoints.push_back(Vec2{ position.x + dimensions.x / 2, position.y - dimensions.y / 2 }); //Bottom Right

	ConstructPoints(cachedWorldPoints);
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
	float cosAngle = cosf(2 * PI / numPoints);
	float sinAngle = -sinf(2 * PI / numPoints);

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
	for (int i = 0; i < vecsFromParent.size(); ++i)
	{ //Convert points from direction and magnitude to Vec2 position
		cachedWorldPoints[i] = position + (vecsFromParent[i]);
	}
}

void PolygonCollider::Rotate(float deg)
{
	float rad = DegToRad(deg);

	for (Vec2& thisPoint : vecsFromParent) {
		thisPoint.RotateBy(rad);
	}
	for (Vec2& thisNormal : edgeNormals) {
		thisNormal.RotateBy(rad);
	}

	UpdatePoints();
}

std::vector<Vec2> PolygonCollider::GetPoints()
{
	return cachedWorldPoints;
}

std::vector<Vec2> PolygonCollider::GetEdgeNormals()
{
	return edgeNormals;
}

void PolygonCollider::ConstructPoints(std::vector<Vec2> points)
{
	cachedWorldPoints = points;

	for (Vec2 currentPoint : points) {
		Vec2 calcPoint = position - currentPoint;
		this->vecsFromParent.push_back(calcPoint);
	}

	for (int i = 0; i < cachedWorldPoints.size(); ++i) {

		int j = ( i + 1) % cachedWorldPoints.size();

		Vec2 current = cachedWorldPoints[j] - cachedWorldPoints[i];
		
		current.Normalise().RotateBy90();
		edgeNormals.push_back(current);
	}
}
