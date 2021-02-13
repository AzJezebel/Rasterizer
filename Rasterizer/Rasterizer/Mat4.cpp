#include "pch.h"
#include "Mat4.h"

#define _USE_MATH_DEFINES
#include <math.h>

float Mat4::FOV = 60.0f;

const Mat4 Mat4::m_identity = { {1.0f, 0.0f, 0.0f, 0.0f},
								{0.0f, 1.0f, 0.0f, 0.0f},
								{0.0f, 0.0f, 1.0f, 0.0f},
								{0.0f, 0.0f, 0.0f, 1.0f} };

Mat4::Mat4(const Mat4& other)
{
	int i, j;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			m_grid[i][j] = other.m_grid[i][j];
		}
	}
}

Mat4::Mat4(const float matrix[4][4])
{
	int i, j;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			m_grid[i][j] = matrix[i][j];
		}
	}
}

Mat4::Mat4(Vec4 firstCol, Vec4 secondCol, Vec4 thirdCol, Vec4 forthCol)
{
	int i;

	for (i = 0; i < 4; i++)
	{
		m_grid[i][0] = firstCol[i];
		m_grid[i][1] = secondCol[i];
		m_grid[i][2] = thirdCol[i];
		m_grid[i][3] = forthCol[i];
	}
}

//Translation Matrix created with a translation 3Dvector
Mat4 Mat4::CreateTranslationMatrix(const Vec3& translation)
{
	Mat4 TranslationMat = m_identity;

	TranslationMat.m_grid[0][3] += translation.x;
	TranslationMat.m_grid[1][3] += translation.y;
	TranslationMat.m_grid[2][3] += translation.z;

	return TranslationMat;
}

//Scale Matrix created with a scaling 3Dvector
Mat4 Mat4::CreateScaleMatrix(const Vec3& scale)
{
	Mat4 ScaleMat = m_identity;

	ScaleMat.m_grid[0][0] *= scale.x;
	ScaleMat.m_grid[1][1] *= scale.y;
	ScaleMat.m_grid[2][2] *= scale.z;

	return ScaleMat;
}

//Rotation Matrix over xAxis created with an angle in degrees
Mat4 Mat4::CreateXRotationMatrix(float angle)
{
	float theta = angle * (float) M_PI / 180.0f;

	float c = cosf(theta);
	float s = sinf(theta);

	Vec4 Col1(Vec3(1.0f, 0.0f, 0.0f), 0.0f);
	Vec4 Col2(Vec3(0.0f, c, s), 0.0f);
	Vec4 Col3(Vec3(0.0f, -s, c), 0.0f);
	Vec4 Col4(Vec3(0.0f, 0.0f, 0.0f), 1.0f);

	Mat4 newMat(Col1, Col2, Col3, Col4);

	return newMat;
}

//Rotation Matrix over yAxis created with an angle in degrees
Mat4 Mat4::CreateYRotationMatrix(float angle)
{
	float theta = angle * (float) M_PI / 180.0f;

	float c = cosf(theta);
	float s = sinf(theta);

	Vec4 Col1(Vec3(c, 0.0f, -s), 0.0f);
	Vec4 Col2(Vec3(0.0f, 1.0f, 0.0f), 0.0f);
	Vec4 Col3(Vec3(s, 0.0f, c), 0.0f);
	Vec4 Col4(Vec3(0.0f, 0.0f, 0.0f), 1.0f);

	Mat4 newMat(Col1, Col2, Col3, Col4);

	return newMat;
}

//Rotation Matrix over zAxis created with an angle in degrees
Mat4 Mat4::CreateZRotationMatrix(float angle)
{
	float theta = angle * (float) M_PI / 180.0f;

	float c = cosf(theta);
	float s = sinf(theta);

	Vec4 Col1(Vec3(c, s, 0.0f), 0.0f);
	Vec4 Col2(Vec3(-s, c, 0.0f), 0.0f);
	Vec4 Col3(Vec3(0.0f, 0.0f, 1.0f), 0.0f);
	Vec4 Col4(Vec3(0.0f, 0.0f, 0.0f), 1.0f);

	Mat4 newMat(Col1, Col2, Col3, Col4);

	return newMat;
}

