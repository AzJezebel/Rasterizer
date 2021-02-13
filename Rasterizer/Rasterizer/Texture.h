#pragma once

#include "Color.h"
#include <vector>
#include "Vertex.h"
#include "Point2d.h"

class Texture
{
public:
	Texture() = delete;
	Texture(unsigned int w, unsigned int h);
	Texture(const char* pathToFile);
	~Texture();

	void SetPixelColor(unsigned int x, unsigned int y, const Color& c);

	void ClearTexture(const Color& clearColor = Color::ms_BLACK);

	void ResizeTexture(int w, int h);

	const Color* GetPixels() const;

	const Point2d GetSize() const;
	
	void LoadBMP(const char* pathToFile);

private:
	size_t width;
	size_t height;
	Color* pixels;
};
