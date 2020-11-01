#include "Scene.h"

Scene::~Scene()
{
}


Scene* InitMainScene()
{
	Scene* scene = new Scene();

	scene->MainCamera = Camera(fvec4(0, 0, 4, 1), fvec4(0, 0, 0, 1), fvec4(0, 1, 0, 0), 70.f, float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.1f, 100.f);
	
	//Mesh Plane("asset/model/Plane.obj"); Plane.material.color = fvec4(1.f, 1.f, 1.f, 1.f); Plane.S = Scale(fvec4(3, 3, 3, 1));
	//Plane.texture = new Texture("asset/texture/Light.png");
	//Plane.enableLight = true;
	//scene->meshList.push_back(Plane);

	//Mesh Cube("asset/model/cube.obj");  Cube.material.color = fvec4(1.f, 1.f, 1.f, 0.0f); 
	//Cube.S = Scale(fvec4(1.f));
	//Cube.enableLight = false;
	//scene->meshList.push_back(Cube);

	Mesh Fish;
	Fish.LoadObj("asset/model/Amago0.obj"); Fish.S = Scale(fvec4(10, 10, 10, 1)); Fish.T = Translate(fvec4(0, 0.8f, 0, 0));
	scene->meshList.push_back(Fish);

	//Mesh Bunny;
	//Bunny.LoadObj("asset/model/bunny.obj");  Bunny.T = Translate(fvec4(0, 0, 0.f, 0)); Bunny.S = Scale(fvec4(2));
	//Bunny.material.color = fvec4(1, 1, 1, 1); Bunny.enableLight = false;
	//scene->meshList.push_back(Bunny);

	PointLight light = PointLight(fvec4(0, 3, 2.f, 1), fvec4(1.f));
	scene->lightList.push_back(light);


	return scene;
}

Scene* MainScene = InitMainScene();