#include "pch.h"
#include "Camera.h"
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

void Camera::SetPosition(const Vec3& newPos)
{
	position.x = newPos.x;
	position.y = newPos.y;
	position.z = newPos.z;
}

void Camera::SetRotation(const Vec3& newRot)
{
	rotation.x = newRot.x;
	rotation.y = newRot.y;
	rotation.z = newRot.z;
}

Vec3 Camera::GetPosition()
{
	return position;
}

Vec3 Camera::GetRotation()
{
	return rotation;
}

void Camera::RightRotation()
{
	rotation.y -= 1;
}

void Camera::LeftRotation()
{
	rotation.y += 1;
}

void Camera::ForwardMove()
{
	position.x -= (cosf((rotation.y * (float) M_PI / 180.f) - (float) M_PI_2)) / 10.f;
	position.z += (sinf((rotation.y * (float) M_PI / 180.f) - (float) M_PI_2)) / 10.f;
}

void Camera::BackwardMove()
{
	position.x -= (cosf((rotation.y * (float) M_PI / 180.f) + (float) M_PI_2)) / 10.f;
	position.z += (sinf((rotation.y * (float) M_PI / 180.f) + (float) M_PI_2)) / 10.f;
}

void Camera::LeftMove()
{
	position.x += (cosf((rotation.y * (float) M_PI / 180.f) + (float) M_PI)) / 10.f;
	position.z -= (sinf((rotation.y * (float) M_PI / 180.f) + (float) M_PI)) / 10.f;
}

void Camera::RightMove()
{
	position.x += (cosf(rotation.y * (float) M_PI / 180.f)) / 10.f;
	position.z -= (sinf(rotation.y * (float) M_PI / 180.f)) / 10.f;
}