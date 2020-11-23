#include "Graphics.h"
#include <string>
#include <InitShader.h>
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

void Graphics::runGPU()
{
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	GLuint shader = InitShader("shader/vs.vert", "shader/fs.frag");

	for (int i = 0; i < MainScene->meshList.size(); i++) {
		MainScene->meshList[i].CreateVao();
		if(MainScene->meshList[i].texture)
			MainScene->meshList[i].texture->CreateDeviceTexture();
	}

	while (!glfwWindowShouldClose(window))
	{
		glUseProgram(shader);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		MainScene->MainCamera.UploatToDevice(shader);
		MainScene->lightList[0].UploadToDevice(shader);

		for (int i = 0; i < MainScene->meshList.size(); i++) {
			MainScene->meshList[i].R = Rotate(1.f, fvec4(0, 1, 0, 0)) * MainScene->meshList[i].R;
			MainScene->meshList[i].DrawGPU(shader, GL_FILL);//GL_LINE
		}
		//Move Camera
		//MainScene->MainCamera.pos = Rotate(0.5f, fvec4(0, 1, 0, 0)) * MainScene->MainCamera.pos;
		//MainScene->MainCamera.SetTarget(fvec4(0, 0, 0, 1));
		//MainScene->MainCamera.UpdateV();
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
