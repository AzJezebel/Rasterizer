#pragma once

#include "Vertex.h"
#include "Texture.h"

#include <vector>
#include <map>

class Mesh
{
public:
	static Mesh* CreateCube();
	static Mesh* CreateSphere(int latitudeCount, int longitudeCount);
	static Mesh* CreateTriangle();

	void AddVertex(const Vertex& v);
	void AddVertexDuplicate(const Vertex& v);

	void AddTriStripVertex(const Vertex& v);

	const int NumberOfTriangles() const;
	const std::array<Vertex, 3> GetTriangle(const size_t index) const;

	std::vector<int> GetIndices() const;

	void SetColor(const Color& color);
	std::map<std::string, Texture*> pTextures;

private:
	std::vector<Vertex> vertices;
	std::vector<int> indices;

	bool ccw = false;
};
