#include "Texture.h"
#include <omp.h>
#include <iostream>
#include <stb_image.h>
#include <stb_image_write.h>

Texture::Texture(std::string fname)
{
	unsigned char* cpixels;
	cpixels = stbi_load(fname.c_str(), &w, &h, &components, 4);
	if (cpixels) {
		std::cout << "Read image: " + fname << std::endl;
	}
	else
		return;
	pixels = new unsigned int[w * h];
	for (int i = 0; i < w; i++) {
		for(int j = 0; j < h; j++){
			const int index = (h - j) * w + i;
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
	if(x < w && y < h)
		pixels[y * w + x] = convVec4ToRGBA8(color);
}

void Texture::Draw()
{
	glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

void Texture::SaveAsBmp(const char* fname)
{
	unsigned char* p = new unsigned char[w * h * 4];
#ifdef MULTI_PROCESS
#pragma omp parallel for
#endif 
	for (int i = 0; i < w * h; i++) {
		fvec4 color = convRGBA8ToVec4(pixels[i]);
		p[4 * i + 0] = (char)color.r;
		p[4 * i + 1] = (char)color.g;
		p[4 * i + 2] = (char)color.b;
		p[4 * i + 3] = (char)color.a;
	}
	stbi_write_bmp(fname, w, h, 4, pixels);
	delete[] p;
}

Texture::~Texture()
{
	if(pixels != nullptr)
		delete[] pixels;
}

fvec4 Texture::Fetch(float u, float v)
{
	if (!pixels)
		return fvec4(0);
	u = u * w, v = v * h;
	unsigned int x = floor(u), y = floor(v);
	//Bilinear interpolation and tiling
	unsigned int uv[4][2] = {
		{x, y}, {x + 1,y},
		{x, y + 1}, {x + 1,y + 1}
	};
	fvec4 color;
	for (int i = 0; i < 4; i++) {
		x = uv[i][0] % w, y = uv[i][1] % h;
		color = color + convRGBA8ToVec4(pixels[y * w + x]);
	}
	
	return color / 4.f;
}
