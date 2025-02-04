#include "Scene.h"
#include "Actor.h"


Scene::~Scene()
{
	objectsInScene.clear();
}

void Scene::AddPhysicsObject(PhysicsObject* object)
{
	if (object != nullptr) {
		objectsInScene.push_back(object);
	}
}