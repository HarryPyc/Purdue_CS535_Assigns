#include "Mesh.h"

Vertex Mesh::GetVertex(int i)
{
	if (i >= data.vertex.size()/3) {
		cout << "Index out of range of Vertices" << endl;
		return Vertex();
	}
	Vertex vert;
	vert.p = fvec4(data.vertex[i * 3], data.vertex[i * 3 + 1], data.vertex[i * 3 + 2], 1.f);
	vert.n = fvec4(data.normal[i * 3], data.normal[i * 3 + 1], data.normal[i * 3 + 2], 0.f);
	//vert.u = TexCoords[i][0]; vert.v = TexCoords[i][1];
	vert.color = material.color; vert.emission = material.emission;
	return vert;
}

uint Mesh::GetIndexSize()
{
	return data.faces["default"].size();
}

uint Mesh::GetIndex(int i)
{
	return data.faces["default"][i];
}


Mesh::Mesh()
{
	R = Rotate(0.f, fvec4(0.f));
	T = Translate(fvec4(0.f));
	S = Scale(fvec4(1.f));
}


void Mesh::LoadObj(const string& path)
{
	data = obj::loadModelFromFile(path);
}


