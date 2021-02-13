#include "pch.h"
#include "Vec3.h"

#include <cassert>

Vec3::Vec3(const float newx, const float newy, const float newz)
{
	x = newx;
	y = newy;
	z = newz;
}

// Quake fast inverse square root
float Q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long*) &y;                       // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);               // what the fuck?
	y = *(float*) &i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

// Returns the magnitude (length) of the vector.
float Vec3::GetMagnitude() const
{
	float a = (x * x) + (y * y) + (z * z);
	return a * Q_rsqrt(a);
}

// Divides all components of the vector by the magnitude, turning it into a unit vector.
void Vec3::Normalize()
{
	float mag = GetMagnitude();

	if (mag == 0.f)
		return;

	x /= mag;
	y /= mag;
	z /= mag;
}

float Dot(const Vec3& vec1, const Vec3& vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

Vec3 Normalize(const Vec3& vec)
{
	float mag = vec.GetMagnitude();

	if (mag == 0)
		return vec;

	return { vec.x / mag, vec.y / mag, vec.z / mag };
}

Vec3 Vec3::operator-(const Vec3& v) const
{
	return Vec3(x - v.x, y - v.y, z - v.z);
}

Vec3 Vec3::operator+(const Vec3& v) const
{
	return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3 Vec3::operator*(const float s) const
{
	return Vec3(x * s, y * s, z * s);
}

float& Vec3::operator[](const unsigned int index)
{
	assert(index < 3 || index >= 0);

	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	default:
		assert(false);
		return x;
	}
}

const float& Vec3::operator[](const unsigned int index) const
{
	assert(index < 3 || index >= 0);

	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	default:
		assert(false);
		return x;
	}
}

const bool Vec3::operator==(const Vec3& v) const
{
	return (x == v.x && y == v.y && z == v.z);
}

Vec3& Vec3::operator=(const Vec3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;

	return *this;
}

Vec3 Cross(const Vec3& v0, const Vec3& v1)
{
	return { (v0.y * v1.z) - (v0.z * v1.y), -((v0.x * v1.z) - (v0.z * v1.x)), (v0.x * v1.y) - (v0.y * v1.x) };
}

void Vec3::Lerp(Vec3& v0, float t)
{
	x = (1 - t) * x + t * v0.x;
	y = (1 - t) * y + t * v0.y;
	z = (1 - t) * z + t * v0.z;
}
