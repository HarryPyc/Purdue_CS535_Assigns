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
	//Transform the vertices
	Mat4 M = T * R * S;
	if (vertices.size() == 0) {
		for (int i = 0; i < data.vertex.size()/3; i ++) {
			Vertex vert;
			vert.p = M * fvec4(data.vertex[3*i], data.vertex[3*i + 1], data.vertex[3*i + 2], 1.f);
			UploadAABB(vert.p);
			vert.n = Normalize(Transpose(Inverse(M)) * fvec4(data.normal[3*i], data.vertex[3*i + 1], data.vertex[3*i + 2], 1.f));
			if (data.texCoord.size() > 0)
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
			UploadAABB(vertices[i].p);
			vertices[i].n = Normalize(Transpose(Inverse(M)) * fvec4(data.normal[3 * i], data.vertex[3 * i + 1], data.vertex[3 * i + 2], 1.f));
		}
	}
}

void Mesh::UploadAABB(fvec4 v)
{
	for (int i = 0; i < 3; i++) {
		minAABB[i] = min(minAABB[i], v[i]);
		maxAABB[i] = max(minAABB[i], v[i]);
	}
}

void Mesh::TranslateMesh(fvec4 t)
{
	T = Translate(t);
	UploadVertex();
}

void Mesh::ScaleMesh(fvec4 s)
{
	S = Scale(s);
	UploadVertex();
}

void Mesh::SetCenterAndScale(fvec4 center, fvec4 s)
{
	center.w = 0.f;
	T = Translate(center);
	S = Scale(s);
	UploadVertex();
}


Mesh::Mesh()
{
	R = Rotate(0.f, fvec4(1.f));
	T = Translate(fvec4(0.f));
	S = Scale(fvec4(1.f));
	minAABB.w = 1.f, maxAABB.w = 1.f;
}


void Mesh::LoadObj(const string& path)
{
	data = obj::loadModelFromFile(path);
}

void Mesh::LoadBin(char* fname) {

	ifstream ifs(fname, ios::binary);
	if (ifs.fail()) {
		cerr << "INFO: cannot open file: " << fname << endl;
		return;
	}
	int vertsN = 0;
	fvec4* verts, * colors, * normals;
	ifs.read((char*)&vertsN, sizeof(int));
	char yn;
	ifs.read(&yn, 1); // always xyz
	if (yn != 'y') {
		cerr << "INTERNAL ERROR: there should always be vertex xyz data" << endl;
		return;
	}
	if (verts)
		delete verts;
	verts = new fvec4[vertsN];

	ifs.read(&yn, 1); // cols 3 floats
	if (colors)
		delete colors;
	colors = 0;
	if (yn == 'y') {
		colors = new fvec4[vertsN];
	}

	ifs.read(&yn, 1); // normals 3 floats
	if (normals)
		delete normals;
	normals = 0;
	if (yn == 'y') {
		normals = new fvec4[vertsN];
	}

	ifs.read(&yn, 1); // texture coordinates 2 floats
	float* tcs = 0; // don't have texture coordinates for now
	if (tcs)
		delete tcs;
	tcs = 0;
	if (yn == 'y') {
		tcs = new float[vertsN * 2];
	}

	ifs.read((char*)verts, vertsN * 3 * sizeof(float)); // load verts

	if (colors) {
		ifs.read((char*)colors, vertsN * 3 * sizeof(float)); // load cols
	}

	if (normals)
		ifs.read((char*)normals, vertsN * 3 * sizeof(float)); // load normals

	if (tcs)
		ifs.read((char*)tcs, vertsN * 2 * sizeof(float)); // load texture coordinates

	ifs.read((char*)&trisN, sizeof(int));
	if (indices)
		delete indices;
	indices = new unsigned int[trisN * 3];
	ifs.read((char*)indices, trisN * 3 * sizeof(unsigned int)); // read tiangles

	ifs.close();

	for (int i = 0; i < vertsN; i++) {
		Vertex vert;
		vert.p = verts[i];
		if (colors)
			vert.c = colors[i];
		if (tcs)
			vert.u = tcs[2 * i], vert.v = tcs[2 * i + 1];
		if (normals)
			vert.n = normals[i];
		vertices.push_back(vert);
	}

	cerr << "INFO: loaded " << vertsN << " verts, " << trisN << " tris from " << endl << "      " << fname << endl;
	cerr << "      xyz " << ((colors) ? "rgb " : "") << ((normals) ? "nxnynz " : "") << ((tcs) ? "tcstct " : "") << endl;

}


