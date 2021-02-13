#pragma once

#include "Entity.h"

class Scene
{
public:
	~Scene()
	{
		entities.clear();
	}

	std::vector<Entity> entities;
	std::vector<Light> lights;
};
