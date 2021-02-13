#include "pch.h"

#include "Mesh.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cassert>
#include <array>

Mesh* Mesh::CreateCube()
{
	Mesh* mesh = new Mesh();

	const float SIZE = 0.5f;
	Texture* newtexture = new Texture("asset/kekw.png");
	//mesh->pTextures.insert(std::pair<std::string, Texture*>("kekw.png", newtexture));

	// Front face
	mesh->AddVertexDuplicate({ -SIZE, -SIZE, SIZE, 0, 0, 1, 0, 1});		// `[F]
	mesh->AddVertexDuplicate({ SIZE, SIZE, SIZE, 0, 0, 1, 1, 0 });		//  [F].
	mesh->AddVertexDuplicate({ SIZE, -SIZE, SIZE, 0, 0, 1 , 1, 1});		//  [F]`
	mesh->AddVertexDuplicate({ -SIZE, -SIZE, SIZE, 0, 0, 1, 0, 1});		// `[F]
	mesh->AddVertexDuplicate({ -SIZE, SIZE, SIZE, 0, 0, 1, 0, 0});		// .[F]
	mesh->AddVertexDuplicate({ SIZE, SIZE, SIZE, 0, 0, 1, 1, 0 });		//  [F].

	// Right face
	mesh->AddVertexDuplicate({ SIZE, -SIZE, SIZE, 1, 0, 0, 0, 1 });		//  [F]`
	mesh->AddVertexDuplicate({ SIZE, SIZE, -SIZE, 1, 0, 0, 1, 0 });		//  [B].
	mesh->AddVertexDuplicate({ SIZE, -SIZE, -SIZE, 1, 0, 0, 1, 1 });	//  [B]`
	mesh->AddVertexDuplicate({ SIZE, -SIZE, SIZE, 1, 0, 0, 0, 1 });		//  [F]`
	mesh->AddVertexDuplicate({ SIZE, SIZE, SIZE, 1, 0, 0, 0, 0 });		//  [F].
	mesh->AddVertexDuplicate({ SIZE, SIZE, -SIZE, 1, 0, 0, 1, 0 });		//  [B].

	// Back face
	mesh->AddVertexDuplicate({ SIZE, -SIZE, -SIZE, 0, 0, -1, 0, 1 });	//  [B]`
	mesh->AddVertexDuplicate({ -SIZE, SIZE, -SIZE, 0, 0, -1, 1, 0 });	// .[B]
	mesh->AddVertexDuplicate({ -SIZE, -SIZE, -SIZE, 0, 0, -1,  1, 1 });	// `[B]
	mesh->AddVertexDuplicate({ SIZE, -SIZE, -SIZE, 0, 0, -1, 0, 1 });	//  [B]`
	mesh->AddVertexDuplicate({ SIZE, SIZE, -SIZE, 0, 0, -1, 0, 0 });	//  [B].
	mesh->AddVertexDuplicate({ -SIZE, SIZE, -SIZE, 0, 0, -1, 1, 0 });	// .[B]

	// Left face
	mesh->AddVertexDuplicate({ -SIZE, -SIZE, -SIZE, -1, 0, 0, 0, 1 });	// `[B]
	mesh->AddVertexDuplicate({ -SIZE, SIZE, SIZE, -1, 0, 0, 1, 0 });	// .[F]
	mesh->AddVertexDuplicate({ -SIZE, -SIZE, SIZE, -1, 0, 0, 1, 1 });	// `[F]
	mesh->AddVertexDuplicate({ -SIZE, -SIZE, -SIZE, -1, 0, 0, 0, 1 });	// `[B]
	mesh->AddVertexDuplicate({ -SIZE, SIZE, -SIZE, -1, 0, 0, 0, 0 });	// .[B]
	mesh->AddVertexDuplicate({ -SIZE, SIZE, SIZE, -1, 0, 0, 1, 0 });	// .[F]

	// Bottom face
	mesh->AddVertexDuplicate({ -SIZE, SIZE, SIZE, 0, 1, 0, 0, 1 });		// .[F]
	mesh->AddVertexDuplicate({ SIZE, SIZE, -SIZE, 0, 1, 0, 1, 0 });		//  [B].
	mesh->AddVertexDuplicate({ SIZE, SIZE, SIZE, 0, 1, 0, 1, 1 });		//  [F].
	mesh->AddVertexDuplicate({ -SIZE, SIZE, SIZE, 0, 1, 0, 0, 1 });		// .[F]
	mesh->AddVertexDuplicate({ -SIZE, SIZE, -SIZE, 0, 1, 0, 0, 0 });	// .[B]
	mesh->AddVertexDuplicate({ SIZE, SIZE, -SIZE, 0, 1, 0, 1, 0 });		//  [B].

	// Top face
	mesh->AddVertexDuplicate({ -SIZE, -SIZE, -SIZE, 0, -1, 0, 0, 1 });	// `[B]
	mesh->AddVertexDuplicate({ SIZE, -SIZE, SIZE, 0, -1, 0, 1, 0 });	//  [F]`
	mesh->AddVertexDuplicate({ SIZE, -SIZE, -SIZE, 0, -1, 0, 1, 1 });	//  [B]`
	mesh->AddVertexDuplicate({ -SIZE, -SIZE, -SIZE, 0, -1, 0, 0, 1 });	// `[B]
	mesh->AddVertexDuplicate({ -SIZE, -SIZE, SIZE, 0, -1, 0, 0, 0 });	// `[F]
	mesh->AddVertexDuplicate({ SIZE, -SIZE, SIZE, 0, -1, 0, 1, 0 });	//  [F]`

	//for (auto& v : mesh->vertices)
	//{
	//	v.mat.textureMapIndex = "kekw.png";
	//}

	return mesh;
}

