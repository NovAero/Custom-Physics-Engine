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

CollisionInfo BoxToBoxCollision(BoxCollider* a, BoxCollider* b) //Deprecated, use poly-poly for OBB
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
		current.Normalise();

		normals.push_back(current);
	}

	//b normals
	for (Vec2 normal : b->GetEdgeNormals()) {
		normals.push_back(normal);
	}

	float smallestDepth = FLT_MAX;
	Vec2 smallestDepthNormal;

	Vec2 overlapPointA;
	Vec2 overlapPointB;

	for (int i = 0; i < normals.size(); ++i) {

		float aMin = FLT_MAX;
		float aMax = -FLT_MAX;
		float bMin = FLT_MAX;
		float bMax = -FLT_MAX;

		//A min and max can be set manually - no for loop needed, there's only 2 points being checked
		aMax = Dot(a->position + (normals[i] * a->radius), normals[i]);
		aMin = Dot(a->position + (-normals[i] * a->radius), normals[i]);

		//B min and max
		for (Vec2 currentPoint : bPoints) {
			float projection = Dot(currentPoint, normals[i]);

			if (projection < bMin) {
				bMin = projection;
			}
			
			if (projection > bMax) {
				bMax = projection;
			}
		}

		float overlapA = aMax - bMin;
		float overlapB = bMax - aMin;

		//Smallest overlap direction and depth
		if (overlapA < smallestDepth) {
			smallestDepth = overlapA;
			smallestDepthNormal = normals[i];
		} 
		if (overlapB < smallestDepth) {
			smallestDepth = overlapB;
			smallestDepthNormal = -normals[i];
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
	//TODO fix contact points to allow for a contact manifold

	CollisionInfo info;
	info.colliderA = a;
	info.colliderB = b;
	
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
	Vec2 smallestDepthNormal;

	Vec2 overlapPointA;
	Vec2 overlapPointB;

	//Find projections on a
	for (int i = 0; i < normals.size(); ++i) {

		float aMin = FLT_MAX;
		float aMax = -FLT_MAX;
		float bMin = FLT_MAX;
		float bMax = -FLT_MAX;

		//A min and max
		for (Vec2 currentPoint : aPoints) {
			float projection = Dot(currentPoint,normals[i]);

			if (projection < aMin) {
				aMin = projection;
			}
			if (projection > aMax) {
				aMax = projection;
			}
		}
		//B min and max
		for (Vec2 currentPoint : bPoints) {
			float projection = Dot(currentPoint, normals[i]);

			if (projection < bMin) {
				bMin = projection;
			}
			if (projection > bMax) {
				bMax = projection;
			}
		}

		float overlapA = aMax - bMin;
		float overlapB = bMax - aMin;

		//Smallest overlap direction and depth
		if (overlapA < smallestDepth) {
			smallestDepth = overlapA;
			smallestDepthNormal = normals[i];
		}
		if (overlapB < smallestDepth) {
			smallestDepth = overlapB;
			smallestDepthNormal = -normals[i];
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
	float second = FLT_MAX;

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