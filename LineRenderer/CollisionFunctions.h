#pragma once

#include "Collision.h"
#include <vector>

class CircleCollider;
class BoxCollider;
class PolygonCollider;

CollisionInfo CircleToCircleCollision(CircleCollider* a, CircleCollider* b);

CollisionInfo BoxToBoxCollision(BoxCollider* a, BoxCollider* b);

CollisionInfo CircleToPolyCollision(CircleCollider* a, PolygonCollider* b);

CollisionInfo PolyToPolyCollision(PolygonCollider* a, PolygonCollider* b);

Vec2 GetMidpoint(Vec2 a, Vec2 b);

Vec2 FindClosestPoint(std::vector<Vec2> points, Vec2 point);
void FindClosestPoints(std::vector<Vec2>points, Vec2 point, Vec2& firstClosest, Vec2& secondClosest);