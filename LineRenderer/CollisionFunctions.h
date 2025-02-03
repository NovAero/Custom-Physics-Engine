#pragma once

#include "Collision.h"

class CircleCollider;
class BoxCollider;
class PolygonCollider;

CollisionInfo CircleToCircleCollision(CircleCollider* a, CircleCollider* b);

CollisionInfo BoxToBoxCollision(BoxCollider* a, BoxCollider* b);

CollisionInfo CircleToPolyCollision(CircleCollider* a, PolygonCollider* b);

CollisionInfo PolyToPolyCollision(PolygonCollider* a, PolygonCollider* b);