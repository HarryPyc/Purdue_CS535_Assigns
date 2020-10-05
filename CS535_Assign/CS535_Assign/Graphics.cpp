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
	Camera cam2(fvec4(40, 5, 5, 1), fvec4(40, 0, 0, 1), fvec4(0, 1, 0, 0), 60.f, float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.1f, 100.f);
	float angle = 0.f; int frames = 0;
	MainScene->meshList[0].texture = fbo1->screen;
	MainScene->meshList[1].texture = new Texture("asset/texture/Brick.jpg");
	MainScene->meshList[2].texture = new Texture("asset/texture/Brick.jpg");
	MainScene->meshList[3].texture = new Texture("asset/texture/Reflection.jpg");
	MainScene->meshList[4].texture = new Texture("asset/texture/Light.png");
	while (!glfwWindowShouldClose(window))
	{
		fbo->Clear(fvec4(0.f, 0.f, 0.f, 1));
		fbo->ClearZBuffer();
		fbo1->Clear(fvec4(0.35f, 0.35f, 0.35f, 1.0f));
		fbo1->ClearZBuffer();

		if (angle++ == 360.f) {
			angle = 0.f;
		}
		fbo1->DrawMesh(&cam2, &MainScene->meshList[5], DRAW_FILL);
		for (int i = 0; i < MainScene->meshList.size(); i++) {
			fbo->DrawMesh(&MainScene->MainCamera, &MainScene->meshList[i], DRAW_FILL);
		}
		MainScene->meshList[5].R = Rotate(angle, fvec4(0, 1, 0, 0));
		
		MainScene->MainCamera.TranslateCamera(fvec4(40.f / 300.f, 0, 0, 0));

		string fname = "output/result" + to_string(frames) + ".bmp";
		fbo->screen->SaveAsBmp(fname.c_str());
		fbo->screen->Draw();
		frames++;
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
