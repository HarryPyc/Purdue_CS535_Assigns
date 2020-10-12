#pragma once
#include <GL/glew.h>
#include "Vec4.h"
#include "Define.h"
#include "Camera.h"
#include "Texture.h"
#include "Mesh.h"



class FrameBuffer
{
public:

	void Clear(fvec4 color);
	void ClearZBuffer();
	void SetPixel(fvec4 pix, fvec4 color);
	void Draw2DSegements(fvec4 a, fvec4 b, Vertex vw0, Vertex vw1);
	void DrawTriangles(fvec4 v0, fvec4 v1, fvec4 v2, Vertex vw0, Vertex vw1, Vertex vw2, Texture* tex,  uint mode, bool light, FrameBuffer* projector);
	void DrawMesh(Mesh* mesh, uint mode, FrameBuffer *projector = NULL);

	FrameBuffer(int _width, int _height, Camera *cam, bool isDepthBuffer = false);
	FrameBuffer(Texture* project, Camera* cam);
	~FrameBuffer();
	Texture *screen;
	Camera* cam;

	float* zBuffer;
	bool isDepthBuffer;

	inline bool DepthTest(int x, int y, float z);
	inline float VisibilityTest(fvec4 worldPos);
	inline fvec4 PerspectiveDevide(fvec4 p);
	//Transformed Vertices
};

