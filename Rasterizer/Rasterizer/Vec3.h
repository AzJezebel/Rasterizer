#pragma once

class Vec3
{
public:

	Vec3() = default;
	Vec3(const float newx, const float newy, const float newz);

	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	float GetMagnitude() const;
	void Normalize();

	Vec3 operator*(const float s) const;
	Vec3 operator-(const Vec3& v)const;
	Vec3 operator+(const Vec3& v) const;
	float& operator[](const unsigned int index);
	const float& operator[](const unsigned int index) const;
	const bool operator==(const Vec3& v) const;
	Vec3& operator=(const Vec3& v);

	void Lerp(Vec3& v0, float t);
};

Vec3 Cross(const Vec3& v0, const Vec3& v1);
float Dot(const Vec3& vec1, const Vec3& vec2);

Vec3 Normalize(const Vec3& vec);
float Q_rsqrt(float number);
