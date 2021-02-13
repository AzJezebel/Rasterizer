#pragma once

#include "Vertex.h"

struct Point2d
{
	Point2d(int xn, int yn)
	{
		x = xn;
		y = yn;
	}

	Point2d(const Vertex& v)
	{
		x = (int) v.position.x;
		y = (int) v.position.y;
	}

	int x;
	int y;
};
