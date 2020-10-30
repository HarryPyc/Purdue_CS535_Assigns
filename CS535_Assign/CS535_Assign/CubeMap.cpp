#include "CubeMap.h"
#include <omp.h>

CubeMap::CubeMap()
{
	cube = obj::loadModelFromFile("asset/model/skybox.obj");
	texture = new Texture("asset/texture/cubemap.png");
}

CubeMap::~CubeMap() {
	delete texture;
}
bool RayTriangleIntersection(fvec4 o, fvec4 d, fvec4 v0, fvec4 v1, fvec4 v2, float& t, fvec4& uvw) {
	const float EPSILON = 1e-6;
	fvec4 e1, e2, h, s, q;
	float a, f, u, v;
	e1 = v1 - v0;
	e2 = v2 - v0;
	h = Cross(d, e2);
	a = Dot(e1, h);
	if (a > -EPSILON && a < EPSILON)
		return false;
	f = 1.0f / a;
	s = o - v0;
	u = f * Dot(s, h);
	if (u < 0.f || u > 1.0f)
		return false;
	q = Cross(s, e1);
	v = f * Dot(d, q);
	if (v < 0.f || u + v>1.0f)
		return false;
	t = f * Dot(e2, q);
	if (t > EPSILON) {
		uvw = fvec4(1.f - u - v, u, v, 0.f);
		return true;
	}
	else
		return false;
}

fvec4 CubeMap::Fetch(fvec4 o, fvec4 d)
{
	float t, u = 0.f, v = 0.f;
	fvec4 uvw;
	for (int i = 0; i < 12; i++) {
		unsigned short i0 = cube.faces["default"][3 * i + 0], i1 = cube.faces["default"][3 * i + 1], i2 = cube.faces["default"][3 * i + 2];
		fvec4 v0(cube.vertex[3 * i0], cube.vertex[3 * i0 + 1], cube.vertex[3 * i0 + 2], 1.f);
		fvec4 v1(cube.vertex[3 * i1], cube.vertex[3 * i1 + 1], cube.vertex[3 * i1 + 2], 1.f);
		fvec4 v2(cube.vertex[3 * i2], cube.vertex[3 * i2 + 1], cube.vertex[3 * i2 + 2], 1.f);
		if (RayTriangleIntersection(o, d, v0, v1, v2, t, uvw)) {
			u = cube.texCoord[2 * i0] * uvw[0] + cube.texCoord[2 * i1] * uvw[1] + cube.texCoord[2 * i2] * uvw[2];
			v = cube.texCoord[2 * i0 + 1] * uvw[0] + cube.texCoord[2 * i1 + 1] * uvw[1] + cube.texCoord[2 * i2 + 1] * uvw[2];
			break;
		}
	}

	return texture->Fetch(u,v);
}

CubeMap skyBox;