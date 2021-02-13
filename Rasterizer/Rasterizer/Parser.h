#pragma once

#include "Vec3.h"
#include "Mesh.h"
#include <vector>
#include <string>

struct face
{
	int vertex[3] = { 0, 0, 0 };
	int texture[3] = { 0, 0, 0 };
	int normal[3] = { 0, 0, 0 };

	std::string mtl;

	void CheckValues(int v, int n, int t);
	int MakePositive(int num, int size);
};

Mesh* LoadOBJ(const std::string& path);