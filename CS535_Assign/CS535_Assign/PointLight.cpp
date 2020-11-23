#include "PointLight.h"
#include <algorithm>
using namespace std;

inline float attenuation(float d) {
	return 1.f / (1 + 0.1 * d + d * d);
}
inline fvec4 reflect(fvec4 l, fvec4 n) {
	return Normalize(n * 2.f * Dot(n, l) - l);
}
PointLight::PointLight(fvec4 p, fvec4 c) : pos(p), color(c)
{
	shadowMap = NULL;
}
void PointLight::TranslateLight(fvec4 t)
{
	pos = Translate(t) * pos;
	if (shadowMap != NULL)
		shadowMap->cam->TranslateCamera(t);
}
void PointLight::UploadToDevice(GLuint shader)
{
	const int color_loc = glGetUniformLocation(shader, "light.color");
	if (color_loc != -1)
		glUniform3fv(color_loc, 1, &color[0]);
	const int pos_loc = glGetUniformLocation(shader, "light.pos");
	if (pos_loc != -1)
		glUniform3fv(pos_loc, 1, &pos[0]);
}
fvec4 PointLight::PhongLighting(fvec4 p, fvec4 n, float shadow, float Ka, float Kd, float Ks, float alpha, fvec4 camPos)
{
	fvec4 l = pos - p;
	const float att = attenuation(l.Length());
	l = Normalize(l);
	fvec4 r = reflect(l, n), v = Normalize(camPos - p);

	//Diffuse
	float diffFact = max(0.f, Dot(n, l)); 
	float specFact = pow(max(0.f, Dot(r, v)), alpha);

	return color * (Ka + shadow * att * (Kd * diffFact + Ks * specFact));
}
