#include "Graphics.h"
#include <string>
Graphics::Graphics(int _width, int _height)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	window = glfwCreateWindow(_width, _height, "CS535_Yucong", NULL, NULL);
	if (!window) {
		std::cout << "Window Creation Failed" << std::endl;
	}
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW initialization failed.\n";
	}
	glfwSwapInterval(1);
	w = _width, h = _height;

	fbo = new FrameBuffer(w, h);
	cam = new Camera(fvec4(0, 5, 8, 1), fvec4(0), fvec4(0, 1, 0, 0), 60.f, float(w) / float(h), 0.1f, 100.f);
	cam2 = new Camera(fvec4(0, -5, 8, 1), fvec4(0), fvec4(0, 1, 0, 0), 60.f, float(w) / float(h), 0.1f, 100.f);

	Mesh* Cube = new Mesh(); Cube->LoadObj("asset/model/cube.obj"); Cube->material.color = fvec4(1);
	Mesh* Fish = new Mesh(); Fish->LoadObj("asset/model/Amago0.obj"); Fish->material.color = fvec4(1); Fish->S = Scale(fvec4(10, 10, 10, 1));
	Mesh* Cube1 = new Mesh(*Cube); Cube1->material.color = fvec4(1, 0, 0, 1); Cube1->T = Translate(fvec4(2, 0, 0, 0));
	Mesh* Cube2 = new Mesh(*Cube); Cube2->material.color = fvec4(0, 1, 0, 1); Cube2->T = Translate(fvec4(-2, 0, 0, 0));
	Mesh* Cube3 = new Mesh(*Cube); Cube3->material.color = fvec4(0, 0, 1, 1); Cube3->T = Translate(fvec4(0, 0, 2, 0));
	Mesh* Cube4 = new Mesh(*Cube); Cube4->material.color = fvec4(1, 1, 0, 1); Cube4->T = Translate(fvec4(0, 0, -2, 0));
	meshList.push_back(Cube); meshList.push_back(Cube1); meshList.push_back(Cube2); meshList.push_back(Cube3); meshList.push_back(Cube4);
}

Graphics::~Graphics() { 
	glfwDestroyWindow(window); 
	delete fbo, cam;
	for (int i = 0; i < meshList.size(); i++) {
		delete meshList[i];
	}
}


void Graphics::run()
{
	float angle = 0.f; int frames = 0;
	while (!glfwWindowShouldClose(window))
	{
		fbo->Clear(fvec4(0));
		fbo->ClearZBuffer();
		if (angle++ == 360.f) {
			angle = 0.f;
		}
		//if (frames++ >= 150) {
		//	cam->Interpolate(cam2, frames - 150, 150);
		//}
		for (int i = 0; i < meshList.size(); i++) {
			fbo->DrawMesh(cam, meshList[i],DRAW_FILL);
		}
		meshList[0]->R = Rotate(angle, fvec4(0, 1, 0, 0));
		meshList[1]->R = Rotate(angle, fvec4(1, 0, 0, 0));
		meshList[2]->R = Rotate(angle, fvec4(0, 0, 1, 0));
		meshList[3]->R = Rotate(angle, fvec4(1, 1, 0, 0));
		meshList[4]->R = Rotate(angle, fvec4(1, 0, 1, 0));
		/*string fname = "output/result" + to_string(frames) + ".tif";
		fbo->SaveAsTiff(fname.c_str());*/

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
