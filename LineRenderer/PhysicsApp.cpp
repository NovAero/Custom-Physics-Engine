#include "PhysicsApp.h"
#include "LineRenderer.h"
#include "Key.h"
#include "RigidBody.h"
#include "Grid.h"

PhysicsApp::PhysicsApp()
{

}

PhysicsApp::~PhysicsApp()
{
	if (player != nullptr) {
		delete player;
	}
}

void PhysicsApp::Initialise()
{
	player = new RigidBody(Vec2{ 0,0.5f }, TRIANGLE, Colour::GREEN, 0.7f, 0.8f, Vec2{ 2.f,2.f });
}

void PhysicsApp::Update(float delta)
{
	
	player->Update(delta, cursorPos);

	if (rightMouseDown && player->GetCurrentPosition().y > -10.0f) {
		float DistanceToCur = (player->GetCurrentPosition() - cursorPos).GetMagnitude();

		launchMagnitude = DistanceToCur >= player->GetTerminalVelocity() ? player->GetTerminalVelocity() : (player->GetCurrentPosition() - cursorPos).GetMagnitude();
		lines->DrawLineWithArrow(player->GetCurrentPosition(), player->GetCurrentPosition() + (Vec2(player->GetCurrentPosition() - cursorPos).Normalise()) * launchMagnitude, Colour::RED);
	}
	else if(rightMouseDown && player->GetCurrentPosition().y <= -10.0f) {
		float DistanceToCur = (player->GetCurrentPosition() - cursorPos).GetMagnitude();

		launchMagnitude = DistanceToCur >= player->GetTerminalVelocity() ? player->GetTerminalVelocity() : (player->GetCurrentPosition() - cursorPos).GetMagnitude();
		
		lines->DrawLineWithArrow(Vec2{ player->GetCurrentPosition().x ,-10.0f }, Vec2{ player->GetCurrentPosition().x, -10.0f } + (Vec2(player->GetCurrentPosition() - cursorPos).Normalise()) * launchMagnitude, Colour::RED);
	}

	player->Draw(lines);
}

void PhysicsApp::OnLeftClick()
{
	player->SetPosition(cursorPos);
	player->SetVelocity(Vec2{ 0,0 });
	player->SetIsDirty(false);
}

void PhysicsApp::OnRightClick()
{
	rightMouseDown = true;
	
}

void PhysicsApp::OnRightRelease()
{
	Vec2 Direction = player->GetCurrentPosition() - cursorPos;
	Direction.Normalise();

	player->ApplyImpulse(Direction, launchMagnitude);

	rightMouseDown = false;

}

void PhysicsApp::OnKeyPress(Key key)
{

}