#include "Scene.h"

Scene::~Scene()
{
}


Scene* InitMainScene()
{
	Scene* scene = new Scene();

	scene->MainCamera = Camera(fvec4(0, 5, 5, 1), fvec4(0, 0, 0, 1), fvec4(0, 1, 0, 0), 60.f, float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.1f, 100.f);
	
	//Mesh* Cube = new Mesh(); Cube->LoadObj("asset/model/cube.obj"); Cube->material.color = fvec4(1);
	//Cube->texture = new Texture("asset/texture/Cube_tex.jpg");
	//meshList.push_back(Cube);

	Mesh Fish;
	Fish.LoadObj("asset/model/Amago0.obj"); Fish.S = Scale(fvec4(15, 15, 15, 1)); Fish.T = Translate(fvec4(0, 1.2, 0, 0));
	Fish.texture = new Texture("asset/texture/AmagoT.bmp"); Fish.enableLight = true;
	scene->meshList.push_back(Fish);

	Mesh Plane("asset/model/Plane.obj"); Plane.material.color = fvec4(1); Plane.S = Scale(fvec4(3, 3, 3, 1));
	Plane.enableLight = true;
	scene->meshList.push_back(Plane);


	PointLight light = PointLight(fvec4(2, 3, 2, 1), fvec4(2.f));
	scene->lightList.push_back(light);
	return scene;
}