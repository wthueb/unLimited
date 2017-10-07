#pragma once

class QAngle
{
public:
    QAngle()
    {
        Init();
    }
    QAngle(float x, float y, float z)
    {
        Init(x, y, z);
    }
    QAngle(const float* clr)
    {
        Init(clr[0], clr[1], clr[2]);
    }

    void Init(float ix = 0.f, float iy = 0.f, float iz = 0.f)
    {
        pitch = ix;
        yaw = iy;
        roll = iz;
    }

    float operator[](int i) const
    {
        return reinterpret_cast<float*>(const_cast<QAngle*>(this))[i];
    }
    float& operator[](int i)
    {
        return reinterpret_cast<float*>(this)[i];
    }

    QAngle& operator+=(const QAngle &v)
    {
        pitch += v.pitch;
		yaw += v.yaw;
		roll += v.roll;

        return *this;
    }
    QAngle& operator-=(const QAngle &v)
    {
        pitch -= v.pitch;
		yaw -= v.yaw;
		roll -= v.roll;

        return *this;
    }
    QAngle& operator*=(float fl)
    {
        pitch *= fl;
        yaw *= fl;
        roll *= fl;

        return *this;
    }
    QAngle& operator*=(const QAngle &v)
    {
        pitch *= v.pitch;
        yaw *= v.yaw;
        roll *= v.roll;

        return *this;
    }
    QAngle& operator/=(const QAngle &v)
    {
        pitch /= v.pitch;
        yaw /= v.yaw;
        roll /= v.roll;

        return *this;
    }
    QAngle& operator+=(float fl)
    {
        pitch += fl;
        yaw += fl;
        roll += fl;

        return *this;
    }
    QAngle& operator/=(float fl)
    {
        pitch /= fl;
        yaw /= fl;
        roll /= fl;

        return *this;
    }
    QAngle& operator-=(float fl)
    {
        pitch -= fl;
        yaw -= fl;
        roll -= fl;

        return *this;
    }

    QAngle& operator=(const QAngle &v)
    {
        pitch = v.pitch;
		yaw = v.yaw;
		roll = v.roll;

        return *this;
    }

    QAngle operator-() const
    {
        return QAngle(-pitch, -yaw, -roll);
    }
    QAngle operator+(const QAngle &v) const
    {
        return QAngle(pitch + v.pitch, yaw + v.yaw, roll + v.roll);
    }
    QAngle operator-(const QAngle &v) const
    {
        return QAngle(pitch - v.pitch, yaw - v.yaw, roll - v.roll);
    }
    QAngle operator*(float fl) const
    {
        return QAngle(pitch * fl, yaw * fl, roll * fl);
    }
    QAngle operator*(const QAngle &v) const
    {
        return QAngle(pitch * v.pitch, yaw * v.yaw, roll * v.roll);
    }
    QAngle operator/(float fl) const
    {
        return QAngle(pitch / fl, yaw / fl, roll / fl);
    }
    QAngle operator/(const QAngle &v) const
    {
        return QAngle(pitch / v.pitch, yaw / v.yaw, roll / v.roll);
    }

    float Length() const
    {
        return sqrt(pitch*pitch + yaw*yaw + roll*roll);
    }
    float LengthSqr() const
    {
        return pitch*pitch + yaw*yaw + roll*roll;
    }
    bool IsZero(float tolerance = .01f) const
    {
        return pitch > -tolerance && pitch < tolerance &&
            yaw > -tolerance && yaw < tolerance &&
            roll > -tolerance && roll < tolerance;
    }

    float pitch; // x
    float yaw; // y
    float roll; // z
};

inline QAngle operator*(float lhs, const QAngle &rhs)
{
    return rhs * lhs;
}
inline QAngle operator/(float lhs, const QAngle &rhs)
{
    return rhs / lhs;
}
