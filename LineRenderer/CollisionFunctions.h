#pragma once

#include "Collision.h"

class CircleCollider;
class BoxCollider;

CollisionInfo GetCircleOverlapAmount(CircleCollider* a, CircleCollider* b);

CollisionInfo GetRectDepenetration(BoxCollider& a, BoxCollider& b);