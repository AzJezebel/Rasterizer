#include "pch.h"
#include "Vec4.h"
#include <cassert>

Vec4::Vec4(float x, float y, float z, float neww)
{
	vec3.x = x;
	vec3.y = y;
	vec3.z = z;
	w = neww;
}

Vec4::Vec4(const Vec3& v, float neww)
{
	vec3 = v;
	w = neww;
}

// Divides all components by w, turning the homogeneous vector into a cartesian vector.
void Vec4::Homogenize()
{
	if (w > 0)
	{
		vec3.x /= w;
		vec3.y /= w;
		vec3.z /= w;
	}
}

// Returns the magnitude (length) of the vector.
float Vec4::GetMagnitude() const
{
	return vec3.GetMagnitude();
}

// Divides all components of the vector by the magnitude, turning it into a unit vector.
void Vec4::Normalize()
{
	vec3.Normalize();
}

Vec4 Vec4::operator+(Vec4& v)
{
	return Vec4(vec3 + v.vec3);
}

Vec4 Vec4::operator*(float x)
{
	return Vec4(vec3 * x);
}

float& Vec4::operator[](const unsigned int index)
{
	assert(index < 4 || index >= 0);

	if (index == 3)
	{
		return w;
	}
	return vec3[index];
}

const bool Vec4::operator==(const Vec4& v) const
{
	return (vec3 == v.vec3 && w == v.w);
}