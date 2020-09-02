#pragma once
#include <math.h>
#include <ostream>
template<class T> class Vec4
{
public:
	union { T r, x; };
	union { T g, y; };
	union { T b, z; };
	union { T a, w; };

	Vec4(T _x, T _y, T _z, T _w) { x = _x, y = _y, z = _z, w = _w; }

	inline T& operator[](int i) { switch (i) { case 0:return x; case 1: return y; case 2:return z; case 3:return w; default:return x; } }

	inline Vec4 operator+(const Vec4& v2) { const Vec4& v1 = *this; return Vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w); }
	template<class U> inline Vec4 operator+( const U& scalar) { const Vec4& v1 = *this; return Vec4(v1.x + scalar, v1.y + scalar, v1.z + scalar, v1.w + scalar); }
	inline Vec4 operator*(const Vec4& v2) { const Vec4& v1 = *this; return Vec4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w); }
	template<class U> inline Vec4 operator*(const U& scalar) { const Vec4& v1 = *this; return Vec4(v1.x * scalar, v1.y * scalar, v1.z * scalar, v1.w * scalar); }
	inline Vec4 operator-(const Vec4& v2) { const Vec4& v1 = *this; return Vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w); }
	template<class U> inline Vec4 operator-(const U& scalar) { const Vec4& v1 = *this; return Vec4(v1.x - scalar, v1.y - scalar, v1.z - scalar, v1.w - scalar); }
	inline Vec4 operator/(const Vec4& v2) { const Vec4& v1 = *this; return Vec4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w); }
	template<class U> inline Vec4 operator/(const U& scalar) { const Vec4& v1 = *this; return Vec4(v1.x / scalar, v1.y / scalar, v1.z / scalar, v1.w / scalar); }

	friend inline std::ostream& operator<<(std::ostream& ostr, Vec4 v) { return ostr << '(' << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ')'; }
	friend inline T Dot(const Vec4& v1, const Vec4& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
	friend inline Vec4 Cross(const Vec4& v1, const Vec4& v2) { return Vec4(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x, T(0)); }
	friend inline Vec4 Normalize(Vec4 v) { return v / v.Length(); }

	T Length() { return sqrt(x * x + y * y + z * z + w * w); }
};



