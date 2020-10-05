#include "Scene.h"

Scene::~Scene()
{
}


Scene* InitMainScene()
{
	Scene* scene = new Scene();

	scene->MainCamera = Camera(fvec4(0, 0, 8, 1), fvec4(0, 0, 0, 1), fvec4(0, 1, 0, 0), 60.f, float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.1f, 100.f);
	
	//Mesh* Cube = new Mesh(); Cube->LoadObj("asset/model/cube.obj"); Cube->material.color = fvec4(1);
	//Cube->texture = new Texture("asset/texture/Cube_tex.jpg");
	//meshList.push_back(Cube);


	Mesh Plane; 
	Plane.LoadObj("asset/model/Plane.obj"); Plane.material.color = fvec4(1);
	Plane.S = Scale(fvec4(4, 4, 4, 1)); Plane.T = Translate(fvec4(0, 0, 0, 0)); Plane.R = Rotate(90, fvec4(1, 0, 0, 0));
	scene->meshList.push_back(Plane);

	Mesh Plane1; Plane1.LoadObj("asset/model/Plane_Tiled.obj"); Plane1.material.color = fvec4(1, 0, 0, 1);
	Plane1.R = Plane.R; Plane1.S = Scale(fvec4(4, 4, 4, 1)); Plane1.T = Translate(fvec4(8, 0, 0, 0));
	scene->meshList.push_back(Plane1);

	Mesh Plane2(Plane); Plane2.material.color = fvec4(1, 1, 0, 1);
	Plane2.T = Translate(fvec4(16, 0, 0, 0));
	scene->meshList.push_back(Plane2);

	Mesh Plane3(Plane); Plane3.material.color = fvec4(0, 0, 1, 1);
	Plane3.T = Translate(fvec4(24, 0, 0, 0));
	scene->meshList.push_back(Plane3);

	Mesh Plane4(Plane); Plane4.material.color = fvec4(0, 1, 0, 1);
	Plane4.T = Translate(fvec4(32, 0, 0, 0));
	scene->meshList.push_back(Plane4);

	Mesh Fish;
	Fish.LoadObj("asset/model/Amago0.obj"); Fish.S = Scale(fvec4(15, 15, 15, 1)); Fish.T = Translate(fvec4(40, 0, 0, 0));
	Fish.texture = new Texture("asset/texture/AmagoT.bmp"); Fish.enableLight = true;
	scene->meshList.push_back(Fish);


	PointLight light = PointLight(fvec4(40, 1, 2, 1), fvec4(2));
	scene->lightList.push_back(light);
	return scene;
}