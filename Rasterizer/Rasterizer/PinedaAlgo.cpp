#include "pch.h"

#include "Rasterizer.h"
#include <algorithm>

// Pass pre-projected points (not in world space but screen space)
void Rasterizer::DrawTriNew(const Vertex& v0, const Vertex& v1, const Vertex& v2, Texture* pTarget) const
{
	Point2d p0(v0);
	Point2d p1(v1);
	Point2d p2(v2);

	// Check if interpolation is needed
	bool interpolateZ = (v0.position.z == v1.position.z && v1.position.z == v2.position.z);
	bool interpolateColor = (v0.color == v1.color && v1.color == v2.color);

	// Triangle bounding box (to avoid rechecking the entire screen)
	int minX = std::min(std::min(p0.x, p1.x), p2.x);
	int minY = std::min(std::min(p0.y, p1.y), p2.y);
	int maxX = std::max(std::max(p0.x, p1.x), p2.x);
	int maxY = std::max(std::max(p0.y, p1.y), p2.y);

	// Don't check pixels outside of screen
	minX = std::max(minX, 0);
	minY = std::max(minY, 0);
	maxX = std::min(maxX, (int) pTarget->GetSize().x - 1);
	maxY = std::min(maxY, (int) pTarget->GetSize().y - 1);

	// Pixel currently being worked
	Point2d p(0, 0);

	// Check what the top vertex is
	Point2d start = (p0.y >= p1.y ? (p0.y >= p2.y ? p0 : p2) : (p1.y >= p2.y ? p1 : p2));

	for (p = start; start.y >= minY; start.y--)
	{
		// Check middle point
		float denom = (float) (p1.y - p2.y) * (p0.x - p2.x) + (p2.x - p1.x) * (p0.y - p2.y);

		float w0 = ((p1.y - p2.y) * (p.x - p2.x) + (p2.x - p1.x) * (p.y - p2.y)) / denom;
		float w1 = ((p2.y - p0.y) * (p.x - p2.x) + (p0.x - p2.x) * (p.y - p2.y)) / denom;
		float w2 = 1.f - w0 - w1;

		if (w0 >= 0 && w1 >= 0 && w2 >= 0)
		{
			float z;
			if (!interpolateZ)
				z = GetZValue(w0, w1, w2, v0.position.z, v1.position.z, v2.position.z);
			else
				z = v0.position.z;

			int offset = ((int) width) * p.y + p.x;

			if (z > zbuffer[offset])
			{
				zbuffer[offset] = z;

				Color c;

				if (!interpolateColor)
					c = GetInterpolationColor(w0, w1, w2, v0.color, v1.color, v2.color);
				else
					c = v0.color;

				pTarget->SetPixelColor(p.x, p.y, c);
			}
		}

		// Check right of middle point
		for (p = start, p.x++; p.x <= maxX; p.x++)
		{
			float denom = (float) (p1.y - p2.y) * (p0.x - p2.x) + (p2.x - p1.x) * (p0.y - p2.y);

			float w0 = ((p1.y - p2.y) * (p.x - p2.x) + (p2.x - p1.x) * (p.y - p2.y)) / denom;
			float w1 = ((p2.y - p0.y) * (p.x - p2.x) + (p0.x - p2.x) * (p.y - p2.y)) / denom;
			float w2 = 1.f - w0 - w1;

			if (w0 >= 0 && w1 >= 0 && w2 >= 0)
			{
				float z;
				if (!interpolateZ)
					z = GetZValue(w0, w1, w2, v0.position.z, v1.position.z, v2.position.z);
				else
					z = v0.position.z;

				int offset = ((int) width) * p.y + p.x;

				if (z > zbuffer[offset])
				{
					zbuffer[offset] = z;

					Color c;

					if (!interpolateColor)
						c = GetInterpolationColor(w0, w1, w2, v0.color, v1.color, v2.color);
					else
						c = v0.color;

					pTarget->SetPixelColor(p.x, p.y, c);
				}
			}
		}

		// Check left of middle point
		for (p = start; p.x >= minX; p.x--)
		{
			int pp2x = p.x - p2.x;
			int pp2y = p.y - p2.y;
			int p1p2y = p1.y - p2.y;
			int p0p2x = p0.x - p2.x;

			float denom = (float) p1p2y * p0p2x + (p2.x - p1.x) * (p0.y - p2.y);

			float w0 = (p1p2y * pp2x + (p2.x - p1.x) * pp2y) / denom;
			float w1 = ((p2.y - p0.y) * pp2x + p0p2x * pp2y) / denom;
			float w2 = 1.f - w0 - w1;

			if (w0 >= 0 && w1 >= 0 && w2 >= 0)
			{
				float z;
				if (!interpolateZ)
					z = GetZValue(w0, w1, w2, v0.position.z, v1.position.z, v2.position.z);
				else
					z = v0.position.z;

				int offset = ((int) width) * p.y + p.x;

				if (z > zbuffer[offset])
				{
					zbuffer[offset] = z;

					Color c;

					if (!interpolateColor)
						c = GetInterpolationColor(w0, w1, w2, v0.color, v1.color, v2.color);
					else
						c = v0.color;

					pTarget->SetPixelColor(p.x, p.y, c);
				}
			}
		}
	}
}