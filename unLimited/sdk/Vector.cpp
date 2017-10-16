#include "Vector.hpp"

void VectorCopy(const Vector& src, Vector& dst)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}
void VectorLerp(const Vector& src1, const Vector& src2, vec_t t, Vector& dest)
{
	dest.x = src1.x + (src2.x - src1.x) * t;
	dest.y = src1.y + (src2.y - src1.y) * t;
	dest.z = src1.z + (src2.z - src1.z) * t;
}
float VectorLength(const Vector& v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}
void VectorCrossProduct(const Vector& a, const Vector& b, Vector& result)
{
	result.x = a.y*b.z - a.z*b.y;
	result.y = a.z*b.x - a.x*b.z;
	result.z = a.x*b.y - a.y*b.x;
}
vec_t NormalizeVector(Vector& v)
{
	vec_t l = v.Length();
	if (l != 0.0f) {
		v /= l;
	}
	else {
		v.x = v.y = v.z = 0.0f;
	}
	return l;
}
