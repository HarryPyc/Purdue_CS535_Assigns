#include "Camera.h"

Camera::Camera(fvec4 pos, fvec4 target, fvec4 up, float fovy, float aspect, float zNear, float zFar)
{
	P = Perspective(fovy, aspect, zNear, zFar);
	V = LookAt(pos, target, up);
}
