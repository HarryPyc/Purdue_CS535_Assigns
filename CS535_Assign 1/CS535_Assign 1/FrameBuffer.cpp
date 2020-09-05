#include "FrameBuffer.h"
#include <libtiff/tiffio.h>
#include <omp.h>

FrameBuffer::FrameBuffer(int _width, int _height)
{
	w = _width; h = _height;
	pixels = new unsigned int[w * h];
	Clear(fvec4(0.f, 0.f, 0.f, 0.f));

}



void FrameBuffer::Clear(fvec4 color)
{
	const uint c = convVec4ToRGBA8(color);
#ifdef MULTI_PROCESS
#pragma omp parallel for
#endif 
	for (int i = 0; i < w * h; i++)
		*(pixels + i) = c;
}

void FrameBuffer::SetPixel(int x, int y, fvec4 color)
{
	const int index = y*w + x;
	if (x < 0 || y < 0 || x >= w || y >= h) {
		//cout << "Index out of range of Pixels" << endl;
		return;
	}
	*(pixels + index) = convVec4ToRGBA8(color);
}



void FrameBuffer::Draw2DSegements(float a[2], float b[2], fvec4 color)
{
	float Dx = fabsf(a[0] - b[0]), Dy = fabsf(a[1] - b[1]);
	const int steps = int(Dx > Dy ? Dx : Dy) + 1;
#ifdef MULTI_PROCESS
#pragma omp parallel for
#endif 
	for (int i = 0; i <= steps; i++) {
		float x = a[0] + (b[0] - a[0]) / float(steps) * float(i);
		float y = a[1] + (b[1] - a[1]) / float(steps) * float(i);
		SetPixel(int(x), int(y), color);
	}

}

void FrameBuffer::DrawMesh(Camera* cam, Mesh* mesh)
{
	const int n = mesh->GetSize();
	vector<float[2]> Vertices = vector<float[2]>(n);
#ifdef MULTI_PROCESS
#pragma omp parallel for
#endif 
	for (int i = 0; i < n; i++) {
		fvec4 screenVert = cam->P * cam->V * mesh->T * mesh->R * mesh->S * mesh->GetVertex(i);
		//Perspective devision
		screenVert = screenVert / screenVert.w;
		Vertices[i][0] = (screenVert.x + 1.f) / 2.f * w;
		Vertices[i][1] = (screenVert.y + 1.f) / 2.f * h;
	}


	for (int i = 0; i < n - 1; i++) {
		Draw2DSegements(Vertices[i], Vertices[i + 1], mesh->material.color);
	}
	glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

void FrameBuffer::LoadTiff(const char* fname) {
	TIFF* in = TIFFOpen(fname, "r");
	if (in == NULL) {
		cerr << fname << " could not be opened" << endl;
		return;
	}

	int width, height;
	TIFFGetField(in, TIFFTAG_IMAGEWIDTH, &width);
	TIFFGetField(in, TIFFTAG_IMAGELENGTH, &height);
	if (w != width || h != height) {
		w = width;
		h = height;
		delete[] pixels;
		pixels = new unsigned int[w * h];
		glFlush();
	}

	if (TIFFReadRGBAImage(in, w, h, pixels, 0) == 0) {
		cerr << "failed to load " << fname << endl;
	}

	TIFFClose(in);
}
void FrameBuffer::SaveAsTiff(const char* fname) {
	TIFF* out = TIFFOpen(fname, "w");

	if (out == NULL) {
		cerr << fname << " could not be opened" << endl;
		return;
	}

	TIFFSetField(out, TIFFTAG_IMAGEWIDTH, w);
	TIFFSetField(out, TIFFTAG_IMAGELENGTH, h);
	TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, 4);
	TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);
	TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

	for (uint32 row = 0; row < (unsigned int)h; row++) {
		TIFFWriteScanline(out, &pixels[(h - row - 1) * w], row);
	}

	TIFFClose(out);
}