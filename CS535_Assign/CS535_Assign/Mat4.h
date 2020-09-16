#pragma once
#include "Vec4.h"


class Mat4
{
public:
	fvec4 rows[4]; 

	Mat4() {}
	Mat4(fvec4 v1, fvec4 v2, fvec4 v3, fvec4 v4);

	fvec4& operator[](int i);

	fvec4 operator*(fvec4 v);
	Mat4 operator*(Mat4 m2);
	Mat4 operator*(const float& scalar);
	friend std::ostream& operator<<(std::ostream& ostr, Mat4 m) {
		return ostr << m[0] << std::endl << m[1] << std::endl << m[2] << std::endl << m[3] << std::endl;
	}

	fvec4 GetColumn(int i);
	void SetColumn(int i, fvec4 col);
	float Determinant();
	


};

 Mat4 Transpose(Mat4 m);
 Mat4 Inverse(Mat4 m);
 Mat4 Perspective(float fovy, float aspect, float zNear, float zFar);
 Mat4 LookAt(fvec4 eye, fvec4 center, fvec4 up);
 Mat4 Translate(fvec4 v);
 Mat4 Scale(fvec4 s);
 Mat4 Rotate(float angle, fvec4 axis);