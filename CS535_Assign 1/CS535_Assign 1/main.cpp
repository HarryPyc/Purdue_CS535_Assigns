#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Graphics.h"
#include "Vec4.h"
#define GLEW_STATIC

const int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}
void printGlInfo()
{
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

int main(int argc, char** argv) {
	/*if (!glfwInit()) {
		std::cout << "GLFW Init Failed" << std::endl;
	}

	glfwSetErrorCallback(error_callback);

	Graphics* graphics = new Graphics(WINDOW_WIDTH, WINDOW_HEIGHT);
	printGlInfo();

	graphics->run();

	delete graphics;
	glfwTerminate();*/

	Vec4<float> v1(1,0,0,0), v2(0,1,0,0);
	Vec4<float> v3 = v1 + 3, v4=v1-3, v5 = v1*3, v6=v1/3;
	std::cout << v3 << std::endl;
	std::cout << v4 << std::endl;
	std::cout << v5 << std::endl;
	std::cout << v6 << std::endl;
	std::cout << Dot(v3, v4) << std::endl;
	std::cout << Cross(v1, v2) << std::endl;
	std::cout << Vec4<float>(1,1,1,1).Length() << std::endl;
	std::cout << Normalize(v3) << std::endl;
	return 0;
}