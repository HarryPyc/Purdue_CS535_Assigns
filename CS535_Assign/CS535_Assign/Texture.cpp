#include "Texture.h"
#include <iostream>
#include <stb_image.h>
#include <stb_image_write.h>

Texture::Texture(std::string fname)
{
	pixels = stbi_load(fname.c_str(), &w, &h, &components, 4);
	if (pixels) {
		std::cout << "Read image: " + fname << std::endl;
	}
}

Texture::~Texture()
{
	delete[] pixels;
}
