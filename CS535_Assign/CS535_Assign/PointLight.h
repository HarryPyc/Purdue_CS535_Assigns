#pragma once
#include "Vec4.h"


class PointLight
{
public:
	fvec4 color, pos;

	PointLight(fvec4 p, fvec4 c);
	~PointLight() {}

	fvec4 PhongLighting(fvec4 p, fvec4 n, float Ka, float Kd, float Ks, float alpha, fvec4 camPos);
};

