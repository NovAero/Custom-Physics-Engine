#include "PhysicsApp.h"
#include "LineRenderer.h"
#include "Key.h"
#include "RigidBody.h"
#include "CollisionFunctions.h"
#include "Collision.h"
#include "Collider.h"

PhysicsApp::PhysicsApp()
{

}

PhysicsApp::~PhysicsApp()
{
	if (player != nullptr) {
		delete player;
	}
	objects.clear();
}

void PhysicsApp::Initialise()
{
	player = new Actor(Vec2{ 1.7,1.7 }, ObjectShape::TRIANGLE, Vec2{2.f,2.f});

	objects.push_back(player);

	for (int i = 1; i < 6; ++i) {
		objects.push_back(new Actor(Vec2{1.f, (float)(i * 2)}, CIRCLE, Vec2(i, i)));
	}
	for (int i = 1; i < 6; ++i) {
		objects.push_back(new Actor(Vec2{ 10.f, (float)(i * 2) }, SQUARE, Vec2(i, i)));
	}
}

void PhysicsApp::Update(float delta)
{
	collisions.clear();

	for (int i = 0; i < objects.size(); i++)
	{
		for (int j = i + 1; j < objects.size(); j++)
		{
			QueueCollision(&objects[i]->GetCollider(), &objects[j]->GetCollider());
		}
	}
	for (CollisionInfo& thisInfo : collisions)
	{
		thisInfo.Resolve();
	}

	for (int i = 0; i < objects.size(); ++i) {
		objects[i]->Update(delta, cursorPos);
	}

	//Draw line and set magnitude for launch
	if (rightMouseDown && player->GetWorldPosition().y > -10.0f) {
		float DistanceToCur = (player->GetWorldPosition() - cursorPos).GetMagnitude();

		launchMagnitude = DistanceToCur >= player->GetRigidBody().GetTerminalVelocity() ? player->GetRigidBody().GetTerminalVelocity() : (player->GetWorldPosition() - cursorPos).GetMagnitude();
		lines->DrawLineWithArrow(player->GetWorldPosition(), player->GetWorldPosition() + (Vec2(player->GetWorldPosition() - cursorPos).Normalise()) * launchMagnitude, Colour::RED);
	}
	else if(rightMouseDown && player->GetWorldPosition().y <= -10.0f) {
		float DistanceToCur = (player->GetWorldPosition() - cursorPos).GetMagnitude();

		launchMagnitude = DistanceToCur >= player->GetRigidBody().GetTerminalVelocity() ? player->GetRigidBody().GetTerminalVelocity() : (player->GetWorldPosition() - cursorPos).GetMagnitude();
		lines->DrawLineWithArrow(Vec2{ player->GetWorldPosition().x ,-10.0f }, Vec2{ player->GetWorldPosition().x, -10.0f } + (Vec2(player->GetWorldPosition() - cursorPos).Normalise()) * launchMagnitude, Colour::RED);
	}
	
	for (int i = 0; i < objects.size(); ++i){
		objects[i]->Draw(lines);
	}
}

void PhysicsApp::QueueCollision(Collider* a, Collider* b)
{

	CollisionInfo thisHit;

	CircleCollider* circA = dynamic_cast<CircleCollider*>(a);
	CircleCollider* circB = dynamic_cast<CircleCollider*>(b);

	PolygonCollider* polyA = nullptr;
	PolygonCollider* polyB = nullptr;

	if (circA != nullptr && circB != nullptr) {
		thisHit = CircleToCircleCollision(circA, circB);
	}
	else if (circA != nullptr && circB == nullptr) {

		polyB = dynamic_cast<PolygonCollider*>(b);
		if (polyB != nullptr) {
			thisHit = CircleToPolyCollision(circA, polyB);
		}
	}
	else if (circA == nullptr && circB != nullptr) {
		
		polyA = dynamic_cast<PolygonCollider*>(a);
		if (polyA != nullptr) {
			thisHit = CircleToPolyCollision(circB, polyA);
		}
	}
	else {
		polyA = dynamic_cast<PolygonCollider*>(a);
		polyB = dynamic_cast<PolygonCollider*>(b);

		thisHit = PolyToPolyCollision(polyA, polyB);
	}

	if (thisHit.IsOverlapping()) {
		collisions.push_back(thisHit);
	}
}

void PhysicsApp::OnLeftClick()
{
	player->GetRigidBody().SetPosition(cursorPos);
	player->GetRigidBody().SetVelocity(Vec2{ 0,0 });
	player->GetRigidBody().SetIsDirty(false);
}

void PhysicsApp::OnRightClick()
{
	rightMouseDown = true;
	
}

void PhysicsApp::OnRightRelease()
{
	Vec2 Direction = player->GetWorldPosition() - cursorPos;
	Direction.Normalise();

	player->GetRigidBody().ApplyImpulse(Direction, launchMagnitude);

	rightMouseDown = false;

}

void PhysicsApp::OnKeyPress(Key key)
{

}