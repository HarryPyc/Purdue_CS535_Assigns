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
	void UploadAABB(fvec4 v);
	void TranslateMesh(fvec4 t);
	void ScaleMesh(fvec4 s);
	void SetCenterAndScale(fvec4 center, fvec4 s);

	Material material;
	Texture* texture;
	Mat4 R, T, S;
	fvec4 minAABB, maxAABB;
	unsigned int *indices;
	int trisN;
	vector<Vertex> vertices;

	void LoadObj(const string& path);
	void LoadBin(char* fname);
	Mesh();
	~Mesh() { delete texture; }
private:
	obj::Model data;
	fvec4 centroid;

};

