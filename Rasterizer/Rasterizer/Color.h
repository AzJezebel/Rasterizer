#pragma once

#include <algorithm>
#include "Vec3.h"

float ClampColorChannel(const float val, const float min, const float max);

struct Color
{
	Color() = default;
	Color(const Color& color);
	Color(float all, float alpha = 1.f);
	Color(float red, float green, float blue, float alpha = 1.f);

	float red = 1.f;
	float green = 1.f;
	float blue = 1.f;
	float alpha = 1.f;

	static const Color ms_WHITE;
	static const Color ms_BLACK;
	static const Color ms_RED;
	static const Color ms_GREEN;
	static const Color ms_BLUE;

	Color operator*(const float val) const
	{
		return {
			ClampColorChannel(red * val, 0.f, 1.f),
			ClampColorChannel(green * val, 0.f, 1.f),
			ClampColorChannel(blue * val, 0.f, 1.f)
		};
	}

	Color operator*(const Color& c) const
	{
		return {
			(red * c.red),
			(green * c.green),
			(blue * c.blue)
		};
	}

	Color operator+(const Color& c) const
	{
		return {
			ClampColorChannel(red + c.red, 0.f, 1.f),
			ClampColorChannel(green + c.green,0.f, 1.f),
			ClampColorChannel(blue + c.blue, 0.f, 1.f),
			ClampColorChannel(alpha + c.alpha * (1.0f - alpha), 0.f, 1.f)
		};
	}

	bool operator==(const Color& c) const
	{
		return (red == c.red) && (green == c.green) && (blue == c.blue) && (alpha == c.alpha);
	}

	Color operator*(const Vec3& v) const
	{
		return {
			ClampColorChannel(red * v.x, 0.f, 1.f),
			ClampColorChannel(green * v.y, 0.f, 1.f),
			ClampColorChannel(blue * v.z, 0.f, 1.f)
		};
	}
};

const Color GetInterpolationColor(const float w0, const float w1, const float w2, const Color& v0, const Color& v1, const Color& v2, const float newAlpha);
const Color GetInterpolationColorLine(const float w0, const float w1, const Color& v0, const Color& v1);
