#include "Mesh.h"

void Mesh::AddVertex(fvec4 vert)
{
	Vertices.push_back(vert);
}

void Mesh::CopyVertices(Mesh m)
{
	this->Vertices = m.Vertices;
}

fvec4 Mesh::GetVertex(int i)
{
	if (i >= Vertices.size()) {
		cout << "Index out of range of Vertices" << endl;
		return fvec4();
	}
	
	return Vertices[i];
}

int Mesh::GetSize()
{
	return Vertices.size();
}

Mesh::Mesh()
{
	R = Rotate(0.f, fvec4(0.f));
	T = Translate(fvec4(0.f));
	S = Scale(fvec4(1.f));
}
