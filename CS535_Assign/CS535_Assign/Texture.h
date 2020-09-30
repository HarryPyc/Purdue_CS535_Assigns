#pragma once
#include <string>
#include "Define.h"
#include "Vec4.h"


class Texture
{
public:
	Texture(std::string fname);
	~Texture();

	fvec4 Fetch(float u, float v);
	int w, h, components;
private:
	unsigned int* pixels;
};

