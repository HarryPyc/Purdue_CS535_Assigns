#pragma once
#include "Mat4.h"
using namespace std;

class Camera
{
public:
	Mat4 P, V;

	Camera(fvec4 pos, fvec4 target, fvec4 up, float fovy, float aspect, float zNear, float zFar);
	Camera() {};
	void UpdateP();
	void UpdateV();
	fvec4 pos, dir, up;
	float fovy, aspect, zNear, zFar;

	void SaveAsTxt(const std::string fileName);
	void LoadFromTxt(const std::string fileName);
	void Interpolate(Camera* tCam, int i, int n);
};

