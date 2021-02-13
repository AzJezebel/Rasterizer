#include "pch.h"
#include "Color.h"

Color::Color(const Color& color)
{
	red = color.red;
	green = color.green;
	blue = color.blue;
	alpha = color.alpha;
}

Color::Color(float all, float a)
{
	red = all;
	green = all;
	blue = all;
	alpha = a;
}

Color::Color(float r, float g, float b, float a)
{
	red = r;
	green = g;
	blue = b;
	alpha = a;
}

const Color Color::ms_BLACK(0.f, 0.f, 0.f);
const Color Color::ms_WHITE(1.f, 1.f, 1.f);
const Color Color::ms_RED(1.f, 0.f, 0.f);
const Color Color::ms_GREEN(0.f, 1.f, 0.f);
const Color Color::ms_BLUE(0.f, 0.f, 1.f);

//Return an interpolated color based on the original vertices of the current triangle of the entity
const Color GetInterpolationColor(const float w0, const float w1, const float w2, const Color& v0, const Color& v1, const Color& v2, const float newAlpha)
{
	Color c;

	c.red = v0.red * w0 + v1.red * w1 + v2.red * w2;
	c.green = v0.green * w0 + v1.green * w1 + v2.green * w2;
	c.blue = v0.blue * w0 + v1.blue * w1 + v2.blue * w2;
	c.alpha *= newAlpha;

	return c;
}

//Return an interpolated color based on the original vertices of the current line of the triangle
const Color GetInterpolationColorLine(const float w0, const float w1, const Color& v0, const Color& v1)
{
	Color c;

	c.red = v0.red * w0 + v1.red * w1;
	c.green = v0.green * w0 + v1.green * w1;
	c.blue = v0.blue * w0 + v1.blue * w1;

	return c;
}



// Making my own clamp function because VS doesn't recongnize the one 
//implemented in the STL for C++17
float ClampColorChannel(const float val, const float min, const float max)
{
	return std::max(std::min(val, max), min);
}