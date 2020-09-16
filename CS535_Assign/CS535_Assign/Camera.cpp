#include "Camera.h"

Camera::Camera(fvec4 _pos, fvec4 _target, fvec4 _up, float _fovy, float _aspect, float _zNear, float _zFar) :
	pos(_pos), dir(Normalize(_target - _pos)), up(_up), fovy(_fovy), aspect(_aspect), zNear(_zNear), zFar(_zFar)
{
	UpdateP();
	UpdateV();
}

void Camera::UpdateP()
{
	P = Perspective(fovy, aspect, zNear, zFar);
}

void Camera::UpdateV()
{
	V = LookAt(pos, pos+dir, up);
}
