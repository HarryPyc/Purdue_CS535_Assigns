#include "FrameBuffer.h"
#include <omp.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>


FrameBuffer::FrameBuffer(int _width, int _height, Camera *_cam) 
{
	screen = new Texture(_width, _height, 4);
	zBuffer = new float[_width * _height];
	cam = _cam;
	Clear(fvec4(0.f, 0.f, 0.f, 0.f));
	ClearZBuffer();
}

FrameBuffer::~FrameBuffer()
{
	if (zBuffer != nullptr)
		delete[] zBuffer;
	if (screen != nullptr)
		delete screen;
}


void FrameBuffer::Clear(fvec4 color)
{
	screen->Clear(color);
}

void FrameBuffer::ClearZBuffer()
{
#ifdef MULTI_PROCESS
#pragma omp parallel for
#endif 
	for (int i = 0; i < screen->w * screen->h; i++)
		zBuffer[i] = 999.f;
}

inline bool FrameBuffer::DepthTest(int x, int y, float z)
{
	if (z < zBuffer[y *screen->w + x]) {
		zBuffer[y * screen->w + x] = z;
		return true;
	}
	return false;
}

inline float FrameBuffer::ShadowTest(fvec4 worldPos)
{
	fvec4 screenPos = cam->Projection(worldPos);
	screenPos = PerspectiveDevide(screenPos);
	const int x = screenPos.x, y = screenPos.y;
	if(x < 0 || y < 0 || x >= screen->w || y >= screen->h)
		return 1.0f;
	const unsigned int index = y * screen->w + x;
	if (abs(screenPos.z - zBuffer[index]) <= 1e-2)
		return 1.f;
	else
		return 0.f;
}

void FrameBuffer::SetPixel(fvec4 pix, fvec4 color)
{
	int x = pix.x, y = pix.y;
	if (x < 0 || y < 0 || x >= screen->w || y >= screen->h) {
		//cout << "Index out of range of Pixels" << endl;
		return;
	}
	if(DepthTest(x,y,pix.z))
		screen->SetPixel(x, y, color);
}



void FrameBuffer::Draw2DSegements(fvec4 a, fvec4 b, Vertex vw0, Vertex vw1)
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
		fvec4 color = (vw0.c*i+vw1.c*(steps-i)) / float(steps);
		fvec4 pix(x, y, z, 1.f);
		SetPixel(pix, color);
	}

}
//return true if point p is in the right side of edge ab
inline float EdgeFunction(fvec4 a, fvec4 b, fvec4 p) {
	return (p[0] - b[0]) * (a[1] - b[1]) - (p[1] - b[1]) * (a[0] - b[0]);
}

inline fvec4 WorldSpaceInterpolation(fvec4 A, fvec4 B, fvec4 C, fvec4 P) {
	fvec4 AB = B - A, AC = C - A, AP = P - A, N = Cross(AB, AC);
	float DotNN = Dot(N, N);
	fvec4 uvw;
	uvw[1] = Dot(Cross(AP, AC), N) / DotNN;
	uvw[2] = Dot(Cross(AB, AP), N) / DotNN;
	uvw[0] = 1.f - uvw[1] - uvw[2];
	return uvw;
}

void FrameBuffer::DrawTriangles(fvec4 v0, fvec4 v1, fvec4 v2, Vertex vw0, Vertex vw1, Vertex vw2, Texture* tex,  uint mode, bool light, FrameBuffer* shadowMap)
{
	if (mode == DRAW_LINES) {
		Draw2DSegements(v0, v1, vw0, vw1);
		Draw2DSegements(v1, v2, vw1, vw2);
		Draw2DSegements(v2, v0, vw2, vw0);
	}
	else if (mode == DRAW_FILL) {
		float* minXY = new float[2], * maxXY = new float[2];
		//Clip
		minXY[0] = round(max(min(min(v0[0], v1[0]), v2[0]), 0.f));
		minXY[1] = round(max(min(min(v0[1], v1[1]), v2[1]), 0.f));
		maxXY[0] = round(min(max(max(v0[0], v1[0]), v2[0]), float(screen->w)));
		maxXY[1] = round(min(max(max(v0[1], v1[1]), v2[1]), float(screen->h)));
		fvec4 p;

		for (p[1] = minXY[1]; p[1] <= maxXY[1]; p[1]++) {
			for (p[0] = minXY[0]; p[0] <= maxXY[0]; p[0]++) {
				float w0 = EdgeFunction(v1, v2, p);
				float w1 = EdgeFunction(v2, v0, p);
				float w2 = EdgeFunction(v0, v1, p);

				if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
					float area = EdgeFunction(v0, v1, v2);
					w0 /= area; w1 /= area; w2 /= area;
					p.z = v0.z * w0 + v1.z * w1 + v2.z * w2;
					p.w = v0.w * w0 + v1.w * w1 + v2.w * w2;

					fvec4 worldPos = cam->InverseProjection(p, screen->w, screen->h);
					fvec4 uvw = WorldSpaceInterpolation(vw0.p, vw1.p, vw2.p, worldPos);
					fvec4 c;
					if (tex != nullptr) {
						float u = vw0.u * uvw[0] + vw1.u * uvw[1] + vw2.u * uvw[2];
						float v = vw0.v * uvw[0] + vw1.v * uvw[1] + vw2.v * uvw[2];
						c = tex->Fetch(u, v);
					}else
						c = vw0.c * uvw[0] + vw1.c * uvw[1] + vw2.c * uvw[2];
					//Per fragment lighting
					if (light) {
						fvec4 n = vw0.n * uvw[0] + vw1.n * uvw[1] + vw2.n * uvw[2];
						fvec4 lightColor;
						//shadow
						float shadow;
						if (shadowMap != NULL) {
							shadow = shadowMap->ShadowTest(worldPos);
						}
						else
							shadow = 1.0f;
						for (int i = 0; i < MainScene->lightList.size(); i++) {
							lightColor = lightColor + MainScene->lightList[i].PhongLighting(worldPos, n, shadow, 0.1f, 2.0f, 2.0f, 10.0f, cam->pos);
						}
						SetPixel(p, c * lightColor);
					}else
						SetPixel(p, c );
				}
			}
		}
		delete[] minXY, delete[] maxXY;
	}
}

inline bool ClipTest(fvec4 p) {
	const float s = 10.f;
	return (p.x <= s*p.w && p.x >= -s*p.w) && (p.y <= s*p.w && p.y >= -s*p.w) && (p.z <= s*p.w && p.z >= -s*p.w);
}
inline fvec4 FrameBuffer::PerspectiveDevide(fvec4 p)
{
	//Perspective Division
	p.z = p.z / p.w;
	//p.z = log(p.z);
	p.x = (p.x / p.w + 1.f) / 2.f * screen->w;
	p.y = (p.y / p.w + 1.f) / 2.f * screen->h;
	return p;
}

void FrameBuffer::DrawMesh( Mesh* mesh, FrameBuffer* shadowMap, uint mode)
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
		//Screen space
		fvec4 vs0 = PV * v0.p, vs1 = PV * v1.p, vs2 = PV * v2.p;
		if (ClipTest(vs0) && ClipTest(vs1) && ClipTest(vs2)) {
			//Transform and Projection
			vs0 = PerspectiveDevide(vs0);
			vs1 = PerspectiveDevide(vs1);
			vs2 = PerspectiveDevide(vs2);
			DrawTriangles(vs0, vs1, vs2, v0, v1, v2, mesh->texture, mode, mesh->enableLight, shadowMap);
		}
	}
}







