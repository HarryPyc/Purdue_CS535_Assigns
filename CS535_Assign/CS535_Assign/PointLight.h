#pragma once
#include "Vec4.h"
#include "FrameBuffer.h"


class PointLight
{
public:
	fvec4 color, pos;
	FrameBuffer* shadowMap;

	PointLight(fvec4 p, fvec4 c);
	~PointLight() { if (shadowMap != nullptr) delete shadowMap; }

	void TranslateLight(fvec4 t);
	void UploadToDevice(GLuint shader);
	fvec4 PhongLighting(fvec4 p, fvec4 n, float shadow, float Ka, float Kd, float Ks, float alpha, fvec4 camPos);

};

