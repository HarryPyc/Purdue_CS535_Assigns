#pragma once
#include "Mat4.h"
using namespace std;

class Camera
{
public:
	Mat4 P, V, InversePV;

	Camera(fvec4 pos, fvec4 target, fvec4 up, float fov, float aspect, float zNear, float zFar);
	Camera() {};
	void Pan(float angle);
	void Tilt(float angle);
	void Roll(float angle);
	void TranslateCamera(fvec4 t);
	void SetFocalLength(float l);
	fvec4 Projection(fvec4 v);
	fvec4 InverseProjection(fvec4 screenPos, float w, float h);

	void UpdateP();
	void UpdateV();
	fvec4 pos, dir, up;
	float fov, aspect, zNear, zFar;

	void SaveAsTxt(const std::string fileName);
	void LoadFromTxt(const std::string fileName);
	void Interpolate(Camera* Cam0, Camera* Cam1, int i, int n);
};

