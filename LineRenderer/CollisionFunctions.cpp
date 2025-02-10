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
	//Essentially a poly to poly collision where the normal for the SAT is
	//from the centre of the circle to the closest point to the centre of a in the poly
	
	CollisionInfo info;

	info.colliderA = a;
	info.colliderB = b;

	float aMin = FLT_MAX;
	float aMax = -FLT_MAX;
	float bMin = FLT_MAX;
	float bMax = -FLT_MAX;

	Vec2 closestPoint = { FLT_MAX, FLT_MAX };

	//Get closest point to centre
	for (int i = 0; i < b->GetPoints().size(); ++i) {

		if (closestPoint.GetMagnitudeSquared() > (b->GetPoints()[i] - a->position).GetMagnitudeSquared()) {
			closestPoint = b->GetPoints()[i];
		}
	}

	//Get vector FROM a pos to B closest point
	Vec2 dirToClosest = closestPoint - a->position;
	//Normalise to make purely directional
	dirToClosest.Normalise();


	//Along the normal, get the max/min of a and b
	
	//A min and max can be set manually
	aMax = Dot(a->position + (dirToClosest * a->radius), dirToClosest);
	aMin = Dot(a->position + (-dirToClosest * a->radius), dirToClosest);
	
	//B min and max
	for (Vec2 currentPoint : b->GetPoints()) {
		float projection = Dot(currentPoint, dirToClosest);

		if (projection < bMin) bMin = projection;
		if (projection > bMax) bMax = projection;
	}

	float overlapA = aMax - bMin;
	float overlapB = bMax - aMin;

	//Smallest overlap direction and depth
	if (overlapA < overlapB) {
		info.overlapAmount = overlapA;
	}
	else {
		info.overlapAmount = overlapB;
	}

	info.collisionNormal = dirToClosest;

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
		current.Normalise().RotateBy270();

		normals.push_back(current);
	}

	float smallestDepth = FLT_MAX;
	Vec2 smallestDepthNormal;

	//Find projections
	for (Vec2 currentNormal : normals) {

		float aMin = FLT_MAX;
		float aMax = -FLT_MAX;
		float bMin = FLT_MAX;
		float bMax = -FLT_MAX;
		//A min and max
		for (Vec2 currentPoint : a->GetPoints()) {
			float projection = Dot(currentPoint, currentNormal);

			if (projection < aMin) aMin = projection;
			if (projection > aMax) aMax = projection;
		}
		//B min and max
		for (Vec2 currentPoint : b->GetPoints()) {
			float projection = Dot(currentPoint, currentNormal);

			if (projection < bMin) bMin = projection;
			if (projection > bMax) bMax = projection;
		}

		float overlapA = aMax - bMin;
		float overlapB = bMax - aMin;

		//Smallest overlap direction and depth
		if (overlapA < smallestDepth) {
			smallestDepth = overlapA;
			smallestDepthNormal = currentNormal;
		}
		if (overlapB < smallestDepth) {
			smallestDepth = overlapB;
			smallestDepthNormal = -currentNormal;
		}
	}
	
	info.overlapAmount = smallestDepth;
	info.collisionNormal = smallestDepthNormal;

	return info;
}

Vec2 GetMidpoint(Vec2 a, Vec2 b)
{
	Vec2 direction = (b-a).GetNormalised();
	Vec2 midPoint = direction * ((b-a).GetMagnitude()/2);
	
	return midPoint;
}
