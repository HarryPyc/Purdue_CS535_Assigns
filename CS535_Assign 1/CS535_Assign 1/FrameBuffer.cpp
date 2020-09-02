#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(int _width, int _height)
{
	w = _width; h = _height;
	pixels = new unsigned int[w * h];

	for (int i = 0; i < w * h; i++)
		*(pixels + i) = 0x000000FF;
}

void FrameBuffer::draw()
{
	glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

void FrameBuffer::clear()
{
}
