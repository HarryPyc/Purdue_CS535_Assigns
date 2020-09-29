#pragma once
#include "Mat4.h"
#include <vector>
#include <iostream>
#include "ObjLoad.h"
#include "Define.h"
#include "Texture.h"

using namespace std;
struct Material {
	fvec4 color;
	fvec4 emission;
};
struct Vertex {
	fvec4 p, n;
	float u,v;
	fvec4 c;
};

class Mesh
{
public:
	uint GetIndexSize();
	uint GetIndex(int i);
	void UploadVertex();
	Material material;
	Texture* texture;
	Mat4 R, T, S;
	fvec4 minAABB, maxAABB;
	vector<Vertex> vertices;

	void LoadObj(const string& path);
	Mesh();
	~Mesh() { delete texture; }
private:
	obj::Model data;

};

