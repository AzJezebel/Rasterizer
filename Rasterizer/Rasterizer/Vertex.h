#pragma once

#include "Vec3.h"
#include "Color.h"
#include "Material.h"

class Vertex
{
public:

	Vertex()
	{
		position.x = 0.f;
		position.y = 0.f;
		position.z = 0.f;
	}

	Vertex(const float x, const float y, const float z, Color newColor = Color::ms_WHITE, Material newMat = Material::ms_DEFAULT) :
		color(newColor), mat(newMat)
	{
		position.x = x;
		position.y = y;
		position.z = z;
	}

	Vertex(const float x, const float y, const float z, const float u, const float v, Color newColor = Color::ms_WHITE, Material newMat = Material::ms_DEFAULT) :
		color(newColor), mat(newMat)
	{
		position.x = x;
		position.y = y;
		position.z = z;
		uPos = u;
		vPos = v;
	}

	Vertex(Vec3 pos, Color newColor = Color::ms_WHITE, Material newMat = Material::ms_DEFAULT) :
		color(newColor), mat(newMat)
	{
		position.x = pos.x;
		position.y = pos.y;
		position.z = pos.z;
	}

	Vertex(Vec3 pos, float u, float v, Color newColor = Color::ms_WHITE, Material newMat = Material::ms_DEFAULT) :
		color(newColor), mat(newMat)
	{
		position.x = pos.x;
		position.y = pos.y;
		position.z = pos.z;
		uPos = u;
		vPos = v;
	}

	Vertex(const float x, const float y, const float z, const float nx, const float ny, const float nz, Color newColor = Color::ms_WHITE, Material newMat = Material::ms_DEFAULT) :
		color(newColor), mat(newMat)
	{
		position.x = x;
		position.y = y;
		position.z = z;
		normal.x = nx;
		normal.y = ny;
		normal.z = nz;
	}

	Vertex(const float x, const float y, const float z, const float nx, const float ny, const float nz, const float u, const float v, Color newColor = Color::ms_WHITE, Material newMat = Material::ms_DEFAULT) :
		color(newColor), mat(newMat)
	{
		position.x = x;
		position.y = y;
		position.z = z;
		normal.x = nx;
		normal.y = ny;
		normal.z = nz;
		uPos = u;
		vPos = v;
	}

	Vertex(Vec3 pos, Vec3 norm, Color newColor = Color::ms_WHITE, Material newMat = Material::ms_DEFAULT) :
		position(pos), normal(norm), color(newColor)
	{
	}

	Vertex(Vec3 pos, Vec3 norm, const float u, const float v, Color newColor = Color::ms_WHITE, Material newMat = Material::ms_DEFAULT) :
		position(pos), normal(norm), color(newColor), mat(newMat)
	{
		uPos = u;
		vPos = v;
	}

	const bool operator==(const Vertex& v) const;

	Vec3 position;
	Color color = Color::ms_WHITE;
	Vec3 normal;
	float uPos = 0;
	float vPos = 0;
	Material mat = Material::ms_DEFAULT;
};
