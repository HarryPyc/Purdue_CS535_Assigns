#include "Camera.h"
#include <iostream>
#include <fstream>
Camera::Camera(fvec4 _pos, fvec4 _target, fvec4 _up, float _fov, float _aspect, float _zNear, float _zFar) :
	pos(_pos), dir(Normalize(_target - _pos)), up(_up), fov(_fov), aspect(_aspect), zNear(_zNear), zFar(_zFar)
{
	UpdateP();
	UpdateV();
}

void Camera::Pan(float angle)
{
	Mat4 R = Rotate(angle, up);
	dir = R * dir;
	UpdateV();
}
void Camera::Tilt(float angle)
{
	fvec4 axis = Cross(up, dir);
	Mat4 R = Rotate(angle, axis);
	dir = R * dir;
	UpdateV();
}
void Camera::Roll(float angle)
{
	Mat4 R = Rotate(angle, dir);
	up = R * up;
	UpdateV();
}

void Camera::TranslateCamera(fvec4 t)
{
	pos = Translate(t) * pos;
	UpdateV();
}

void Camera::SetFocalLength(float l)
{
	fov = 2 * atan(1.f / l);
	UpdateP();
}

fvec4 Camera::Projection(fvec4 v)
{
	return P*V*v;
}

void Camera::UpdateP()
{
	P = Perspective(fov, aspect, zNear, zFar);
}

void Camera::UpdateV()
{
	V = LookAt(pos, pos+dir, up);
}

void Camera::SaveAsTxt(const std::string fileName)
{
	ofstream file;
	file.open(fileName.c_str());
	file << pos << dir << up << fov << ' ' << aspect << ' ' << zNear << ' ' << zFar << endl;
	file.close();
}

void Camera::LoadFromTxt(const std::string fileName)
{
	ifstream file;
	file.open(fileName.c_str());
	file >> pos >> dir >> up >> fov >> aspect >> zNear >> zFar;
	file.close();
	UpdateP();
	UpdateV();
}

void Camera::Interpolate(Camera *Cam0, Camera* Cam1, int i, int n)
{
	pos = Cam0->pos + (Cam1->pos - Cam0->pos) * float(i) / float(n);
	dir = Cam0->dir + (Cam1->dir - Cam0->dir) * float(i) / float(n);
	up = Cam0->up + (Cam1->up - Cam0->up) * float(i) / float(n);
	UpdateV();
}
