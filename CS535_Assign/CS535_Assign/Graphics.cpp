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
	cam = new Camera(fvec4(2, 2, 3, 1), fvec4(0, 0, 0, 1), fvec4(0, 1, 0, 0),
		60.f, float(w) / float(h), 0.1f, 100.f);

	Mesh* Cube = new Mesh();
	Cube->LoadObj("asset/model/cube.obj"); Cube->material.color = fvec4(1);
	meshList.push_back(Cube); 
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
	float angle = 0.f;
	while (!glfwWindowShouldClose(window))
	{
		fbo->Clear(fvec4(0));
		if (angle++ == 360.f) {
			angle = 0.f;
		}
		for (int i = 0; i < meshList.size(); i++) {
			fbo->DrawMesh(cam, meshList[i], DRAW_FILL);
		}
		meshList[0]->R = Rotate(angle, fvec4(0, 1, 0, 0));
		/*string fname = "output/result" + to_string(int(angle)) + ".tif";
		fbo->SaveAsTiff(fname.c_str());*/

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
