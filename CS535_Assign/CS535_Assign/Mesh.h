#pragma once
#include "Mat4.h"
#include <vector>
#include <iostream>
#include "ObjLoad.h"
#include "Define.h"

using namespace std;
struct Material {
	fvec4 color;
	fvec4 emission;
};
struct Vertex {
	fvec4 p, n;
	float u,v;
};

class Mesh
{
public:
	uint GetIndexSize();
	uint GetIndex(int i);
	void UploadVertex();
	Material material;
	Mat4 R, T, S;
	fvec4 minAABB, maxAABB;
	vector<Vertex> vertices;

	void LoadObj(const string& path);
	Mesh();
	~Mesh() {}
private:
	obj::Model data;

};

