#include "Scene.h"

Scene::~Scene()
{
}

Scene* InitMainScene()
{
	Scene* scene = new Scene();

	scene->MainCamera = Camera(fvec4(0, 2, 8, 1), fvec4(0), fvec4(0, 1, 0, 0), 60.f, float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.1f, 100.f);

	//Mesh* Cube = new Mesh(); Cube->LoadObj("asset/model/cube.obj"); Cube->material.color = fvec4(1);
	//Cube->texture = new Texture("asset/texture/Cube_tex.jpg");
	//meshList.push_back(Cube);

	Mesh Fish; Fish.LoadObj("asset/model/Amago0.obj"); Fish.material.color = fvec4(1); Fish.S = Scale(fvec4(20, 20, 20, 1));
	Fish.texture = new Texture("asset/texture/AmagoT.bmp");
	scene->meshList.push_back(Fish);

	/*Mesh* Plane = new Mesh(); Plane->LoadObj("asset/model/Plane.obj"); Plane->material.color = fvec4(1);
	Plane->S = Scale(fvec4(4, 4, 4, 1)); Plane->T = Translate(fvec4(0, 0, -1, 0)); Plane->R = Rotate(90, fvec4(1, 0, 0, 0));
	scene.meshList.push_back(Plane);*/

	//Mesh* Cube1 = new Mesh(*Cube); Cube1->material.color = fvec4(1, 0, 0, 1); Cube1->T = Translate(fvec4(2, 0, 0, 0));
	//Mesh* Cube2 = new Mesh(*Cube); Cube2->material.color = fvec4(0, 1, 0, 1); Cube2->T = Translate(fvec4(-2, 0, 0, 0));
	//Mesh* Cube3 = new Mesh(*Cube); Cube3->material.color = fvec4(0, 0, 1, 1); Cube3->T = Translate(fvec4(0, 0, 2, 0));
	//Mesh* Cube4 = new Mesh(*Cube); Cube4->material.color = fvec4(1, 1, 0, 1); Cube4->T = Translate(fvec4(0, 0, -2, 0));
	//meshList.push_back(Cube); meshList.push_back(Cube1); meshList.push_back(Cube2); meshList.push_back(Cube3); meshList.push_back(Cube4);
	PointLight light = PointLight(fvec4(0, 2, 2, 1), fvec4(1));
	scene->lightList.push_back(light);
	return scene;
}