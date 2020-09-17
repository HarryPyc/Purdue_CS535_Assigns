#include "Mat4.h"

Mat4::Mat4(float val)
{
	fvec4 row = fvec4(val);
	for (int i = 0; i < 4; i++) {
		rows[i] = row;
	}
}

Mat4::Mat4(fvec4 v1, fvec4 v2, fvec4 v3, fvec4 v4)
{
	rows[0] = v1, rows[1] = v2, rows[2] = v3, rows[3] = v4;

}

fvec4& Mat4::operator[](int i)
{
	return rows[i];
}

fvec4 Mat4::operator*( fvec4 v)
{
	return fvec4(Dot(rows[0], v), Dot(rows[1], v), Dot(rows[2], v), Dot(rows[3], v));
}

 Mat4 Mat4::operator*(Mat4 m2)
{
	Mat4 m1 = *this;
	fvec4 _rows[4];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			_rows[i][j] = Dot(m1[i], m2.GetColumn(j));
	return Mat4(_rows[0], _rows[1], _rows[2], _rows[3]);
}

Mat4 Mat4::operator*(const float& scalar)
{
	Mat4& m = *this;
	for (int i = 0; i < 4; i++)
		m[i] = m[i] * scalar;
	return m;
}

fvec4 Mat4::GetColumn(int i)
{
	return fvec4(rows[0][i], rows[1][i], rows[2][i], rows[3][i]);
}

void Mat4::SetColumn(int i, fvec4 col)
{
	for (int r = 0; r < 4; r++)
		rows[r][i] = col[r];
}

float GetCofactor(Mat4 m, int x, int y, int n);
float Det(Mat4 m, int n) {
	float det = 0;
	if (n == 2)
		return m[0][0] * m[1][1] - m[1][0] * m[0][1];
	else {
		for (int c = 0; c < n; c++)
			det += m[0][c] * GetCofactor(m, 0, c, n);
		return det;
	}
}
float GetCofactor(Mat4 m, int x, int y, int n) {
	if (n == 2)
		return Det(m, n);
	else {
		Mat4 subM; int r = 0, c = 0;
		for (int i = 0 ; i < n; i++) {
			if (r == x) i++;
			for (int j = 0; j < n; j++) {
				if (c == y) j++;
				subM[r][c++] = m[i][j];
			}
			r++;
			c = 0;
		}
		return pow(-1, x+y) * Det(subM, n - 1);
	}
}
float Mat4::Determinant()
{
	return Det(*this, 4);
}


Mat4 Transpose(Mat4 m)
{
	return Mat4(fvec4(m[0][0], m[1][0], m[2][0], m[3][0]),
				fvec4(m[0][1], m[1][1], m[2][1], m[3][1]),
				fvec4(m[0][2], m[1][2], m[2][2], m[3][2]),
				fvec4(m[0][3], m[1][3], m[2][3], m[3][3]));
}

Mat4 Inverse(Mat4 m)
{
	float det = Det(m, 4);
	if (det == 0) return Mat4();
	Mat4 C;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			C[i][j] = GetCofactor(m, i, j, 4);
	Mat4 adjM = Transpose(C);
	return adjM * (1.f/det);
}

//fovy is angle
Mat4 Perspective(float fovy, float aspect, float zNear, float zFar)
{
	float rad = 3.1415926f*fovy/180.f, tanHalfFovy = tan(rad / 2.f);
	Mat4 P(fvec4(1/(aspect*tanHalfFovy), 0.f, 0.f, 0.f),
			fvec4(0.f,1/tanHalfFovy, 0.f, 0.f),
			fvec4(0.f, 0.f, -(zNear+zFar)/(zFar-zNear), -(2 * zFar * zNear) / (zFar - zNear)),
			fvec4(0.f, 0.f, -1.f , 0.f));
	return P;
}

Mat4 LookAt(fvec4 eye, fvec4 center, fvec4 up)
{
	fvec4 f = Normalize(center - eye), s = Normalize(Cross(f, up)), u = Cross(s, f);
	s.w = -Dot(s, eye);
	u.w = -Dot(u, eye);
	f.w = -Dot(f, eye);
	Mat4 res;
	res[0] = s;
	res[1] = u;
	res[2] = f * (-1.f);
	res[3] = fvec4(1.f);
	return res;
}

Mat4 Translate(fvec4 v)
{
	Mat4 T;
	T.SetColumn(3, v);
	for (int i = 0; i < 4; i++)
		T[i][i] = 1.f;
	return T;
}

Mat4 Scale(fvec4 s)
{
	Mat4 S;
	S[0][0] = s[0], S[1][1] = s[1], S[2][2] = s[2], S[3][3] = 1.f;
	return S;
}

Mat4 Rotate(float angle, fvec4 axis)
{
	float rad = 3.1415926f * angle / 180.f;
	axis = Normalize(axis);
	float rcos = cos(rad), rsin = sin(rad), u = axis.x, v = axis.y, w = axis.z;
	Mat4 R;
	R[0][0] = rcos + u * u * (1 - rcos);
	R[1][0] = w * rsin + v * u * (1 - rcos);
	R[2][0] = -v * rsin + w * u * (1 - rcos);
	R[0][1] = -w * rsin + u * v * (1 - rcos);
	R[1][1] = rcos + v * v * (1 - rcos);
	R[2][1] = u * rsin + w * v * (1 - rcos);
	R[0][2] = v * rsin + u * w * (1 - rcos);
	R[1][2] = -u * rsin + v * w * (1 - rcos);
	R[2][2] = rcos + w * w * (1 - rcos);
	R[3][3] = 1.f;
	return R;
}
