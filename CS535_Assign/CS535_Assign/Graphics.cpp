#include "Graphics.h"
#include <string>
Graphics::Graphics()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	w = WINDOW_WIDTH, h = WINDOW_HEIGHT;
	window = glfwCreateWindow(w, h, "CS535_Yucong", NULL, NULL);
	if (!window) {
		std::cout << "Window Creation Failed" << std::endl;
	}
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW initialization failed.\n";
	}
	glfwSwapInterval(1);


}

Graphics::~Graphics() { 
	glfwDestroyWindow(window); 
}


void Graphics::run()
{
	FrameBuffer *fbo = new FrameBuffer(w, h), *fbo1 = new FrameBuffer(w, h);
	float angle = 0.f; int frames = 0;
	while (!glfwWindowShouldClose(window))
	{
		fbo->Clear(fvec4(0.35, 0.35, 0.35, 1));
		fbo->ClearZBuffer();
		/*fbo1->Clear(fvec4(0.f));
		fbo1->ClearZBuffer();*/
		if (angle++ == 360.f) {
			angle = 0.f;
		}

		/*for (int i = 0; i < meshList.size(); i++) {
			fbo->DrawMesh(cam, meshList[i],DRAW_FILL);
		}*/
		fbo->DrawMesh(&MainScene->MainCamera, &MainScene->meshList[0], DRAW_FILL);
		//fbo1->DrawMesh(cam, meshList[1], DRAW_FILL);
		MainScene->meshList[0].R = Rotate(angle, fvec4(0, 1, 0, 0));

		//string fname = "output/result" + to_string(frames) + ".bmp";
		//fbo->SaveAsBmp(fname.c_str());
		fbo->screen->Draw();
		frames++;
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
