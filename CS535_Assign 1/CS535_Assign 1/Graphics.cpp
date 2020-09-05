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
	cam = new Camera(fvec4(3, 3, 3, 1), fvec4(0, 0, 0, 1), fvec4(0, 1, 0, 0),
		60.f, float(w) / float(h), 0.1f, 100.f);

	Mesh *AxisX = new Mesh(), *AxisY = new Mesh(), *AxisZ = new Mesh(), *Point = new Mesh();
	AxisX->AddVertex(fvec4(0, 0, 0, 1)); AxisX->AddVertex(fvec4(1, 0, 0, 1)); AxisX->material.color = fvec4(0, 1, 0, 1);
	AxisY->AddVertex(fvec4(0, 0, 0, 1)); AxisY->AddVertex(fvec4(0, 1, 0, 1)); AxisY->material.color = fvec4(1, 0, 0, 1);
	AxisZ->AddVertex(fvec4(0, 0, 0, 1)); AxisZ->AddVertex(fvec4(0, 0, 1, 1)); AxisZ->material.color = fvec4(0, 0, 1, 1);
	Point->AddVertex(fvec4(0.5, 0.5, 0.5, 1)); Point->AddVertex(fvec4(0.5, 0.5, 0.5, 1)); Point->material.color = fvec4(1, 1, 0, 1);
	Mesh* Point2 = new Mesh(*Point); Point2->material.color = fvec4(0, 1, 1, 1);
	meshList.push_back(AxisX); meshList.push_back(AxisY); meshList.push_back(AxisZ); meshList.push_back(Point), meshList.push_back(Point2);
	//Triangle2.CopyVertices(Triangle1);
	//Triangle2.R = Rotate(90.f, fvec4(1, 0, 0, 0));
	//meshList.push_back(Triangle2);
}


void Graphics::run()
{
	float angle = 0.f;
	while (!glfwWindowShouldClose(window))
	{
		
		if (angle == 360.f) {
			angle = 0.f;
			fbo->Clear(fvec4(0));
		}
		for (int i = 0; i < meshList.size(); i++) {
			fbo->DrawMesh(cam, meshList[i]);
		}
		/*string fname = "output/result" + to_string(int(angle)) + ".tif";
		fbo->SaveAsTiff(fname.c_str());*/
		meshList[3]->R = Rotate(angle, fvec4(0, 1, 0, 0));
		meshList[4]->R = Rotate(angle++, fvec4(0, 1, 1, 0));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
