#pragma once
#include "Vec3.h"
#include <vector>
#include "Color.h"

class Light
{
public:
	Light() = default;
	Light(Vec3 pos, Vec3 ambient, Vec3 diffuse, Vec3 specular);
	Light(Vec3 pos, float ambient, float diffuse, float specular, Color col = Color::ms_WHITE);
	~Light() = default;

	Vec3 position;
	Vec3 ambientComponent;
	Vec3 diffuseComponent;
	Vec3 specularComponent;

	void ToggleOn()
	{
		onoff = !onoff;
	}

	bool onoff = true;
};