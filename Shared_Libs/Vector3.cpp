#include "Vector3.h"

namespace TDS
{
    Vec3::Vec3() : x(0), y(0), z(0) {}
    //Vec3::Vec3(const Vec3& v) : x(v.x), y(v.y), z(v.z) {}
    Vec3::Vec3(float value) : x(value), y(value), z(value) {}
    Vec3::Vec3(float data[]) : x(data[0]), y(data[1]), z(data[2]) {}
    Vec3::Vec3(float newX, float newY, float newZ) : x(newX), y(newY), z(newZ) {}
    Vec3::Vec3(const Vec2& v, float newZ) : x(v.x), y(v.y), z(newZ) {}
    Vec3::Vec3(const float* newX, const float* newY, const float* newZ) : x(*newX), y(*newY), z(*newZ) {}
    //Vec3::~Vec3() {}

    Vec3 Vec3::back() { return Vec3(0.f, 0.f, -1.f); }
    Vec3 Vec3::down() { return Vec3(0.f, -1.f, 0.f); }
    Vec3 Vec3::forward() { return Vec3(0.f, 0.f, 1.f); }
    Vec3 Vec3::left() { return Vec3(-1.f, 0.f, 0.f); }
    Vec3 Vec3::one() { return Vec3(1.f, 1.f, 1.f); }
    Vec3 Vec3::right() { return Vec3(1.f, 0.f, 0.f); }
    Vec3 Vec3::up() { return Vec3(0.f, 1.f, 0.f); }
    Vec3 Vec3::zero() { return Vec3(0.f, 0.f, 0.f); }
    Vec3 Vec3::positiveInfinity() { return Vec3(INFINITY, INFINITY, INFINITY); }
    Vec3 Vec3::negativeInfinity() { return Vec3(-INFINITY, -INFINITY, -INFINITY); }

    float Vec3::magnitude() { return Mathf::Sqrt(x * x + y * y + z * z); }
    Vec3 Vec3::normalize()
    {
        Vec3 v = Vec3(x, y, z);
        v.Normalize();
        return v;
    }

    float Vec3::sqrMagnitude() { return x * x + y * y + z * z; }
    bool Vec3::Equals(const Vec3& v) { return x == v.x && y == v.y && z == v.z; }
    void Vec3::Normalize()
    {
        float mag = magnitude();
        if (mag > kEpsilon)
        {
            x /= mag;
            y /= mag;
            z /= mag;
        }
        else
        {
            x = 0;
            y = 0;
            z = 0;
        }
    }

    void Vec3::Set(float newX, float newY, float newZ) { x = newX; y = newY; z = newZ; }

    std::string Vec3::ToString()
    {
        std::ostringstream oss;
        oss << "Vec3(" << x << ", " << y << ", " << z << ")";
        return oss.str();
    }

    float Vec3::Angle(Vec3 from, Vec3 to)
    {
        float denominator = Mathf::Sqrt(from.sqrMagnitude() * to.sqrMagnitude());
        if (denominator < kEpsilonNormalSqrt)
            return 0.f;
        float dot = Mathf::Clamp(Dot(from, to) / denominator, -1.f, 1.f);
        return Mathf::Acos(dot);
    }

    Vec3 Vec3::ClampMagnitude(Vec3 vector, float maxLength)
    {
        float sqrmag = vector.sqrMagnitude();
        if (sqrmag > maxLength * maxLength)
        {
            float mag = Mathf::Sqrt(sqrmag);
            float normalizedX = vector.x / mag;
            float normalizedY = vector.y / mag;
            float normalizedZ = vector.z / mag;
            return Vec3(normalizedX * maxLength, normalizedY * maxLength, normalizedZ * maxLength);
        }
        return vector;
    }

    Vec3 Vec3::Cross(const Vec3 lhs, const Vec3 rhs)
    {
        float x = lhs.y * rhs.z - lhs.z * rhs.y;
        float y = lhs.z * rhs.x - lhs.x * rhs.z;
        float z = lhs.x * rhs.y - lhs.y * rhs.x;
        return Vec3(x, y, z);
    }

    float Vec3::Distance(Vec3 a, Vec3 b)
    {
        float diffX = a.x - b.x;
        float diffY = a.y - b.y;
        float diffZ = a.z - b.z;
        return Mathf::Sqrt(diffX * diffX + diffY * diffY + diffZ * diffZ);
    }

    float Vec3::Dot(const Vec3 lhs, const Vec3 rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }

    Vec3 Vec3::Lerp(const Vec3 a, const Vec3 b, float t)
    {
        t = Mathf::Clamp01(t);
        return Vec3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
    }

