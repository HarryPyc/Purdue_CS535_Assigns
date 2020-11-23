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

void Camera::SetTarget(fvec4 target)
{
	dir = Normalize(target - pos);
}

fvec4 Camera::Projection(fvec4 v)
{
	return P*V*v;
}

fvec4 Camera::InverseProjection(fvec4 screenPos, float w, float h)
{
	screenPos.x = ((screenPos.x / w) * 2.f - 1.f) * screenPos.w;
	screenPos.y = ((screenPos.y / h) * 2.f - 1.f) * screenPos.w;
	screenPos.z *= screenPos.w;
	return screenPos * InversePV;
}

void Camera::UpdateP()
{
	P = Perspective(fov, aspect, zNear, zFar);
	InversePV = Inverse(Transpose(P * V));
}

void Camera::UpdateV()
{
	V = LookAt(pos, pos+dir, up);
	InversePV = Inverse(Transpose(P * V));
}

void Camera::UploatToDevice(GLuint program)
{

	int P_loc = glGetUniformLocation(program, "P");
	if (P_loc != -1)
	{
		glUniformMatrix4fv(P_loc, 1, false, &P[0][0]);
	}
	int V_loc = glGetUniformLocation(program, "V");
	if (V_loc != -1)
	{
		glUniformMatrix4fv(V_loc, 1, false, &V[0][0]);
	}
	glUniform3fv(glGetUniformLocation(program, "camPos"), 1, &pos[0]);
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
