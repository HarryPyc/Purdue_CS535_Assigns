#include "Texture.h"
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
	for (int i = 0; i < w * h; i++) {
		fvec4 color((float)cpixels[4 * i], (float)cpixels[4 * i + 1], (float)cpixels[4 * i + 2], (float)cpixels[4 * i + 3]);
		color = color / 255.f;
		pixels[i] = convVec4ToRGBA8(color);
	}
	delete[] cpixels;
}

Texture::~Texture()
{
	delete[] pixels;
}

fvec4 Texture::Fetch(float u, float v)
{
	if (!pixels)
		return fvec4(0);
	unsigned int x = u * w, y = v * h;
	x = x % w, y = h - y % h;
	return convRGBA8ToVec4(pixels[y * w + x]);
}
