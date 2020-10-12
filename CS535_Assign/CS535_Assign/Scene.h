#pragma once
#include "Mesh.h"
#include "PointLight.h"
#include "Camera.h"
#include <vector>

class Scene
{
public:
	vector<Mesh> meshList;
	vector<PointLight> lightList;
	Camera MainCamera;

	

	Scene() {}
	~Scene();
};



