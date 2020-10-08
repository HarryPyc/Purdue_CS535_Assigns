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
