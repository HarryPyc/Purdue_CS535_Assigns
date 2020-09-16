#pragma once
#include <GL/glew.h>
#include "Mat4.h"
#include "Camera.h"
#include "Mesh.h"
#include "Define.h"
#define MULTI_PROCESS

class FrameBuffer
{
public:
	int w, h;

	void Clear(fvec4 color);
	void SetPixel(int x, int y, fvec4 color);
	void Draw2DSegements(float* a, float* b, fvec4 c1, fvec4 c2);
	void DrawTriangles(float* v0, float* v1, float* v2, fvec4* color, uint mode);
	void DrawMesh(Camera* cam, Mesh* mesh, uint mode);

	void LoadTiff(const char* fname);
	void SaveAsTiff(const char* fname);

	FrameBuffer(int _width, int _height);
	~FrameBuffer() { delete pixels; }

private:
	unsigned int *pixels;
	//Transformed Vertices
	float* ConvToScreenSpace(fvec4 p);
};

