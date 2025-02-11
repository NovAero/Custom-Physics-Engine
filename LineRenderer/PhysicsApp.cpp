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
	player = new Actor(Vec2{ 4,4 }, ObjectShape::CIRCLE, Vec2{9,2});
	player->colour = Colour::GREEN;
	player->GetCollider().surfaceFriction = 0.2f;

	objects.push_back(player);
	/*
	Actor* box1 = new Actor(Vec2{ 5,5 }, ObjectShape::TRIANGLE, Vec2{ 20,7 });

	objects.push_back(box1);*/

	objects.push_back(new Actor(Vec2{ 0.f, 0.f }, ObjectShape::SQUARE, Vec2{ 100, 5 }));
	objects[1]->GetCollider().surfaceFriction = 0.1f;
	objects[1]->SetInverseMass(0.f);
	objects[1]->GetRigidBody().SetIsStatic(true);

	objects.push_back(new Actor(Vec2{ 50.f, 50.f }, ObjectShape::SQUARE, Vec2{ 5, 100 }));
	objects[2]->GetCollider().surfaceFriction = 0.1f;
	objects[2]->SetInverseMass(0.f);
	objects[2]->GetRigidBody().SetIsStatic(true);

	objects.push_back(new Actor(Vec2{ -50.f, 50.f }, ObjectShape::SQUARE, Vec2{ 5, 100 }));
	objects[3]->GetCollider().surfaceFriction = 0.1f;
	objects[3]->SetInverseMass(0.f);
	objects[3]->GetRigidBody().SetIsStatic(true);

	for (float i = 1.f; i < 26; ++i) {
		Actor* temp = new Actor(Vec2{ 1.f, i * 2 }, ObjectShape::POLYGON, Vec2{ 4, 2 });
		temp->GetCollider().surfaceFriction = 0.2f;

		objects.push_back(temp);
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
		//lines->DrawLineWithArrow(thisInfo.colliderA->position, thisInfo.colliderA->position + thisInfo.collisionNormal * 1.0, Colour::RED, 0.3f);
		//lines->DrawLineWithArrow(thisInfo.colliderB->position, thisInfo.colliderB->position - thisInfo.collisionNormal * 1.0, Colour::RED, 0.3f);
		thisInfo.Resolve();
	}

	for (int i = 0; i < objects.size(); ++i) {
		objects[i]->Draw(lines);
	}
	
	for (int i = 0; i < objects.size(); ++i) {
		objects[i]->Update(delta, cursorPos);
	}


	//Draw line and set magnitude for launch
	if (rightMouseDown && player->GetWorldPosition().y > -10.0f) {
		float DistanceToCur = (player->GetWorldPosition() - cursorPos).GetMagnitude();

		launchMagnitude = DistanceToCur >= player->GetRigidBody().GetTerminalVelocity() ? player->GetRigidBody().GetTerminalVelocity() : (player->GetWorldPosition() - cursorPos).GetMagnitude();
		lines->DrawLineWithArrow(player->GetWorldPosition(), player->GetWorldPosition() + (Vec2(player->GetWorldPosition() - cursorPos).Normalise()) * launchMagnitude * 2, Colour::RED);
	}
	else if(rightMouseDown && player->GetWorldPosition().y <= -10.0f) {
		float DistanceToCur = (player->GetWorldPosition() - cursorPos).GetMagnitude();

		launchMagnitude = DistanceToCur >= player->GetRigidBody().GetTerminalVelocity() ? player->GetRigidBody().GetTerminalVelocity() : (player->GetWorldPosition() - cursorPos).GetMagnitude();
		lines->DrawLineWithArrow(Vec2{ player->GetWorldPosition().x ,-10.0f }, Vec2{ player->GetWorldPosition().x, -10.0f } + (Vec2(player->GetWorldPosition() - cursorPos).Normalise()) * launchMagnitude * 2 , Colour::RED);
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
	player->SetPosition(cursorPos);
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