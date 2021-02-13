#pragma once

#include "Vec3.h"

class Camera
{
public:
	Camera() = default;
	~Camera() = default;

	void SetPosition(const Vec3& newPos);
	void SetRotation(const Vec3& newRot);

	void RightRotation();
	void LeftRotation();

	void ForwardMove();
	void BackwardMove();
	void LeftMove();
	void RightMove();

	Vec3 GetPosition();
	Vec3 GetRotation();

private:
	Vec3 position = {};
	Vec3 rotation = {};
};