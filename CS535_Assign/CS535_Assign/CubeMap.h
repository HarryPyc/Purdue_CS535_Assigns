#pragma once
#include "ObjLoad.h"
#include "Texture.h"
#include "Vec4.h"
class CubeMap
{
public:
	CubeMap();
	~CubeMap();

	fvec4 Fetch(fvec4 o, fvec4 d);
private:
	obj::Model cube;
	Texture *texture;
	int previous;
};

