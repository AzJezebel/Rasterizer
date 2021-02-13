#pragma once
#include "Vec3.h"

class Vec4
{
public:
	Vec4() = default;
	Vec4(float x, float y, float z, float neww);
	Vec4(const Vec3& v, float w = 1.f);

	Vec3 vec3;
	float w = 1.f;

	void Homogenize();
	float GetMagnitude() const;
	void Normalize();

	Vec4 operator+(Vec4& v);
	Vec4 operator*(float x);
	float& operator[](const unsigned int index);
	const bool operator==(const Vec4& v) const;
};
