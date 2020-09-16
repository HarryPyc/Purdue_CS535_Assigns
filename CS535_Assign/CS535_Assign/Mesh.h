#pragma once
#include "Mat4.h"
#include <vector>
#include <iostream>
#include "ObjLoad.h"

using namespace std;
struct Material {
	fvec4 color;
	fvec4 emission;
};
struct Vertex {
	fvec4 p, n;
	float u,v;
	fvec4 color, emission;
};

class Mesh
{
public:
	uint GetIndexSize();
	uint GetIndex(int i);
	Vertex GetVertex(int i);
	Material material;
	Mat4 R, T, S;

	void LoadObj(const string& path);
	Mesh();
	~Mesh() {}
private:
	obj::Model data;

};

