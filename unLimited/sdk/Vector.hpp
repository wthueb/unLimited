#pragma once

#include <sstream>

class Vector
{
public:
	Vector()
	{
		Invalidate();
	}
	Vector(float x, float y, float z)
	{
		x = x;
		y = y;
		z = z;
	}
	Vector(const float* clr)
	{
		x = clr[0];
		y = clr[1];
		z = clr[2];
	}
	void Init(float ix = 0.f, float iy = 0.f, float iz = 0.f)
	{
		x = ix;
		y = iy;
		z = iz;
	}

public:
	bool IsValid() const
	{
		return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
	}
	void Invalidate()
	{
		x = y = z = std::numeric_limits<float>::infinity();
	}

	float& operator[](int i)
	{
		return reinterpret_cast<float*>(this)[i];
	}
	float operator[](int i) const
	{
		return reinterpret_cast<float*>(const_cast<Vector*>(this))[i];
	}

	void Zero()
	{
		x = y = z = 0.f;
	}

	bool operator==(const Vector &src) const
	{
		return src.x == x && src.y == y && src.z == z;
	}
	bool operator!=(const Vector &src) const
	{
		return (src.x != x) || (src.y != y) || (src.z != z);
	}

	Vector& operator+=(const Vector &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}
	Vector& operator-=(const Vector &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}
	Vector& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		z *= fl;

		return *this;
	}
	Vector& operator*=(const Vector &v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;

		return *this;
	}
	Vector& operator/=(const Vector &v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;

		return *this;
	}
	Vector& operator+=(float fl)
	{
		x += fl;
		y += fl;
		z += fl;

		return *this;
	}
	Vector& operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		z /= fl;

		return *this;
	}
	Vector& operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		z -= fl;

		return *this;
	}

	void NormalizeInPlace()
	{
		*this = Normalized();
	}
	Vector Normalized() const
	{
		Vector res = *this;

		float l = res.Length();

		if (l != 0.0f)
			res /= l;
		else
			res.x = res.y = res.z = 0.f;

		return res;
	}

	float DistTo(const Vector &v) const
	{
		Vector delta;

		delta.x = x - v.x;
		delta.y = y - v.y;
		delta.z = z - v.z;

		return delta.Length();
	}
	float DistToSqr(const Vector &v) const
	{
		Vector delta;

		delta.x = x - v.x;
		delta.y = y - v.y;
		delta.z = z - v.z;

		return delta.LengthSqr();
	}

	float Dot(const Vector &v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	float Length() const
	{
		return sqrt(x*x + y*y + z*z);
	}
	float LengthSqr() const
	{
		return x*x + y*y + z*z;
	}
	float Length2D() const
	{
		return sqrt(x*x + y*y);
	}

	Vector& operator=(const Vector &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}

	Vector operator-() const
	{
		return Vector(-x, -y, -z);
	}
	Vector operator+(const Vector &v) const
	{
		return Vector(x + v.x, y + v.y, z + v.z);
	}
	Vector operator-(const Vector &v) const
	{
		return Vector(x - v.x, y - v.y, z - v.z);
	}
	Vector operator*(float fl) const
	{
		return Vector(x * fl, y * fl, z * fl);
	}
	Vector operator*(const Vector &v) const
	{
		return Vector(x * v.x, y * v.y, z * v.z);
	}
	Vector operator/(float fl) const
	{
		return Vector(x / fl, y / fl, z / fl);
	}
	Vector operator/(const Vector &v) const
	{
		return Vector(x / v.x, y / v.y, z / v.z);
	}

	float x, y, z;
};

inline Vector operator*(float lhs, const Vector &rhs)
{
	return rhs * lhs;
}
inline Vector operator/(float lhs, const Vector &rhs)
{
	return rhs / lhs;
}

class __declspec(align(16)) VectorAligned : public Vector
{
public:
	inline VectorAligned() {}
	inline VectorAligned(float x, float y, float z)
	{
		Init(x, y, z);
	}

public:
	explicit VectorAligned(const Vector &v)
	{
		Init(v.x, v.y, v.z);
	}

	VectorAligned& operator=(const Vector &v)
	{
		Init(v.x, v.y, v.z);
		return *this;
	}

	VectorAligned& operator=(const VectorAligned &v)
	{
		Init(v.x, v.y, v.z);
		return *this;
	}

	float w;
};