Mesh* Mesh::CreateSphere(int latitudeCount, int longitudeCount)
{
	Mesh* mesh = new Mesh();

	const float pi = (float) M_PI;
	float radius = 0.5f;
	Vec3 normal;

	for (float alpha = 0.f; alpha < pi; alpha += pi / latitudeCount)
	{
		for (float beta = 0.f; beta < 2.01 * pi; beta += pi / longitudeCount)
		{
			float x = radius * cosf(beta) * sinf(alpha);
			float y = radius * sinf(beta) * sinf(alpha);
			float z = radius * cosf(alpha);

			normal.x = x;
			normal.y = y;
			normal.z = z;
			normal.Normalize();

			mesh->AddTriStripVertex({ {x, y, z}, normal });

			x = radius * cosf(beta) * sinf(alpha + pi / longitudeCount);
			y = radius * sinf(beta) * sinf(alpha + pi / longitudeCount);
			z = radius * cosf(alpha + pi / longitudeCount);

			normal.x = x;
			normal.y = y;
			normal.z = z;
			normal.Normalize();

			mesh->AddTriStripVertex({ { x, y, z }, normal });
		}
	}
	return mesh;
}

Mesh* Mesh::CreateTriangle()
{
	Mesh* mesh = new Mesh();

	const float SIZE = 0.1f;

	mesh->AddVertex({ -SIZE, -SIZE, 0, Color::ms_RED });
	mesh->AddVertex({ SIZE, -SIZE, 0, Color::ms_GREEN });
	mesh->AddVertex({ 0, SIZE, 0, Color::ms_BLUE });

	//mesh->AddVertex({ 0, -SIZE, -SIZE, Color::ms_RED });
	//mesh->AddVertex({ 0, -SIZE, SIZE, Color::ms_GREEN });
	//mesh->AddVertex({ 0, SIZE, 0, Color::ms_BLUE });

	return mesh;
}

// Use this if scanning for duplicates is necessary
void Mesh::AddVertex(const Vertex& v)
{
	for (int i = 0; i < vertices.size(); i++)
	{
		if (vertices[i] == v)
		{
			indices.push_back(i);
			return;
		}
	}

	vertices.push_back(v);
	indices.push_back((int) vertices.size() - 1);
}

// Use this if you want to have duplicate vertices
void Mesh::AddVertexDuplicate(const Vertex& v)
{
	vertices.push_back(v);
	indices.push_back((int) vertices.size() - 1);
}

void Mesh::AddTriStripVertex(const Vertex& v)
{
	if (indices.size() > 2)
	{
		if (ccw)
		{
			ccw = !ccw;
			indices.push_back(indices[indices.size() - 3]);
			indices.push_back(indices[indices.size() - 2]);
		}
		else
		{
			ccw = !ccw;
			indices.push_back(indices[indices.size() - 1]);
			indices.push_back(indices[indices.size() - 3]);
		}
	}

	AddVertex(v);
}

const int Mesh::NumberOfTriangles() const
{
	assert(indices.size() % 3 == 0);

	return (int) indices.size() / 3;
}

const std::array<Vertex, 3> Mesh::GetTriangle(const size_t index) const
{
	assert(indices.size() % 3 == 0);

	return { vertices[indices[index * ((size_t) 3)]], vertices[indices[(index * (size_t) 3) + 1]], vertices[indices[(index * (size_t) 3) + 2]] };
}

std::vector<int> Mesh::GetIndices() const
{
	return indices;
}

void Mesh::SetColor(const Color& color)
{
	for (auto& v : vertices)
	{
		v.color = color;
	}
}