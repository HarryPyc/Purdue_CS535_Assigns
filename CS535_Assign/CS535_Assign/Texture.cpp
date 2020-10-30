#include "Texture.h"
#include <omp.h>
#include <iostream>
#include <stb_image.h>
#include <stb_image_write.h>

Texture::Texture(std::string fname)
{
	unsigned char* cpixels;
	cpixels = stbi_load(fname.c_str(), &w, &h, &components, 4);
	if (cpixels == nullptr) {
		std::cout << "Failed Read image: " + fname << std::endl;
		return;
	}
	pixels = new unsigned int[w * h];
	for (int i = 0; i < w; i++) {
		for(int j = 0; j < h; j++){
			const int index = (h - 1 - j) * w + i;
			fvec4 color((float)cpixels[4 * index], (float)cpixels[4 * index + 1], (float)cpixels[4 * index + 2], (float)cpixels[4 * index + 3]);
			color = color / 255.f;
			pixels[j * w + i] = convVec4ToRGBA8(color);
		}
	}
	delete[] cpixels;
}

Texture::Texture(int width, int height, int Components) : w(width), h(height), components(Components)
{
	pixels = new unsigned int[w*h];
}

Texture::Texture()
{
	pixels = nullptr;
}

void Texture::Clear(fvec4 color)
{
	const uint c = convVec4ToRGBA8(color);
#ifdef MULTI_PROCESS
#pragma omp parallel for
#endif 
	for (int i = 0; i < w * h; i++)
		pixels[i] = c;
}

void Texture::SetPixel(unsigned int x, unsigned int y, fvec4 color)
{
	color.a = 1.f;
	if(x < w && y < h)
		pixels[y * w + x] = convVec4ToRGBA8(color);
}

void Texture::Draw()
{
	glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

void Texture::SaveAsBmp(const char* fname)
{
	stbi_write_bmp(fname, w, h, 4, pixels);
}

inline fvec4 Texture::GetPixel(unsigned int x, unsigned int y)
{
	y = y % h, x = x % w;
	return convRGBA8ToVec4(pixels[y * w + x]);
}

Texture::~Texture()
{
	if(pixels != nullptr)
		delete[] pixels;
}

fvec4 Texture::Fetch(float u, float v)
{
	if (!pixels)
		return fvec4(1);
	u = abs(u * w), v = abs(v * h);
	unsigned int x = floor(u), y = floor(v);
	//Bilinear interpolation and tiling
	float hor = abs(u - x), ver = abs(v - y);
	
	fvec4 color = (GetPixel(x, y) * (1 - hor) + GetPixel(x + 1, y) * hor) * (1 - ver) +
		(GetPixel(x, y + 1) * (1 - hor) + GetPixel(x + 1, y + 1) * hor) * ver;
	return color;
}
