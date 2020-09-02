#pragma once
#include <GL/glew.h>


class FrameBuffer
{
public:
	int w, h;

	void draw();
	void clear();

	FrameBuffer(int _width, int _height);
	~FrameBuffer() { delete pixels; }

private:
	unsigned int *pixels;
};

