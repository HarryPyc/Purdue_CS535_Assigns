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
	Camera cam2(fvec4(2, 3, 2, 1), fvec4(0, 0, 0, 1), fvec4(0, 1, 0, 0), 100.f, float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.1f, 100.f);
	FrameBuffer *fbo = new FrameBuffer(w, h, &MainScene->MainCamera), *shadowMap = new FrameBuffer(w, h, &cam2);

	float angle = 0.f; int frames = 0;
	while (!glfwWindowShouldClose(window))
	{
		fbo->Clear(fvec4(0.f, 0.f, 0.f, 1));
		fbo->ClearZBuffer();
		shadowMap->Clear(fvec4(0.35f, 0.35f, 0.35f, 1.0f));
		shadowMap->ClearZBuffer();

		if (angle++ == 360.f) {
			angle = 0.f;
		}

		for (int i = 0; i < MainScene->meshList.size(); i++) {
			shadowMap->DrawMesh(&MainScene->meshList[i], NULL, DRAW_FILL);
			fbo->DrawMesh(&MainScene->meshList[i], shadowMap, DRAW_FILL);
		}
		MainScene->meshList[0].R = Rotate(angle, fvec4(0, 1, 0, 0));
		

		//string fname = "output/result" + to_string(frames) + ".bmp";
		//fbo->screen->SaveAsBmp(fname.c_str());
		fbo->screen->Draw();
		frames++;
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
