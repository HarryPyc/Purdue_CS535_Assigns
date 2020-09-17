#include "Camera.h"
#include <iostream>
#include <fstream>
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

void Camera::SaveAsTxt(const std::string fileName)
{
	ofstream file;
	file.open(fileName.c_str());
	file << pos << dir << up << fovy << ' ' << aspect << ' ' << zNear << ' ' << zFar << endl;
	file.close();
}

void Camera::LoadFromTxt(const std::string fileName)
{
	ifstream file;
	file.open(fileName.c_str());
	file >> pos >> dir >> up >> fovy >> aspect >> zNear >> zFar;
	file.close();
	UpdateP();
	UpdateV();
}

void Camera::Interpolate(Camera* tCam, int i, int n)
{
	pos = pos + (tCam->pos - pos) * float(i) / float(n);
	dir = dir + (tCam->dir - dir) * float(i) / float(n);
	up = up + (tCam->up - up) * float(i) / float(n);
	UpdateV();
}
