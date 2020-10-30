#include "FrameBuffer.h"
#include "Scene.h"
#include "Mat4.h"
#include "CubeMap.h"
#include <omp.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>
extern CubeMap skyBox;
extern Scene* MainScene;
bool ALPHA_BLEND = false;

FrameBuffer::FrameBuffer(int _width, int _height, Camera *_cam, bool isDB) 
{
	screen = new Texture(_width, _height, 4);
	zBuffer = new float[_width * _height];
	cam = _cam;
	isDepthBuffer = isDB;
	Clear(fvec4(0.f, 0.f, 0.f, 0.f));
	ClearZBuffer();
}

FrameBuffer::FrameBuffer(Texture* project, Camera* cam)
{
	screen = project;
	zBuffer = new float[screen->w * screen->h];
	this->cam = cam;
	isDepthBuffer = true;
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

void FrameBuffer::Clear()
{
	fvec4 ray00 = fvec4(-1, -1, 0, 1) * cam->InversePV;
	fvec4 ray01 = fvec4(-1,  1, 0, 1) * cam->InversePV;
	fvec4 ray10 = fvec4( 1, -1, 0, 1) * cam->InversePV;
	fvec4 ray11 = fvec4( 1,  1, 0, 1) * cam->InversePV;
	ray00 = Normalize(ray00 / ray00.w - cam->pos);
	ray01 = Normalize(ray01 / ray01.w - cam->pos);
	ray10 = Normalize(ray10 / ray10.w - cam->pos);
	ray11 = Normalize(ray11 / ray11.w - cam->pos);
#ifdef MULTI_PROCESS
#pragma omp parallel for
#endif 
	for(int i = 0; i < screen->w; i++)
		for (int j = 0; j < screen->h; j++) {
			float u = float(i) / float(screen->w), v = float(j) / float(screen->h);
			fvec4 d = (ray00 * (1.f - u) + ray10 * u) * (1.f - v) + (ray01 * (1.f - u) + ray11 * u) * v;
			d = Normalize(d);
			fvec4 c = skyBox.Fetch(cam->pos, d);
			screen->SetPixel(i, j, c);
		}
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

inline float FrameBuffer::VisibilityTest(fvec4 worldPos)
{
	fvec4 screenPos = cam->Projection(worldPos);
	screenPos = PerspectiveDevide(screenPos);
	const int x = screenPos.x, y = screenPos.y;
	if(x < 0 || y < 0 || x >= screen->w || y >= screen->h)
		return 0.0f;
	const unsigned int index = y * screen->w + x;
	if (screenPos.z - zBuffer[index] <= 1e-2)
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
	if (DepthTest(x, y, pix.z)) {
		const float alpha = color.a;
		if (alpha < 1.0f && ALPHA_BLEND) {
			color = screen->GetPixel(x, y) * (1 - alpha) + color * alpha;
		}
		screen->SetPixel(x, y, color);
	}
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

void FrameBuffer::DrawTriangles(fvec4 v0, fvec4 v1, fvec4 v2, Vertex vw0, Vertex vw1, Vertex vw2, Texture* tex,  uint mode, bool light, FrameBuffer* projector)
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
		maxXY[0] = round(min(max(max(v0[0], v1[0]), v2[0]), float(screen->w)-1.f));
		maxXY[1] = round(min(max(max(v0[1], v1[1]), v2[1]), float(screen->h)-1.f));
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
					p.w = 1.f / (1.f/v0.w * w0 + 1.f/v1.w * w1 + 1.f/v2.w * w2);

					fvec4 worldPos = cam->InverseProjection(p, screen->w, screen->h);
					fvec4 uvw = WorldSpaceInterpolation(vw0.p, vw1.p, vw2.p, worldPos);
					fvec4 c;
					if (isDepthBuffer) {
						DepthTest(p.x, p.y, p.z);
						continue;
					}
					if (tex != nullptr) {
						float u = vw0.u * uvw[0] + vw1.u * uvw[1] + vw2.u * uvw[2];
						float v = vw0.v * uvw[0] + vw1.v * uvw[1] + vw2.v * uvw[2];
						c = tex->Fetch(u, v);
					}else
						c = vw0.c * uvw[0] + vw1.c * uvw[1] + vw2.c * uvw[2];
					fvec4 n = vw0.n * uvw[0] + vw1.n * uvw[1] + vw2.n * uvw[2];
					n = Normalize(n);
					//Specular
					fvec4 v = Normalize(cam->pos - worldPos);
					fvec4 r = n * Dot(n, v) * 2.f - v;
					r = Normalize(r);
					c = skyBox.Fetch(worldPos, r);
					//Projective Texture Mapping
					if (projector != NULL) {
						if (projector->VisibilityTest(worldPos) == 1.f) {
							fvec4 proPos = projector->cam->Projection(worldPos);
							float u = (proPos.x / proPos.w + 1.f) / 2.f, v = (proPos.y / proPos.w + 1.f) / 2.f;
							if(u >= 0.f && u < 1.f && v >= 0.f && v < 1.f)
								c =  projector->screen->Fetch(u, v);
						}
					}
					//Per fragment lighting
					if (light) {
						
						fvec4 lightColor;
						//shadow
						
						for (int i = 0; i < MainScene->lightList.size(); i++) {
							float shadow;
							if (MainScene->lightList[i].shadowMap != NULL) {
								shadow = MainScene->lightList[i].shadowMap->VisibilityTest(worldPos);
							}
							else
								shadow = 1.0f;
							lightColor = lightColor + MainScene->lightList[i].PhongLighting(worldPos, n, shadow, 0.3f, 1.0f, 1.0f, 10.0f, cam->pos);
							lightColor.a = 1.f;
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
	p.x = (p.x / p.w + 1.f) / 2.f * screen->w;
	p.y = (p.y / p.w + 1.f) / 2.f * screen->h;
	return p;
}

void FrameBuffer::DrawMesh( Mesh* mesh, uint mode, FrameBuffer *projector)
{
	const int n = mesh->GetIndexSize();
	Mat4 PV = cam->P * cam->V;
#ifdef MULTI_PROCESS
#pragma omp parallel for
#endif 
	for (int i = 0; i < n; i+=3) {
		Vertex v0 = mesh->vertices[mesh->GetIndex(i)];
		Vertex v1 = mesh->vertices[mesh->GetIndex(i+1)];
		Vertex v2 = mesh->vertices[mesh->GetIndex(i+2)];
		//Screen space
		fvec4 vs0 = PV * v0.p, vs1 = PV * v1.p, vs2 = PV * v2.p;

			//Transform and Projection
		vs0 = PerspectiveDevide(vs0);
		vs1 = PerspectiveDevide(vs1);
		vs2 = PerspectiveDevide(vs2);
		DrawTriangles(vs0, vs1, vs2, v0, v1, v2, mesh->texture, mode, mesh->enableLight, projector);
		
	}
}







