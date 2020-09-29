#pragma once
#include <GL/glew.h>
#include "Mat4.h"
#include "Camera.h"
#include "Mesh.h"
#include "Define.h"
#include "Texture.h"


class FrameBuffer
{
public:
	int w, h;

	void Clear(fvec4 color);
	void ClearZBuffer();
	void SetPixel(fvec4 pix, fvec4 color);
	void Draw2DSegements(fvec4 a, fvec4 b, Vertex vw0, Vertex vw1);
	void DrawTriangles(fvec4 v0, fvec4 v1, fvec4 v2, Vertex vw0, Vertex vw1, Vertex vw2, Texture* tex, uint mode);
	void DrawMesh(Camera* cam, Mesh* mesh, uint mode);

	void SaveAsBmp(const char* fname);

	FrameBuffer(int _width, int _height);
	~FrameBuffer() { delete[] pixels, zBuffer; }

private:
	unsigned int *pixels;
	float* zBuffer;

	inline bool DepthTest(int x, int y, float z);
	//Transformed Vertices
	fvec4 ConvToScreenSpace(fvec4 p);
};

