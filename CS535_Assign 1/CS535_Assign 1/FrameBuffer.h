#pragma once
#include <GL/glew.h>
#include "Mat4.h"
#include "Camera.h"
#include "Mesh.h"
#define MULTI_PROCESS

class FrameBuffer
{
public:
	int w, h;

	void clear(fvec4 color);
	void SetPixel(int x, int y, fvec4 color);
	void Draw2DSegements(float a[2], float b[2], fvec4 color);
	void DrawMesh(Camera* cam, Mesh* mesh);

	void LoadTiff(char* fname);
	void SaveAsTiff(char* fname);

	FrameBuffer(int _width, int _height);
	~FrameBuffer() { delete pixels; }

private:
	unsigned int *pixels;
	//Transformed Vertices
};

