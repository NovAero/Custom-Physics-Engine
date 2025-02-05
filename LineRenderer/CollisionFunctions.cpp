#include "CollisionFunctions.h"
#include "Collider.h"
#include "Actor.h"

CollisionInfo CircleToCircleCollision(CircleCollider* a, CircleCollider* b)
{
	CollisionInfo info;
	info.colliderA = a;
	info.colliderB = b;

	Vec2 centreDisplacement = b->position - a->position;
	float distance = centreDisplacement.GetMagnitude();
	float separationOfSurfaces = distance - a->radius - b->radius;
	info.overlapAmount = -separationOfSurfaces;

	info.collisionNormal = centreDisplacement / distance;

	return info;
}

CollisionInfo BoxToBoxCollision(BoxCollider* a, BoxCollider* b)
{
	CollisionInfo info;
	info.colliderA = a;
	info.colliderB = b;

	//Top edge of b past a bottom
	float v1Disp = ((b->position.y + b->dimensions.y) - a->position.y);
	//Top edge of a past b bottom
	float v2Disp = ((a->position.y + a->dimensions.y) - b->position.y);
	//Right edge of b vs left of a
	float h1Disp = ((b->position.x + b->dimensions.x) - a->position.x);
	//Right edge of a vs left of b
	float h2Disp = ((a->position.x + a->dimensions.x) - b->position.x);

	Vec2 overlapDirection;

	if (v1Disp < v2Disp && v1Disp < h1Disp && v1Disp < h2Disp) {

		overlapDirection = { 0,-1.f };
		info.overlapAmount = v1Disp;

	}
	else if (v2Disp < h1Disp && v2Disp < h2Disp) {
		overlapDirection = { 0,1.f };
		info.overlapAmount = v2Disp;
	}
	else if (h1Disp < h2Disp) {
		overlapDirection = { -1.f,0 };
		info.overlapAmount = h1Disp;
	}
	else {
		overlapDirection = { 1.f,0 };
		info.overlapAmount = h2Disp;
	}
	
	//Collision Direction
	info.collisionNormal = overlapDirection;

	return info;
}

CollisionInfo CircleToPolyCollision(CircleCollider* a, PolygonCollider* b)
{
	CollisionInfo info;

	info.colliderA = a;
	info.colliderB = b;

	//TODO SAT check code - watch recording from 05/02

	float min1 = FLT_MAX;
	float max1 = -FLT_MAX;

	std::vector<Vec2> normals;


	return info;
}

CollisionInfo PolyToPolyCollision(PolygonCollider* a, PolygonCollider* b)
{
	CollisionInfo info;
	info.colliderA = a;
	info.colliderB = b;

	BoxCollider* boxA = dynamic_cast<BoxCollider*>(a);
	BoxCollider* boxB = dynamic_cast<BoxCollider*>(b);

	if (boxA != nullptr && boxB != nullptr) {
		info = BoxToBoxCollision(boxA, boxB);
		return info;
	}

	//TODO SAT check code - watch recording from 05/02

	float min1 = FLT_MAX;
	float max1 = -FLT_MAX;

	std::vector<Vec2> normals;

	//PolyA's normals
	for (int i = 0; i < a->GetPoints().size(); ++i) {

		Vec2 current = a->GetPoints()[(i+1)% a->GetPoints().size()] - a->GetPoints()[i];
		current.Normalise().RotateBy90();

		normals.push_back(current);
	}

	//PolyB's normals
	for (int i = 0; i < b->GetPoints().size(); ++i) {

		Vec2 current = b->GetPoints()[(i + 1) % b->GetPoints().size()] - b->GetPoints()[i];
		current.Normalise().RotateBy90();

		normals.push_back(current);
	}

	//TODO sort out distances on normals

	return info;
}

Vec2 GetMidpoint(Vec2 a, Vec2 b)
{
	Vec2 direction = (b-a).GetNormalised();
	Vec2 midPoint = direction * ((b-a).GetMagnitude()/2);
	
	return midPoint;
}
