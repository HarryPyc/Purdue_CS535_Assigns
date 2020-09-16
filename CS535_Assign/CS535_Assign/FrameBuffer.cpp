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



void FrameBuffer::Draw2DSegements(float* a, float* b, fvec4 c1, fvec4 c2)
{
	float Dx = fabsf(a[0] - b[0]), Dy = fabsf(a[1] - b[1]);
	const int steps = int(Dx > Dy ? Dx : Dy) + 1;
#ifdef MULTI_PROCESS
#pragma omp parallel for
#endif 
	for (int i = 0; i <= steps; i++) {
		float x = a[0] + (b[0] - a[0]) / float(steps) * float(i);
		float y = a[1] + (b[1] - a[1]) / float(steps) * float(i);
		fvec4 color = (c1*i+c2*(steps-i)) / float(steps);
		SetPixel(int(x), int(y), color);
	}

}
//return true if point p is in the right side of edge ab
inline float EdgeFunction(float* a, float* b, float* p) {
	return (p[0] - a[0]) * (b[1] - a[1]) - (p[1] - a[1]) * (b[0] - a[0]);
}

void FrameBuffer::DrawTriangles(float* v0, float* v1, float* v2, fvec4* color, uint mode)
{
	if (mode == DRAW_LINES) {
		Draw2DSegements(v0, v1, color[0], color[1]);
		Draw2DSegements(v1, v2, color[1], color[2]);
		Draw2DSegements(v0, v2, color[0], color[2]);
	}
	else if (mode == DRAW_FILL) {
		float* minXY = new float[2], * maxXY = new float[2];
		minXY[0] = round(max(min(min(v0[0], v1[0]), v2[0]), 0.f));
		minXY[1] = round(max(min(min(v0[1], v1[1]), v2[1]), 0.f));
		maxXY[0] = round(min(max(max(v0[0], v1[0]), v2[0]), float(w)));
		maxXY[1] = round(min(max(max(v0[1], v1[1]), v2[1]), float(h)));
		float* p = new float[2];

		for (p[1] = minXY[1]; p[1] < maxXY[1]; p[1]++) {
			for (p[0] = minXY[0]; p[0] < maxXY[0]; p[0]++) {
				float area = EdgeFunction(v0, v1, v2);
				float w0 = EdgeFunction(v1, v2, p);
				float w1 = EdgeFunction(v2, v0, p);
				float w2 = EdgeFunction(v0, v1, p);
				if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
					w0 /= area; w1 /= area; w2 /= area;
					fvec4 c = color[0] * w0 + color[1] * w1 + color[2] * w2;
					SetPixel(int(p[0]), int(p[1]), c);
				}
			}
		}
		delete[] minXY, maxXY, p;
	}
}

void FrameBuffer::DrawMesh(Camera* cam, Mesh* mesh, uint mode)
{
	const int n = mesh->GetIndexSize();

#ifdef MULTI_PROCESS
#pragma omp parallel for
#endif 
	for (int i = 0; i < n; i+=3) {
		Vertex v0 = mesh->GetVertex(mesh->GetIndex(i));
		Vertex v1 = mesh->GetVertex(mesh->GetIndex(i+1));
		Vertex v2 = mesh->GetVertex(mesh->GetIndex(i+2));
		//Transform and Projection
		v0.p = cam->P * cam->V * mesh->T * mesh->R * mesh->S * v0.p;
		v1.p = cam->P * cam->V * mesh->T * mesh->R * mesh->S * v1.p;
		v2.p = cam->P * cam->V * mesh->T * mesh->R * mesh->S * v2.p;
		float* _v0 = ConvToScreenSpace(v0.p);
		float* _v1 = ConvToScreenSpace(v1.p);
		float* _v2 = ConvToScreenSpace(v2.p);
		fvec4* colors = new fvec4[3]{ v0.color, v1.color, v2.color };
		DrawTriangles(_v0, _v1, _v2, colors, mode);
	}
	glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

float* FrameBuffer::ConvToScreenSpace(fvec4 p)
{
	float* v = new float[2];
	p = p / p.w;//Perspective Division
	p = (p + fvec4(1.f, 1.f, 0.f, 0.f)) / 2.f * fvec4(w, h, 1.f, 1.f);
	v[0] = p.x, v[1] = p.y;
	return v;
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