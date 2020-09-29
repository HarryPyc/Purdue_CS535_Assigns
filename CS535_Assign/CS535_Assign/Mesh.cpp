#include "Mesh.h"

uint Mesh::GetIndexSize()
{
	return data.faces["default"].size();
}

uint Mesh::GetIndex(int i)
{
	return data.faces["default"][i];
}

void Mesh::UploadVertex()
{
	Mat4 M = T * R * S;
	if (vertices.size() == 0) {
		for (int i = 0; i < data.vertex.size()/3; i ++) {
			Vertex vert;
			vert.p = M * fvec4(data.vertex[3*i], data.vertex[3*i + 1], data.vertex[3*i + 2], 1.f);
			vert.n = Normalize(Transpose(Inverse(M)) * fvec4(data.normal[3*i], data.vertex[3*i + 1], data.vertex[3*i + 2], 1.f));
			if(data.texCoord.size()>0)
				vert.u = data.texCoord[2 * i], vert.v = data.texCoord[2 * i + 1];
			vert.c = material.color;
			vertices.push_back(vert);
		}
	}
	else{
#ifdef  MULTI_PROCESS
#pragma omp parallel for
#endif //  MULTI_PROCESS
		for (int i = 0; i < vertices.size(); i++) {
			vertices[i].p = M * fvec4(data.vertex[3 * i], data.vertex[3 * i + 1], data.vertex[3 * i + 2], 1.f);
			vertices[i].n = Normalize(Transpose(Inverse(M)) * fvec4(data.normal[3 * i], data.vertex[3 * i + 1], data.vertex[3 * i + 2], 1.f));
		}
	}
}


Mesh::Mesh()
{
	R = Rotate(0.f, fvec4(0.f));
	T = Translate(fvec4(0.f));
	S = Scale(fvec4(1.f));
	texture = 0;
}


void Mesh::LoadObj(const string& path)
{
	data = obj::loadModelFromFile(path);
}


