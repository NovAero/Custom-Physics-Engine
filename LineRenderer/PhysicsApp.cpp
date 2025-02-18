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
	player = new Actor(Vec2{ 100.f,4.f }, ObjectShape::POLYGON, Vec2{7,8});
	player->colour = Colour::GREEN;
	player->GetCollider().surfaceFriction = 0.5f;
	player->GetCollider().invMass = 5.f;

	objects.push_back(player);

	objects.push_back(new Actor(Vec2{ 0.f, -100.f }, ObjectShape::POLYGON, Vec2{ 7, 50 }));
	objects[1]->GetCollider().surfaceFriction = 0.7f;
	objects[1]->SetInverseMass(0.f);
	objects[1]->GetRigidBody().SetIsStatic(true);

	objects.push_back(new Actor(Vec2{ 20.f, -50.f }, ObjectShape::POLYGON, Vec2{ 7, 50 }));
	objects[2]->GetCollider().surfaceFriction = 0.7f;
	objects[2]->SetInverseMass(0.f);
	objects[2]->GetRigidBody().SetIsStatic(true);

	objects.push_back(new Actor(Vec2{ -20.f, -50.f }, ObjectShape::RECT, Vec2{ 6, 100}));
	objects[3]->GetCollider().surfaceFriction = 0.7f;
	objects[3]->SetInverseMass(0.f);
	objects[3]->GetRigidBody().SetIsStatic(true);

	for (float i = 1.f; i < 20; ++i) {
		Actor* temp = new Actor(Vec2{ 1.f, i * 2 }, ObjectShape::POLYGON, Vec2{ 6, 3 });
		temp->GetCollider().surfaceFriction = 0.1f;
		temp->GetRigidBody().SetElasticityPerc(0.7f);

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
		objects[i]->Update(delta, cursorPos);
	}

	for (int i = 0; i < objects.size(); ++i) {
		objects[i]->Draw(lines);
	}

	//Draw line and set magnitude for launch
	if (rightMouseDown) {
		float DistanceToCur = (player->GetWorldPosition() - cursorPos).GetMagnitude();

		launchMagnitude = DistanceToCur >= player->GetRigidBody().GetTerminalVelocity() ? player->GetRigidBody().GetTerminalVelocity() : (player->GetWorldPosition() - cursorPos).GetMagnitude();
		lines->DrawLineWithArrow(player->GetWorldPosition(), player->GetWorldPosition() + (Vec2(player->GetWorldPosition() - cursorPos).Normalise()) * launchMagnitude * 2, Colour::RED);
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
	
	player->SetPosition(player->GetWorldPosition() + Vec2{ Direction * 0.1f });

	player->GetRigidBody().SetVelocity(Direction * launchMagnitude);
	player->GetRigidBody().SetShouldApplyFriction(false);
	player->GetRigidBody().SetIsDirty(true);

	rightMouseDown = false;

}

void PhysicsApp::OnKeyPress(Key key)
{
	if (key == Key::E) {
		player->GetCollider().Rotate(45);
	}
	else if (key == Key::Q) {
		player->GetCollider().Rotate(-45);
	}
}