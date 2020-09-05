#include "Vec4.h"

fvec4 convRGBA8ToVec4(uint val) {
	fvec4 res = fvec4(float((val & 0x000000FF)), float((val & 0x0000FF00) >> 8U), float((val & 0x00FF0000) >> 16U), float((val & 0xFF000000) >> 24U));
	res.r /= 255.f; res.g /= 255.f; res.b /= 255.f;
	return res;
}

uint convVec4ToRGBA8(fvec4 val) {
	val.r *= 255.f; val.g *= 255.f; val.b *= 255.f;
	return (uint(val.w) & 0x000000FF) << 24U | (uint(val.z) & 0x000000FF) << 16U | (uint(val.y) & 0x000000FF) << 8U | (uint(val.x) & 0x000000FF);
}
