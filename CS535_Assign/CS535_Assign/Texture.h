#pragma once
#include <string>
#include "Define.h"


class Texture
{
public:
	Texture(std::string fname);
	~Texture();
	int w, h, components;
private:
	unsigned char* pixels;
};

