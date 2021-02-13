#pragma once

#include "Vec4.h"

class Mat4
{
public:
	Mat4() = default;
	Mat4(const Mat4& other);
	Mat4(const float matrix[4][4]);
	Mat4(Vec4 firstCol, Vec4 secondCol, Vec4 thirdCol, Vec4 forthCol);
	~Mat4() = default;

	const static Mat4 m_identity;

	static Mat4 CreateTransformMatrix(const Vec3& rotation,
		const Vec3& position,
		const Vec3& scale);

	static Mat4 CreateProjectionMatrix(const float near, const float far, const float aspectRatio);

	static Mat4 CreateCameraMatrix(const Vec3& rotation, const Vec3& position);

	Mat4 operator*(Mat4& other);
	Vec4 operator*(Vec4& other);
	Vec3 operator*(Vec3& other);
	float* operator[](int index);
	const float* operator[](int index) const;

	static Mat4 CreateTranslationMatrix(const Vec3& translation);
	static Mat4 CreateScaleMatrix(const Vec3& scale);
	static Mat4 CreateXRotationMatrix(float angle);
	static Mat4 CreateYRotationMatrix(float angle);
	static Mat4 CreateZRotationMatrix(float angle);	

	float GetFov();
	void SetFov(const float fov);
	void UpFov(const float delta);
	void DownFov(const float delta);

private:
	float m_grid[4][4] = { {0.0f, 0.0f, 0.0f, 0.0f},
							{0.0f, 0.0f, 0.0f, 0.0f},
							{0.0f, 0.0f, 0.0f, 0.0f},
							{0.0f, 0.0f, 0.0f, 0.0f} };

	static float FOV;
};