#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "FrameBuffer.h"

class Graphics
{
public:
	int w, h;
	GLFWwindow* window;
	FrameBuffer* fbo;

	void run();
	Graphics(int _width, int _height);
	~Graphics() { glfwDestroyWindow(window); };
};

