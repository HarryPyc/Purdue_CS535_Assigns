#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "FrameBuffer.h"
#include "Mesh.h"
#include "Camera.h"
#include "Scene.h"

class Graphics
{
public:
	int w, h;
	GLFWwindow* window;


	void run();
	Graphics();
	~Graphics();
};

