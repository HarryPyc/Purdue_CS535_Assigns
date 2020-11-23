#pragma once
#include <GL/glew.h>
#include <string>
#include "Define.h"
#include "Vec4.h"


class Texture
{
public:
	Texture(std::string fname);
	Texture(int w, int h, int components);
	Texture();

	void Clear(fvec4 color);
	void SetPixel(unsigned int x, unsigned int y, fvec4 color);
	void Draw();
	void SaveAsBmp(const char* fname);
	void CreateDeviceTexture();
	void UploatToDevice(GLuint shader);
	inline fvec4 GetPixel(unsigned int x, unsigned int y);
	~Texture();

	fvec4 Fetch(float u, float v);
	int w, h, components;
private:
	unsigned int* pixels;
	GLuint tex_id;
};

