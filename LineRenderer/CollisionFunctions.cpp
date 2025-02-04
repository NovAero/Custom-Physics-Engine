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

	//Get first and second closest points. First initialised to centre of b to centre of a
	Vec2 first = b->position - a->position;
	Vec2 second = { 0,0 };

	for (auto point : b->GetPoints()) {
		if ((point - a->position).GetMagnitudeSquared() <= (first - a->position).GetMagnitudeSquared()) {
			second = first;
			first = point;
		}
		else if ((point - a->position).GetMagnitudeSquared() <= (second - a->position).GetMagnitudeSquared()) {
			second = point;
		}
	}

	//Get middle of two points
	Vec2 midPoint = GetMidpoint(first, second);
	//From a to midpoint * radius - is overlapping the mid point
	Vec2 aToMid = (midPoint - a->position).GetNormalised() * a->radius;
	Vec2 aToFirst = (first - a->position).GetNormalised() * a->radius;
	Vec2 aToSecond = (second - a->position).GetNormalised() * a->radius;

	//Useful for the edge case where its not over first point, and needs
	//to be pushed along first dot second
	bool overMidPoint = false;
	//If over first point
	bool overFirstPoint = false;
	//if over second
	bool overSecondPoint = false;

	//if the circle overlaps the first point
	if ((first - a->position).GetMagnitudeSquared() < aToFirst.GetMagnitudeSquared()) {
		overFirstPoint = true;
	}
	//if the circle overlaps the midpoint of first and second
	if ((midPoint - a->position).GetMagnitudeSquared() < aToMid.GetMagnitudeSquared()) {
		overMidPoint = true;
	}
	if ((second - a->position).GetMagnitudeSquared() < aToSecond.GetMagnitudeSquared()) {
		overSecondPoint = true;
	}

	Vec2 saveVec;

	//Check edge cases
	if (overFirstPoint && overSecondPoint) { //if true it is over midpoint aswell so no need to check
		info.collisionNormal = aToMid.GetNormalised();
		saveVec = midPoint;
	}
	else if (overFirstPoint && !overSecondPoint) {
		if (overMidPoint) { //Get middle of first and middle then get direction to it
			Vec2 qtrMid = GetMidpoint(first, midPoint);
			saveVec = qtrMid;
			info.collisionNormal = (qtrMid - a->position).GetNormalised(); //middle of first and midpoint to a
		}
		else { //only over first 
			saveVec = first;
			info.collisionNormal = aToFirst.GetNormalised(); //first to a
		}
	}
	else if (overMidPoint) {
		//Only over Midpoint
		info.collisionNormal = aToMid.GetNormalised(); //midpoint to a
		saveVec = midPoint;
	}

	if (b->position.x + midPoint.x > a->position.x + aToMid.x) {
		info.collisionNormal = aToMid.GetNormalised();
	}
	else if (b->position.y + midPoint.y > a->position.y + aToMid.y) {
		info.collisionNormal = aToMid.GetNormalised();
	}
	
	if (overFirstPoint || overSecondPoint || overMidPoint) {
		//distance of a to its radius in direction - the distance of a to point
		float overlap = a->radius - (a->position - saveVec).GetMagnitude();
		info.overlapAmount = overlap;
	}

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

	Vec2 closestV2ToA, closestV2ToB;
	std::vector<Vec2> aPts = a->GetPoints();
	std::vector<Vec2> bPts = b->GetPoints();

	//Check closest to centre of A
	for (int j = 0; j < b->GetPoints().size(); ++j) {
		if ((bPts[j] - a->position).GetMagnitudeSquared() < (closestV2ToA - a->position).GetMagnitudeSquared()) {
			closestV2ToA = bPts[j];
		}
	}
	//Check closest to centre of B
	for (int i = 0; i < a->GetPoints().size(); ++i) {
		if ((aPts[i] - b->position).GetMagnitudeSquared() < (closestV2ToB - b->position).GetMagnitudeSquared()) {
			closestV2ToB = aPts[i];
		}
	}


	//Get direction FROM a TO B, and the depth between the two closest vertices
	info.collisionNormal = (closestV2ToB - closestV2ToA).GetNormalised();
	info.overlapAmount = -(closestV2ToB - closestV2ToA).GetMagnitude();

	return info;
}

Vec2 GetMidpoint(Vec2 a, Vec2 b)
{
	Vec2 direction = (b-a).GetNormalised();
	Vec2 midPoint = direction * ((b-a).GetMagnitude()/2);
	
	return midPoint;
}
