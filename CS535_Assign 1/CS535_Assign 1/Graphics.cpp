#include "Graphics.h"

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
	glfwGetFramebufferSize(window, &w, &h);

	fbo = new FrameBuffer(w, h);
}

void Graphics::run()
{
	while (!glfwWindowShouldClose(window))
	{
		glViewport(0, 0, w, h);
		fbo->draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
