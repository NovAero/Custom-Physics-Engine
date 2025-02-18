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

	std::vector<Vec2> normals;

	std::vector<Vec2> bPoints = b->GetPoints();

	//Get normals from a to b points
	for (Vec2 currentPoint : bPoints) {
		Vec2 current = currentPoint - a->position;
		current.Normalise().RotateBy90();

		normals.push_back(current);
	}

	//b normals
	for (Vec2 normal : b->GetEdgeNormals()) {
		normals.push_back(normal);
	}

	float smallestDepth = FLT_MAX;
	Vec2 smallestDepthNormal;

	for (Vec2 currentNormal : normals) {

		Vec2 maxPoint, minPoint;
		
		float aMin = FLT_MAX;
		float aMax = -FLT_MAX;
		float bMin = FLT_MAX;
		float bMax = -FLT_MAX;

		//A min and max can be set manually - no for loop needed, there's only 2 points being checked
		aMax = Dot(a->position + (currentNormal * a->radius), currentNormal);
		aMin = Dot(a->position + (-currentNormal * a->radius), currentNormal);

		//B min and max
		for (Vec2 currentPoint : bPoints) {
			float projection = Dot(currentPoint, currentNormal);

			if (projection < bMin) {
				bMin = projection;
				minPoint = currentPoint;
			}
			
			if (projection > bMax) {
				bMax = projection;
				maxPoint = currentPoint;
			}
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

	if (smallestDepth == FLT_MAX) { //in case of breaks in positional data
		smallestDepth = -1.f;
		smallestDepthNormal = { 0,1 };
	}
	
	info.overlapAmount = smallestDepth;
	info.collisionNormal = smallestDepthNormal;

	return info;
}


CollisionInfo PolyToPolyCollision(PolygonCollider* a, PolygonCollider* b)
{
	CollisionInfo info;
	info.colliderA = a;
	info.colliderB = b;
	
	if (a->invMass == 0.f && b->invMass == 9.f) {
		return info;
	}
	
	std::vector<Vec2> normals;

	std::vector<Vec2> aPoints = a->GetPoints();
	std::vector<Vec2> bPoints = b->GetPoints();

	//PolyA's normals
	for (Vec2 normal : a->GetEdgeNormals()) {
		normals.push_back(normal);
	}

	//PolyB's normals
	for (Vec2 normal : b->GetEdgeNormals()) {
		normals.push_back(normal);
	}

	float smallestDepth = FLT_MAX;
	int smallestNormalIndex = INT_MAX;
	Vec2 smallestDepthNormal;

	Vec2 overlapPointA;
	Vec2 overlapPointB;

	//Find projections on a
	for (int i = 0; i < normals.size(); ++i) {

		Vec2 closestPoints[4];
		
		float aMin = FLT_MAX;
		float aMax = -FLT_MAX;
		float bMin = FLT_MAX;
		float bMax = -FLT_MAX;

		//A min and max
		for (Vec2 currentPoint : aPoints) {
			float projection = Dot(currentPoint,normals[i]);

			if (projection < aMin) {
				aMin = projection;
				closestPoints[0] = currentPoint;
			}
			if (projection > aMax) {
				aMax = projection;
				closestPoints[1] = currentPoint;
			}
		}
		//B min and max
		for (Vec2 currentPoint : bPoints) {
			float projection = Dot(currentPoint, normals[i]);

			if (projection < bMin) {
				bMin = projection;
				closestPoints[2] = currentPoint;
			}
			if (projection > bMax) {
				bMax = projection;
				closestPoints[3] = currentPoint;
			}
		}

		float overlapA = aMax - bMin;
		float overlapB = bMax - aMin;

		//Smallest overlap direction and depth
		if (overlapA < smallestDepth) {
			smallestDepth = overlapA;
			smallestDepthNormal = normals[i];
			smallestNormalIndex = i;
			overlapPointA = closestPoints[1];
			overlapPointB = closestPoints[2];
		}
		if (overlapB < smallestDepth) {
			smallestDepth = overlapB;
			smallestDepthNormal = -normals[i];
			smallestNormalIndex = i;
			overlapPointB = closestPoints[3];
			overlapPointA = closestPoints[0];
		}
	}

	//With two closest points and smallest normal, rotate normal back to a point to point
	//directional and dot against it to get collision point on edge
	if (smallestNormalIndex < a->GetEdgeNormals().size()) { 
		//if normal is coming from a, overlapPointA will be a vertex
		Vec2 direction = smallestDepthNormal.GetRotatedBy270();
		//Get the dot between the distance a to b point on the normal rotated to tangent
		float dottedMag = Dot(aPoints[smallestNormalIndex], direction) - Dot(overlapPointB, direction);

		info.contactPoints[0] = overlapPointB + (direction * dottedMag);
	}
	else {
		//if normal is coming from b, overlapPointB will be a vertex
		Vec2 direction = smallestDepthNormal.GetRotatedBy270();
		//Get the dot between the distance b to a point on the normal rotated to tangent
		float dottedMag = Dot(overlapPointA, direction) - Dot(overlapPointB, direction);

		info.contactPoints[0] = overlapPointB + (direction * dottedMag);
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

Vec2 FindClosestPoint(std::vector<Vec2> points, Vec2 point)
{
	float closest = FLT_MAX;
	Vec2 closestPoint;

	for (Vec2 currentPoint : points) {
		float dist = (point - currentPoint).GetMagnitudeSquared();

		closestPoint = dist < closest ? currentPoint : closestPoint;
	}

	return closestPoint;
}

void FindClosestPoints(std::vector<Vec2> points, Vec2 point, Vec2& firstClosest, Vec2& secondClosest)
{
	float first = FLT_MAX;
	float second;

	for (Vec2 currentPoint : points) {
		float distSqr = (currentPoint - point).GetMagnitudeSquared();

		if ( distSqr < first) {
			second = first;
			first = distSqr;
			secondClosest = firstClosest;
			firstClosest = currentPoint;
		}
		else if (distSqr < second) {
			second = distSqr;
			secondClosest = currentPoint;
		}
	}
}