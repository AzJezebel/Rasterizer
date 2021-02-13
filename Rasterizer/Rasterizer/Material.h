#pragma once
#include "Vec3.h"
#include <string>

struct Material
{
	Material()
	{
		ambient = ms_DEFAULT.ambient;
		diffuse = ms_DEFAULT.diffuse;
		specular = ms_DEFAULT.specular;
		shininess = ms_DEFAULT.shininess;
	}

	Material(Vec3 a, Vec3 d, Vec3 s, float sh)
	{
		ambient = a;
		diffuse = d;
		specular = s;
		shininess = sh;
	}

	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
	float shininess;
	std::string textureMapIndex;

	static const Material ms_DEFAULT;
};