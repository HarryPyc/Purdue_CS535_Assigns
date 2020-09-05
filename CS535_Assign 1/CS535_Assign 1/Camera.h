#pragma once
#include "Mat4.h"
class Camera
{
public:
	Mat4 P, V;

	Camera(fvec4 pos, fvec4 target, fvec4 up, float fovy, float aspect, float zNear, float zFar);
};

