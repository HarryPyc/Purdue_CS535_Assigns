#include "FrameBuffer.h"
#include <libtiff/tiffio.h>
#include <omp.h>

FrameBuffer::FrameBuffer(int _width, int _height)
{
	w = _width; h = _height;
	pixels = new unsigned int[w * h];
	zBuffer = new float[w * h];
	Clear(fvec4(0.f, 0.f, 0.f, 0.f));
	ClearZBuffer();
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

void FrameBuffer::ClearZBuffer()
{
#ifdef MULTI_PROCESS
#pragma omp parallel for
#endif 
	for (int i = 0; i < w * h; i++)
		zBuffer[i] = 999.f;
}

inline bool FrameBuffer::DepthTest(int x, int y, float z)
{
	if (z < zBuffer[y * w + x]) {
		zBuffer[y * w + x] = z;
		return true;
	}
	return false;
}

void FrameBuffer::SetPixel(fvec4 pix, fvec4 color)
{
	int x = pix.x, y = pix.y;
	const int index = y * w + x;
	
	if (x < 0 || y < 0 || x >= w || y >= h) {
		//cout << "Index out of range of Pixels" << endl;
		return;
	}
	if(DepthTest(x,y,pix.z))
		*(pixels + index) = convVec4ToRGBA8(color);
}



void FrameBuffer::Draw2DSegements(fvec4 a, fvec4 b, fvec4 c1, fvec4 c2)
{
	float Dx = fabsf(a[0] - b[0]), Dy = fabsf(a[1] - b[1]);
	const int steps = int(Dx > Dy ? Dx : Dy) + 1;
#ifdef MULTI_PROCESS
#pragma omp parallel for
#endif 
	for (int i = 0; i <= steps; i++) {
		float x = a[0] + (b[0] - a[0]) / float(steps) * float(i);
		float y = a[1] + (b[1] - a[1]) / float(steps) * float(i);
		float z = a[2] + (b[2] - a[2]) / float(steps) * float(i);
		fvec4 color = (c1*i+c2*(steps-i)) / float(steps);
		fvec4 pix(x, y, z, 1.f);
		SetPixel(pix, color);
	}

}
//return true if point p is in the right side of edge ab
inline float EdgeFunction(fvec4 a, fvec4 b, fvec4 p) {
	return (p[0] - b[0]) * (a[1] - b[1]) - (p[1] - b[1]) * (a[0] - b[0]);
}

void FrameBuffer::DrawTriangles(fvec4 v0, fvec4 v1, fvec4 v2, fvec4* color, uint mode)
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
		fvec4 p;

		for (p[1] = minXY[1]; p[1] <= maxXY[1]; p[1]++) {
			for (p[0] = minXY[0]; p[0] <= maxXY[0]; p[0]++) {
				float w0 = EdgeFunction(v1, v2, p);
				float w1 = EdgeFunction(v2, v0, p);
				float w2 = EdgeFunction(v0, v1, p);

				if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
					float area = EdgeFunction(v0, v1, v2);
					w0 /= area; w1 /= area; w2 /= area;
					//Interpolate Color & Depth in Screen Space
					fvec4 c = color[0] * w0 + color[1] * w1 + color[2] * w2;
					p.z = v0.z * w0 + v1.z * w1 + v2.z * w2;
					SetPixel(p, c);
				}
			}
		}
		delete[] minXY, maxXY;
	}
}


void FrameBuffer::DrawMesh(Camera* cam, Mesh* mesh, uint mode)
{
	const int n = mesh->GetIndexSize();
	Mat4 PV = cam->P * cam->V;
	mesh->UploadVertex();
#ifdef MULTI_PROCESS
#pragma omp parallel for
#endif 
	for (int i = 0; i < n; i+=3) {
		Vertex v0 = mesh->vertices[mesh->GetIndex(i)];
		Vertex v1 = mesh->vertices[mesh->GetIndex(i+1)];
		Vertex v2 = mesh->vertices[mesh->GetIndex(i+2)];
		//Transform and Projection
		v0.p = PV * v0.p;
		v1.p = PV * v1.p;
		v2.p = PV * v2.p;
		fvec4 _v0 = ConvToScreenSpace(v0.p);
		fvec4 _v1 = ConvToScreenSpace(v1.p);
		fvec4 _v2 = ConvToScreenSpace(v2.p);
		fvec4* colors = new fvec4[3]{ v0.c, v1.c, v2.c };
		DrawTriangles(_v0, _v1, _v2, colors, mode);
	}
	glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}



fvec4 FrameBuffer::ConvToScreenSpace(fvec4 p)
{
	p = p / p.w;//Perspective Division
	//p.z = log(p.z);
	p.x = (p.x + 1.f) / 2.f * w;
	p.y = (p.y + 1.f) / 2.f * h;
	return p;
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