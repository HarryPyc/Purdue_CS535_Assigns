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
#ifdef SHOWSHADOEMAPPING
	Camera Cam2(fvec4(0, 3, -1.5, 1), fvec4(0, 0, -1.5f, 1), fvec4(1, 0, 0, 0), 90.f, (float)w / (float)h, 0.1f, 100.f);
	FrameBuffer* projector = new FrameBuffer(SHADOWRES, SHADOWRES, &Cam2, true);
	MainScene->lightList[0].shadowMap = projector;
#else
	Camera Cam2(fvec4(0, 3, -1.5, 1), fvec4(0, 0, -1.5f, 1), fvec4(0, 0, -1, 0), 60.f, (float)w / (float)h, 0.1f, 100.f);
	Texture proj("asset/texture/batmanlogo.jpg");
	FrameBuffer* projector = new FrameBuffer(&proj, &Cam2);
#endif // SHOWSHADOWMAPPING
	int frames = 0;
	while (!glfwWindowShouldClose(window))
	{
		fbo->Clear(fvec4(0.f, 0.f, 0.f, 1));
		fbo->ClearZBuffer();
		projector->ClearZBuffer();

		for (int i = 0; i < MainScene->meshList.size(); i++) {
			MainScene->meshList[i].UploadVertex();
		}

		for (int i = 0; i < MainScene->meshList.size(); i++) {
			projector->DrawMesh(&MainScene->meshList[i], DRAW_FILL);
		}

		for (int i = 0; i < MainScene->meshList.size(); i++) {
#ifdef SHOWSHADOEMAPPING
			fbo->DrawMesh(&MainScene->meshList[i], DRAW_FILL);
#else
			fbo->DrawMesh(&MainScene->meshList[i], DRAW_FILL, projector);
#endif // SHOWSHADOWMAPPING
		}

#ifdef SHOWSHADOEMAPPING
		MainScene->lightList[0].TranslateLight(fvec4(0, 0, 0.01f, 0));
		MainScene->meshList[1].R = Rotate(frames, fvec4(0, 1, 0, 0));
#else
		Cam2.TranslateCamera(fvec4(0, 0, 0.01f, 0));
#endif // SHOWSHADOWMAPPING

		//string fname = "output/result" + to_string(frames) + ".bmp";
		//fbo->screen->SaveAsBmp(fname.c_str());
		fbo->screen->Draw();
		frames++;
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
