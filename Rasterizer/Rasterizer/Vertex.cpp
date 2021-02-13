#include "pch.h"
#include "Vertex.h"

const bool Vertex::operator==(const Vertex& v) const
{
	return (position == v.position) && (color == v.color) && (normal == v.normal);
}