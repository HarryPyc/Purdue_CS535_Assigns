#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Graphics.h"
#include "Vec4.h"
#include "Mat4.h"
#define GLEW_STATIC



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
	if (!glfwInit()) {
		std::cout << "GLFW Init Failed" << std::endl;
	}

	glfwSetErrorCallback(error_callback);

	Graphics* graphics = new Graphics();
	printGlInfo();

	graphics->runGPU();

	delete graphics;
	glfwTerminate();
	

	return 0;
}