    Vec3 Vec3::LerpUnclamped(const Vec3 a, const Vec3 b, float t) 
    { 
        return Vec3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t); 
    }

    float Vec3::Magnitude(const Vec3 vector)
    {
		return (Mathf::Sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z));
    }

    Vec3 Vec3::Max(const Vec3 lhs, const Vec3 rhs) 
    { 
        return Vec3(Mathf::Max(lhs.x, rhs.x), Mathf::Max(lhs.y, rhs.y), Mathf::Max(lhs.z, rhs.z)); 
    }

    Vec3 Vec3::Min(const Vec3 lhs, const Vec3 rhs) 
    { 
        return Vec3(Mathf::Min(lhs.x, rhs.x), Mathf::Min(lhs.y, rhs.y), Mathf::Min(lhs.z, rhs.z)); 
    }

    Vec3 Vec3::MoveTowards(Vec3 current, Vec3 target, float maxDistanceDelta)
    {
        float toVector_x = target.x - current.x;
        float toVector_y = target.y - current.y;
        float toVector_z = target.z - current.z;

        float sqDist = toVector_x * toVector_x + toVector_y * toVector_y + toVector_z * toVector_z;

        if (sqDist == 0 || (maxDistanceDelta >= 0 && sqDist <= maxDistanceDelta * maxDistanceDelta))
            return target;
        float dist = Mathf::Sqrt(sqDist);

        return Vec3(current.x + toVector_x / dist * maxDistanceDelta, 
        current.y + toVector_y / dist * maxDistanceDelta, 
        current.z + toVector_z / dist * maxDistanceDelta);
    }

    Vec3 Vec3::Normalize(Vec3 value)
    {
        float mag = value.magnitude();
        if (mag > kEpsilon)
        {
            float x = value.x / mag;
            float y = value.y / mag;
            float z = value.z / mag;
            return Vec3(x, y, z);
        }
        else
        {
            return Vec3::zero();
        }
    }

    Vec3 Vec3::Orthogonal(Vec3 normal) 
    {
        return normal.z < normal.x ? Vec3(normal.y, -normal.x, 0.f) : Vec3(0.f, -normal.z, normal.y);
    }

    void Vec3::OrthoNormalize(Vec3& normal, Vec3& tangent, Vec3& binormal) 
    {
        normal = Normalize(normal);
        tangent = ProjectOnPlane(tangent, normal);
        tangent = Normalize(tangent);
        binormal = ProjectOnPlane(binormal, tangent);
        binormal = ProjectOnPlane(binormal, normal);
        binormal = Normalize(binormal);
    }

    Vec3 Vec3::Project(Vec3 vector, Vec3 onNormal)
    {
        float sqrMag = Dot(onNormal, onNormal);
        if (sqrMag < Mathf::Epsilon)
            return Vec3::zero();
        else
        {
            float dot = Dot(vector, onNormal);
            return Vec3(onNormal.x * dot / sqrMag, 
                        onNormal.y * dot / sqrMag, 
                        onNormal.z * dot / sqrMag);
        }
    }

    Vec3 Vec3::ProjectOnPlane(Vec3 vector, Vec3 planeNormal)
    {
        float sqrMag = Dot(planeNormal, planeNormal);
        if (sqrMag < Mathf::Epsilon)
            return vector;
        else
        {
            float dot = Dot(vector, planeNormal);
            return Vec3(vector.x - planeNormal.x * dot / sqrMag, 
                        vector.y - planeNormal.y * dot / sqrMag, 
                        vector.z - planeNormal.z * dot / sqrMag);
        }
    }

    Vec3 Vec3::Reflect(Vec3 inDirection, Vec3 inNormal)
    {
        return inDirection - 2.f * Project(inDirection, inNormal);
    }

    Vec3 Vec3::RotateTowards(Vec3 current, Vec3 target, float maxRadiansDelta, float maxMagnitudeDelta)
    {
        float magCur = current.magnitude();
        float magTar = target.magnitude();
        float newMag = magCur + maxMagnitudeDelta * ((magTar > magCur) - (magCur > magTar));
        newMag = Mathf::Min(newMag, Mathf::Max(magCur, magTar));
        newMag = Mathf::Max(newMag, Mathf::Min(magCur, magTar));

        float totalAngle = Angle(current, target) - maxRadiansDelta;
        if (totalAngle <= 0)
            return target.normalize() * newMag;
        else if (totalAngle >= Mathf::PI)
            return -target.normalize() * newMag;
        
        Vec3 axis = Cross(current, target);
        float magAxis = axis.magnitude();
        if (magAxis < kEpsilonNormalSqrt)
        {
            axis = Orthogonal(current);
            axis.Normalize();
        }
        else
        {
            axis /= magAxis;
        }
        current.Normalize();
        Vec3 newVector = current * Mathf::Cos(maxRadiansDelta) + Cross(axis, current) * Mathf::Sin(maxRadiansDelta);
        return newVector * newMag;
    }

    Vec3 Vec3::Scale(const Vec3 a, const Vec3 b) { return Vec3(a.x * b.x, a.y * b.y, a.z * b.z); }
    void Vec3::Scale(const Vec3 scale) { x *= scale.x; y *= scale.y; z *= scale.z; }

    float Vec3::SignedAngle(Vec3 from, Vec3 to, Vec3 axis)
    {
        float unsignedAngle = Angle(from, to);
        float cross_x = from.y * to.z - from.z * to.y;
        float cross_y = from.z * to.x - from.x * to.z;
        float cross_z = from.x * to.y - from.y * to.x;
        float sign = Mathf::Sign(axis.x * cross_x + axis.y * cross_y + axis.z * cross_z);
        return unsignedAngle * sign;
    }

    Vec3 Vec3::Slerp(Vec3 a, Vec3 b, float t)
    {
        if (t < 0.f) return a;
        if (t > 1.f) return b;
        return SlerpUnclamped(a, b, t);
    }

    Vec3 Vec3::SlerpUnclamped(Vec3 a, Vec3 b, float t)
    {
        float magA = a.magnitude();
        float magB = b.magnitude();
        a/= magA;
        b/= magB;
        float dot = Dot(a, b);
        dot = Mathf::Clamp(dot, -1.f, 1.f);
        float theta = Mathf::Acos(dot) * t;
        Vec3 relativeVec = b - a * dot;
        relativeVec.Normalize();
        Vec3 newVec = a * Mathf::Cos(theta) + relativeVec * Mathf::Sin(theta);
        return newVec * (magA + (magB - magA) * t);
    }

    float* Vec3::Vec3Value_ptr(const Vec3& vec)
    {
        float* ptr = new float[3];

        for (int i{ 0 }; i < 3; ++i)
        {
            ptr[i] = vec[i];
        }

        return  ptr;
    }

    Vec3& Vec3::operator=(const Vec3& v) 
    {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    Vec3& Vec3::operator+=(float scalar) 
    {
        x += scalar;
        y += scalar;
        z += scalar;
        return *this;
    }

    Vec3& Vec3::operator+=(const Vec3& v) 
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vec3& Vec3::operator-=(float scalar) 
    {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        return *this;
    }

    Vec3& Vec3::operator-=(const Vec3& v) 
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vec3& Vec3::operator*=(float scalar) 
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vec3& Vec3::operator*=(const Vec3& v) 
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    Vec3& Vec3::operator/=(float scalar) 
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    Vec3& Vec3::operator/=(const Vec3& v) 
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    float& Vec3::operator[](int index) 
    {
        switch (index)
        {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("Index out of range");
        }
    }

    float const& Vec3::operator[](int index) const
    {
        switch (index)
        {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("Index out of range");
        }
    }

    Vec3::operator Vec2() { return Vec2(x, y); }
    Vec3::operator Vec4() { return Vec4(x, y, z, 0.f); }

    std::ostream& operator<<(std::ostream& os, const Vec3& v)
    {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }

    Vec3 operator-(const Vec3& v) 
    {
        return Vec3(-v.x, -v.y, -v.z);
    }

    Vec3 operator+(const Vec3& v, float const& scalar) 
    {
        return Vec3(v.x + scalar, v.y + scalar, v.z + scalar);
    }

    Vec3 operator+(float const& scalar, const Vec3& v) 
    {
        return Vec3(v.x + scalar, v.y + scalar, v.z + scalar);
    }

    Vec3 operator+(const Vec3& v1, const Vec3& v2) 
    {
        return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    }

    Vec3 operator-(const Vec3& v, float const& scalar) 
    {
        return Vec3(v.x - scalar, v.y - scalar, v.z - scalar);
    }

    Vec3 operator-(float const& scalar, const Vec3& v) 
    {
        return Vec3(scalar - v.x, scalar - v.y, scalar - v.z);
    }

    Vec3 operator-(const Vec3& v1, const Vec3& v2) 
    {
        return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    }

    Vec3 operator*(const Vec3& v, float const& scalar) 
    {
        return Vec3(v.x * scalar, v.y * scalar, v.z * scalar);
    }

    Vec3 operator*(float const& scalar, const Vec3& v) 
    {
        return Vec3(v.x * scalar, v.y * scalar, v.z * scalar);
    }

    Vec3 operator*(const Vec3& v1, const Vec3& v2) 
    {
        return Vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
    }

    Vec3 operator/(const Vec3& v, float const& scalar) 
    {
        return Vec3(v.x / scalar, v.y / scalar, v.z / scalar);
    }

    Vec3 operator/(float const& scalar, const Vec3& v) 
    {
        return Vec3(scalar / v.x, scalar / v.y, scalar / v.z);
    }

    bool operator==(const Vec3& v1, const Vec3& v2) 
    {
        return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
    }

    bool operator!=(const Vec3& v1, const Vec3& v2) 
    {
        return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z;
    }

    Vec3 floatsToVec3(float x, float y, float z)
    {
        return Vec3(x, y, z);
    }

}  // namespace WD