//Transformation Matrix created with translation, rotation and scaling 3Dvectors
Mat4 Mat4::CreateTransformMatrix(const Vec3& position, const Vec3& rotation, const Vec3& scale)
{
	Mat4 TransformMat = m_identity;

	Mat4 matRotX = CreateXRotationMatrix(rotation.x);
	Mat4 matRotY = CreateYRotationMatrix(rotation.y);
	Mat4 matRotZ = CreateZRotationMatrix(rotation.z);
	Mat4 matTrans = CreateTranslationMatrix(position);
	Mat4 matScale = CreateScaleMatrix(scale);

	TransformMat = matTrans * matRotY * matRotX * matRotZ * matScale;

	return TransformMat;
}

//Camera Matrix created as the inverse of the Transformation matrix
Mat4 Mat4::CreateCameraMatrix(const Vec3& position, const Vec3& rotation)
{
	Mat4 CameraMat = m_identity;
	Vec3 newPos = { -(position.x), -(position.y), -(position.z) };

	Mat4 matRotX = CreateXRotationMatrix(-(rotation.x));
	Mat4 matRotY = CreateYRotationMatrix(-(rotation.y));
	Mat4 matRotZ = CreateZRotationMatrix(-(rotation.z));
	Mat4 matTrans = CreateTranslationMatrix(newPos);

	CameraMat = matRotZ * matRotX * matRotY * matTrans;

	return CameraMat;
}

//Projection Matrix created with the near and far composants
Mat4 Mat4::CreateProjectionMatrix(const float near, const float far, const float aspectRatio)
{
	Mat4 ProjMat = m_identity;

	if (FOV >= 180)
	{
		FOV = 180;
	}
	else if (FOV <= 0)
	{
		FOV = 0;
	}

	float FOVY = (FOV * M_PI) / 180.f;
	//Care to import aspect ratio
	//float aspectRatio = 1000 / 1000;
	//float aspectRatio = 1024 / 768;

	float top = tanf(FOVY * 0.5f) * near;
	float bot = -top;
	float right = top * aspectRatio;
	float left = -right;

	ProjMat.m_grid[0][0] = (2 * near) / (right - left);
	ProjMat.m_grid[1][0] = 0;
	ProjMat.m_grid[2][0] = 0;
	ProjMat.m_grid[3][0] = 0;

	ProjMat.m_grid[0][1] = 0;
	ProjMat.m_grid[1][1] = (2 * near) / (top - bot);
	ProjMat.m_grid[2][1] = 0;
	ProjMat.m_grid[3][1] = 0;

	ProjMat.m_grid[0][2] = (right + left) / (right - left);
	ProjMat.m_grid[1][2] = (top + bot) / (top - bot);
	ProjMat.m_grid[2][2] = -(far + near) / (far - near);
	ProjMat.m_grid[3][2] = -1;

	ProjMat.m_grid[0][3] = 0;
	ProjMat.m_grid[1][3] = 0;
	ProjMat.m_grid[2][3] = (-2 * far * near) / (far - near);
	ProjMat.m_grid[3][3] = 0;

	return ProjMat;
}

Mat4 Mat4::operator*(Mat4& other)
{
	Mat4 newMat;

	int i, j, k;
	float sum;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			sum = 0;

			for (k = 0; k < 4; k++)
			{
				sum += this->m_grid[i][k] * other.m_grid[k][j];
			}

			newMat.m_grid[i][j] = sum;
		}
	}

	return newMat;
}

Vec4 Mat4::operator*(Vec4& other)
{
	Vec4 newVec;

	int i, j;
	float sum;

	for (i = 0; i < 4; i++)
	{
		sum = 0;

		for (j = 0; j < 4; j++)
		{
			sum += this->m_grid[i][j] * other[j];
		}

		newVec[i] = sum;
	}

	return newVec;
}

Vec3 Mat4::operator*(Vec3& other)
{
	Vec4 other4;
	other4.vec3 = other;
	Vec4 newVec;

	int row, col;
	float sum;

	for (row = 0; row < 4; row++)
	{
		sum = 0;

		for (col = 0; col < 4; col++)
		{
			sum += this->m_grid[row][col] * other4[col];
		}

		newVec[row] = sum;
	}

	newVec.Homogenize();

	return newVec.vec3;
}

float* Mat4::operator[](int index)
{
	return m_grid[index];
}

const float* Mat4::operator[](int index) const
{
	return m_grid[index];
}

float Mat4::GetFov()
{
	return FOV;
}

void Mat4::SetFov(const float fov)
{
	FOV = fov;
}

void Mat4::UpFov(const float delta)
{
	if (FOV == 180)
	{
		return;
	}

	FOV += delta;
}

void Mat4::DownFov(const float delta)
{
	if (FOV == 0)
	{
		return;
	}
	FOV -= delta;
}