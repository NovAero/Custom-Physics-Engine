#include "CollisionFunctions.h"
#include "Collider.h"

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

	Vec2 overlapDirection = { (v1Disp - v2Disp), (h1Disp - h2Disp) };
	
	//Overlap Depth
	info.overlapAmount = overlapDirection.x < overlapDirection.y ? overlapDirection.x : overlapDirection.y;
	//Collision Direction
	info.collisionNormal = overlapDirection.Normalise();

	return info;
}

CollisionInfo CircleToPolyCollision(CircleCollider* a, PolygonCollider* b)
{
	CollisionInfo info;

	info.colliderA = a;
	info.colliderB = b;

	Vec2 closestPoint;

	//For each of the points in the polygon, get the closest point to the centre of the circle
	for (int i = 0; i < b->GetPoints().size(); ++i) {
		if ((b[i].position - a->position).GetMagnitudeSquared() < (closestPoint - a->position).GetMagnitudeSquared()) {
			closestPoint = b[i].position;
		}
	}

	//Get direction and depth of overlap
	info.collisionNormal = (closestPoint - a->position).GetNormalised();
	info.overlapAmount = a->radius - (closestPoint - a->position).GetMagnitude();

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
	else {
		boxA = nullptr;
		boxB = nullptr;
	}

	Vec2 closestV2ToA, closestV2ToB;
	std::vector<Vec2> aPts = a->GetPoints();
	std::vector<Vec2> bPts = b->GetPoints();

	//Check closest to centre of B
	for (int i = 0; i < a->GetPoints().size(); ++i) {
		if ((aPts[i] - b->position).GetMagnitudeSquared() < (closestV2ToB - b->position).GetMagnitudeSquared()) {
			closestV2ToB = aPts[i];
		}
	}

	//Check closest to centre of A
	for (int j = 0; j < b->GetPoints().size(); ++j) {
		if ((bPts[j] - a->position).GetMagnitudeSquared() < (closestV2ToA - a->position).GetMagnitudeSquared()) {
			closestV2ToA = bPts[j];
		}
	}

	//Get direction FROM a TO B, and the depth between the two closest vertices
	info.collisionNormal = (closestV2ToB - closestV2ToA).GetNormalised();
	info.overlapAmount = (closestV2ToB - closestV2ToA).GetMagnitude();

	return info;
}
