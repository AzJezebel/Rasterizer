#include "pch.h"
#include "Light.h"

Light::Light(Vec3 pos, Vec3 ambient, Vec3 diffuse, Vec3 specular) :
	position(pos), ambientComponent(ambient), diffuseComponent(diffuse), specularComponent(specular)
{
}

Light::Light(Vec3 pos, float ambient, float diffuse, float specular, Color col) :
	position(pos)
{
	ambientComponent = { col.red * ambient, col.green * ambient, col.blue * ambient };
	diffuseComponent = { col.red * diffuse, col.green * diffuse, col.blue * diffuse };
	specularComponent = { col.red * specular, col.green * specular, col.blue * specular };
}