#include "pch.h"
#include "Texture.h"
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const unsigned int w, const unsigned int h)
{
	pixels = new Color[w * h];
	width = w;
	height = h;

	ClearTexture(Color::ms_BLACK);
}

//Call the BMP loader based on the path given
Texture::Texture(const char* pathToFile)
{
	LoadBMP(pathToFile);
}

Texture::~Texture()
{
	delete[] pixels;
}

//Place the wanted color on a designated pixel
void Texture::SetPixelColor(unsigned int x, unsigned int y, const Color& c)
{
	size_t index = width * y + x;

	Color backUpColor = pixels[index];

	pixels[index] = c + backUpColor * (1.0f - c.alpha);
}

//Clear the current texture on pixels
void Texture::ClearTexture(const Color& clearColor)
{
	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			pixels[width * j + i] = clearColor;
		}
	}
}

//Used in case of a resize
void Texture::ResizeTexture(int w, int h)
{
	width = w;
	height = h;

	delete[] pixels;

	pixels = new Color[w * h];

	ClearTexture();
}

const Color* Texture::GetPixels() const
{
	return pixels;
}

const Point2d Texture::GetSize() const
{
	return Point2d((int) width, (int) height);
}

//Based on the path given, create pixel of color corresponding to the texture color at the same coordinate
void Texture::LoadBMP(const char* pathToFile)
{
	int picWidth, picHeight, comp, counter = 0;

	stbi_uc* imgData = stbi_load(pathToFile, &picWidth, &picHeight, &comp, 0);

	width = picWidth;
	height = picHeight;

	stbi_set_flip_vertically_on_load(true);

	pixels = new Color[width * height];

	if (comp == 4)
	{
		for (int i = 0; i < width * height * comp; i++)
		{
			if (counter == 0)
			{
				pixels[i / comp].red = imgData[i] / 255.f;
				counter++;
			}

			else if (counter == 1)
			{
				pixels[i / comp].green = imgData[i] / 255.f;
				counter++;
			}

			else if (counter == 2)
			{
				pixels[i / comp].blue = imgData[i] / 255.f;
				counter++;
			}

			else if (counter == 3)
			{
				pixels[i / comp].alpha = imgData[i] / 255.f;
				counter = 0;
			}
		}
	}

	if (comp == 3)
	{
		for (int i = 0; i < width * height * comp; i++)
		{
			if (counter == 0)
			{
				pixels[i / comp].red = imgData[i] / 255.f;
				counter++;
			}

			else if (counter == 1)
			{
				pixels[i / comp].green = imgData[i] / 255.f;
				counter++;
			}

			else if (counter == 2)
			{
				pixels[i / comp].blue = imgData[i] / 255.f;
				counter = 0;
			}
		}
	}

	stbi_image_free(imgData);
}