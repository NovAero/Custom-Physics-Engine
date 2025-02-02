#pragma once
#include <vector>

class PhysicsObject;

class Scene {
public:

	Scene() = default;
	~Scene();

private:
	//Array of objects
	std::vector<PhysicsObject*> objectsInScene;

	void AddPhysicsObject(PhysicsObject* object);
};