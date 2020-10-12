#include "Scene.h"

Scene::~Scene()
{
}


Scene* InitMainScene()
{
	Scene* scene = new Scene();

	scene->MainCamera = Camera(fvec4(0, 5, 5, 1), fvec4(0, 0, 0, 1), fvec4(0, 1, 0, 0), 70.f, float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.1f, 100.f);
	
	Mesh Plane("asset/model/Plane.obj"); Plane.material.color = fvec4(1.f, 1.f, 1.f, 1.f); Plane.S = Scale(fvec4(3, 3, 3, 1));
#ifdef SHOWSHADOEMAPPING
	Plane.enableLight = true;
#endif // SHOWSHADOEMAPPING
	scene->meshList.push_back(Plane);

	//Mesh Cube("asset/model/cube.obj");  Cube.material.color = fvec4(1.f, 0.f, 0.f, 0.0f); Cube.T = Translate(fvec4(-1.5f, 1, -1.5f, 0));
	//Cube.S = Scale(fvec4(0.5f));
	//Cube.enableLight = false;
	//scene->meshList.push_back(Cube);

	Mesh Fish;
	Fish.LoadObj("asset/model/Amago0.obj"); Fish.S = Scale(fvec4(10, 10, 10, 1)); Fish.T = Translate(fvec4(0, 0.8f, 0, 0));
#ifndef SHOWSHADOEMAPPING
	Fish.R = Rotate(90.f, fvec4(1, 0, 0, 0)); 
#else
	Fish.enableLight = true;
#endif // !SHOWSHADOWMAPPING
	Fish.texture = new Texture("asset/texture/AmagoT.bmp"); 
	scene->meshList.push_back(Fish);

#ifdef SHOWSHADOEMAPPING
	Mesh Bunny;
	Bunny.LoadObj("asset/model/bunny.obj");  Bunny.T = Translate(fvec4(0, -0.1f, 2.f, 0));
	Bunny.material.color = fvec4(1, 1, 0, 1); Bunny.enableLight = true;
	scene->meshList.push_back(Bunny);

	PointLight light = PointLight(fvec4(0, 3, -1.5f, 1), fvec4(2.f));
	scene->lightList.push_back(light);
#endif // SHOWSHADOEMAPPING


	return scene;
}

Scene* MainScene = InitMainScene();