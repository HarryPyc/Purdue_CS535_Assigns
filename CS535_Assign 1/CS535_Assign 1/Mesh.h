#pragma once
#include "Mat4.h"
#include <vector>
#include <iostream>
using namespace std;
struct Material {
	fvec4 color;
	fvec4 emission;
};

class Mesh
{
public:
	void AddVertex(fvec4 vert);
	void CopyVertices(Mesh m);
	fvec4 GetVertex(int i);
	int GetSize();
	Material material;
	Mat4 R, T, S;

	Mesh();
	~Mesh() {}
private:
	vector<fvec4> Vertices;

};

