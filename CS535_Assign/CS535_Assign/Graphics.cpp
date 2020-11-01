#include "Graphics.h"
#include <string>

extern Scene* MainScene;
extern bool ALPHA_BLEND;

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
	FrameBuffer* fbo = new FrameBuffer(w, h, &MainScene->MainCamera);

	int frames = 0;
	while (!glfwWindowShouldClose(window))
	{
		fbo->Clear();
		fbo->ClearZBuffer();
		
		for (int i = 0; i < MainScene->meshList.size(); i++) {
			MainScene->meshList[i].UploadVertex();
		}

		
		for (int i = 0; i < MainScene->meshList.size(); i++) {
			fbo->DrawMesh(&MainScene->meshList[i], DRAW_FILL);
		}
		MainScene->MainCamera.SetTarget(fvec4(0, 0, 0, 1));

		if (frames < 200) {
			MainScene->MainCamera.pos = Rotate(0.9f, fvec4(0, 1, 0, 0)) * MainScene->MainCamera.pos;
		}
		else if (frames < 400) {
			MainScene->MainCamera.pos = Rotate(0.9f, fvec4(1, 0, 0, 0)) * MainScene->MainCamera.pos;
		}
		else {
			MainScene->MainCamera.Roll(0.9f);
		}
		MainScene->MainCamera.UpdateV();


		//string fname = "output/result" + to_string(frames) + ".bmp";
		//fbo->screen->SaveAsBmp(fname.c_str());
		fbo->screen->Draw();
		frames++;
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
