#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "FrameBuffer.h"
#include "Mesh.h"
#include "Camera.h"

class Graphics
{
public:
	int w, h;
	GLFWwindow* window;
	FrameBuffer* fbo;
	Camera* cam;
	vector<Mesh*> meshList;

	void run();
	Graphics(int _width, int _height);
	~Graphics();
};

