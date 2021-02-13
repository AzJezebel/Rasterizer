#pragma once

#include "Mesh.h"
#include "Mat4.h"
#include "Light.h"

struct Entity
{
public:
	Entity()
	{
		transformation = Mat4({
			{0.f, 0.f, 1.f, 1.f},
			{0.f, 0.f, 1.f, 1.f},
			{0.f, 0.f, 1.f, 1.f},
			{1.f, 1.f, 1.f, 1.f}
			});
	}
	Entity(Mesh* m)
	{
		mesh = m;
		transformation = Mat4({
			{0.f, 0.f, 1.f, 1.f},
			{0.f, 0.f, 1.f, 1.f},
			{0.f, 0.f, 1.f, 1.f},
			{1.f, 1.f, 1.f, 1.f}
			});
		alpha = 1.0f;
	}

	Entity(Mesh* m, float newAlpha)
	{
		mesh = m;
		transformation = Mat4({
			{0.f, 0.f, 1.f, 1.f},
			{0.f, 0.f, 1.f, 1.f},
			{0.f, 0.f, 1.f, 1.f},
			{1.f, 1.f, 1.f, 1.f}
			});
		alpha = newAlpha;
	}

	~Entity() = default;

	const Mesh* GetMesh() const;
	Mat4 GetTransfo() const;

	Vec3 GetTranslation() const;
	Vec3 GetRotation() const;
	Vec3 GetScale() const;
	float GetAlpha() const;

	void SetTranslation(Vec3 comp);
	void SetRotation(Vec3 comp);
	void SetScale(Vec3 comp);
	void SetColor(const Color& color);
	const void SetAlpha(const float newAlpha);

private:
	Mesh* mesh = nullptr;
	Mat4 transformation;
	float alpha = 1.0f;
};
