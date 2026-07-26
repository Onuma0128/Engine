#include "Vector4.h"

#include "Vector4.h"

#include <algorithm>

#include "Matrix4x4.h"

NumaEngine::Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

const NumaEngine::Vector4 NumaEngine::Vector4::ExprUnitXYZW = { 1.0f,1.0f,1.0f,1.0f };
const NumaEngine::Vector4 NumaEngine::Vector4::ExprUnitX = { 1.0f,0.0f,0.0f,0.0f };
const NumaEngine::Vector4 NumaEngine::Vector4::ExprUnitY = { 0.0f,1.0f,0.0f,0.0f };
const NumaEngine::Vector4 NumaEngine::Vector4::ExprUnitZ = { 0.0f,0.0f,1.0f,0.0f };
const NumaEngine::Vector4 NumaEngine::Vector4::ExprUnitW = { 0.0f,0.0f,0.0f,1.0f };
const NumaEngine::Vector4 NumaEngine::Vector4::ExprZero = { 0.0f,0.0f,0.0f,0.0f };

NumaEngine::Vector4 NumaEngine::Vector4::Lerp(const NumaEngine::Vector4& v1, const NumaEngine::Vector4& v2, float t)
{
	t = std::clamp(t, 0.0f, 1.0f);
	return NumaEngine::Vector4(
		v1.x + (v2.x - v1.x) * t,
		v1.y + (v2.y - v1.y) * t,
		v1.z + (v2.z - v1.z) * t,
		v1.w + (v2.w - v1.w) * t
	);
}

NumaEngine::Vector4 NumaEngine::Vector4::operator-() const
{
	return { -x,-y,-z,-w };
}

NumaEngine::Vector4 NumaEngine::Vector4::operator+(const NumaEngine::Vector4& v) const
{
	return { x + v.x, y + v.y, z + v.z, w + v.w };
}

NumaEngine::Vector4 NumaEngine::Vector4::operator-(const NumaEngine::Vector4& v) const
{
	return { x - v.x, y - v.y, z - v.z, w - v.w };
}

NumaEngine::Vector4 NumaEngine::Vector4::operator*(float scalar) const
{
	return { x * scalar, y * scalar, z * scalar, w * scalar };
}

NumaEngine::Vector4& NumaEngine::Vector4::operator+=(const NumaEngine::Vector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

NumaEngine::Vector4& NumaEngine::Vector4::operator-=(const NumaEngine::Vector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

NumaEngine::Vector4& NumaEngine::Vector4::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}

NumaEngine::Vector4 operator*(const NumaEngine::Vector4& v, const NumaEngine::Matrix4x4& m) {
	NumaEngine::Vector4 r;
	r.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0];
	r.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1];
	r.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2];
	r.w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3];
	return r;
}



