#include "pch.h"
#include "Entity.h"

const Mesh* Entity::GetMesh() const
{
	return mesh;
}

Mat4 Entity::GetTransfo() const
{
	return transformation;
}

Vec3 Entity::GetTranslation() const
{
	Vec3 translation;

	translation.x = transformation[0][0];
	translation.y = transformation[0][1];
	translation.z = transformation[0][2];

	return translation;
}

Vec3 Entity::GetRotation() const
{
	Vec3 rotation;

	rotation.x = transformation[1][0];
	rotation.y = transformation[1][1];
	rotation.z = transformation[1][2];

	return rotation;
}

Vec3 Entity::GetScale() const
{
	Vec3 scale;

	scale.x = transformation[2][0];
	scale.y = transformation[2][1];
	scale.z = transformation[2][2];

	return scale;
}

/*
Transfo
|T|T|T|1|
|R|R|R|1|
|S|S|S|1|
|1|1|1|1|
*/

//Store in the first row the translation 3Dvector in a 4x4 matrix
void Entity::SetTranslation(Vec3 comp)
{
	transformation[0][0] = comp.x;
	transformation[0][1] = comp.y;
	transformation[0][2] = comp.z;
}

//Store in the second row the rotation 3Dvector in a 4x4 matrix
void Entity::SetRotation(Vec3 comp)
{
	transformation[1][0] = comp.x;
	transformation[1][1] = comp.y;
	transformation[1][2] = comp.z;
}

//Store in the third row the scaling 3Dvector in a 4x4 matrix
void Entity::SetScale(Vec3 comp)
{
	transformation[2][0] = comp.x;
	transformation[2][1] = comp.y;
	transformation[2][2] = comp.z;
}

void Entity::SetColor(const Color& color)
{
	mesh->SetColor(color);
}

float Entity::GetAlpha() const
{
	return alpha;
}

const void Entity::SetAlpha(const float newAlpha)
{
	alpha = newAlpha;
}