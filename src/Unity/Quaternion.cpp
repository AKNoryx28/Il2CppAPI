#include <Unity/Quaternion.h>

Quaternion::Quaternion() : X(0), Y(0), Z(0), W(1) {}
Quaternion::Quaternion(float data[]) : X(data[0]), Y(data[1]), Z(data[2]),
                                       W(data[3]) {}
Quaternion::Quaternion(Vector3 vector, float scalar) : X(vector.X),
                                                       Y(vector.Y), Z(vector.Z), W(scalar) {}
Quaternion::Quaternion(float x, float y, float z, float w) : X(x), Y(y),
                                                             Z(z), W(w) {}


Quaternion Quaternion::Identity() { return Quaternion(0, 0, 0, 1); }

float Quaternion::Angle(Quaternion a, Quaternion b)
{
    float dot = Dot(a, b);
    return acos(fmin(fabs(dot), 1)) * 2;
}

Quaternion Quaternion::Conjugate(Quaternion rotation)
{
    return Quaternion(-rotation.X, -rotation.Y, -rotation.Z, rotation.W);
}

float Quaternion::Dot(Quaternion lhs, Quaternion rhs) {
  return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z + lhs.W * rhs.W;
}

Quaternion Quaternion::FromAngleAxis(float angle, Vector3 axis)
{
    Quaternion q;
    float m = sqrt(axis.X * axis.X + axis.Y * axis.Y + axis.Z * axis.Z);
    float s = sin(angle / 2) / m;
    q.X = axis.X * s;
    q.Y = axis.Y * s;
    q.Z = axis.Z * s;
    q.W = cos(angle / 2);
    return q;
}

Quaternion Quaternion::FromEuler(Vector3 rotation)
{
    return FromEuler(rotation.X, rotation.Y, rotation.Z);
}

Quaternion Quaternion::FromEuler(float x, float y, float z)
{
    float cx = cos(x * 0.5);
    float cy = cos(y * 0.5);
    float cz = cos(z * 0.5);
    float sx = sin(x * 0.5);
    float sy = sin(y * 0.5);
    float sz = sin(z * 0.5);
    Quaternion q;
    q.X = cx * sy * sz + cy * cz * sx;
    q.Y = cx * cz * sy - cy * sx * sz;
    q.Z = cx * cy * sz - cz * sx * sy;
    q.W = sx * sy * sz + cx * cy * cz;
    return q;
}

Quaternion Quaternion::FromToRotation(Vector3 fromVector, Vector3 toVector)
{
    float dot = Vector3::Dot(fromVector, toVector);
    float k = sqrt(Vector3::SqrMagnitude(fromVector) *
                   Vector3::SqrMagnitude(toVector));
    if (fabs(dot / k + 1) < 0.00001)
    {
        Vector3 ortho = Vector3::Orthogonal(fromVector);
        return Quaternion(Vector3::Normalized(ortho), 0);
    }
    Vector3 cross = Vector3::Cross(fromVector, toVector);
    return Normalized(Quaternion(cross, dot + k));
}

Quaternion Quaternion::Inverse(Quaternion rotation)
{
    float n = Norm(rotation);
    return Conjugate(rotation) / (n * n);
}

Quaternion Quaternion::Lerp(Quaternion a, Quaternion b, float t)
{
    if (t < 0) return Normalized(a);
    else if (t > 1) return Normalized(b);
    return LerpUnclamped(a, b, t);
}

Quaternion Quaternion::LerpUnclamped(Quaternion a, Quaternion b, float t)
{
    Quaternion quaternion;
    if (Dot(a, b) >= 0)
        quaternion = a * (1 - t) + b * t;
    else
        quaternion = a * (1 - t) - b * t;
    return Normalized(quaternion);
}

Quaternion Quaternion::LookRotation(Vector3 forward)
{
    return LookRotation(forward, Vector3(0, 1, 0));
}

Quaternion Quaternion::LookRotation(Vector3 forward, Vector3 upwards)
{
    // Normalize inputs
    forward = Vector3::Normalized(forward);
    upwards = Vector3::Normalized(upwards);
    // Don't allow zero vectors
    if (Vector3::SqrMagnitude(forward) < SMALL_float || Vector3::SqrMagnitude(upwards) < SMALL_float)
        return Quaternion::Identity();
    // Handle alignment with up direction
    if (1 - fabs(Vector3::Dot(forward, upwards)) < SMALL_float)
        return FromToRotation(Vector3::Forward(), forward);
    // Get orthogonal vectors
    Vector3 right = Vector3::Normalized(Vector3::Cross(upwards, forward));
    upwards = Vector3::Cross(forward, right);
    // Calculate rotation
    Quaternion quaternion;
    float radicand = right.X + upwards.Y + forward.Z;
    if (radicand > 0)
    {
        quaternion.W = sqrt(1.0 + radicand) * 0.5;
        float recip = 1.0 / (4.0 * quaternion.W);
        quaternion.X = (upwards.Z - forward.Y) * recip;
        quaternion.Y = (forward.X - right.Z) * recip;
        quaternion.Z = (right.Y - upwards.X) * recip;
    }
    else if (right.X >= upwards.Y && right.X >= forward.Z)
    {
        quaternion.X = sqrt(1.0 + right.X - upwards.Y - forward.Z) * 0.5;
        float recip = 1.0 / (4.0 * quaternion.X);
        quaternion.W = (upwards.Z - forward.Y) * recip;
        quaternion.Z = (forward.X + right.Z) * recip;
        quaternion.Y = (right.Y + upwards.X) * recip;
    }
    else if (upwards.Y > forward.Z)
    {
        quaternion.Y = sqrt(1.0 - right.X + upwards.Y - forward.Z) * 0.5;
        float recip = 1.0 / (4.0 * quaternion.Y);
        quaternion.Z = (upwards.Z + forward.Y) * recip;
        quaternion.W = (forward.X - right.Z) * recip;
        quaternion.X = (right.Y + upwards.X) * recip;
    }
    else
    {
        quaternion.Z = sqrt(1.0 - right.X - upwards.Y + forward.Z) * 0.5;
        float recip = 1.0 / (4.0 * quaternion.Z);
        quaternion.Y = (upwards.Z + forward.Y) * recip;
        quaternion.X = (forward.X + right.Z) * recip;
        quaternion.W = (right.Y - upwards.X) * recip;
    }
    return quaternion;
}

float Quaternion::Norm(Quaternion rotation)
{
    return sqrt(rotation.X * rotation.X +
                rotation.Y * rotation.Y +
                rotation.Z * rotation.Z +
                rotation.W * rotation.W);
}

Quaternion Quaternion::Normalized(Quaternion rotation)
{
    return rotation / Norm(rotation);
}

Quaternion Quaternion::RotateTowards(Quaternion from, Quaternion to,
                                     float maxRadiansDelta)
{
    float angle = Quaternion::Angle(from, to);
    if (angle == 0)
        return to;
    maxRadiansDelta = fmax(maxRadiansDelta, angle - M_PI);
    float t = fmin(1, maxRadiansDelta / angle);
    return Quaternion::SlerpUnclamped(from, to, t);
}

Quaternion Quaternion::Slerp(Quaternion a, Quaternion b, float t)
{
    if (t < 0) return Normalized(a);
    else if (t > 1) return Normalized(b);
    return SlerpUnclamped(a, b, t);
}

Quaternion Quaternion::SlerpUnclamped(Quaternion a, Quaternion b, float t)
{
    float n1;
    float n2;
    float n3 = Dot(a, b);
    bool flag = false;
    if (n3 < 0)
    {
        flag = true;
        n3 = -n3;
    }
    if (n3 > 0.999999)
    {
        n2 = 1 - t;
        n1 = flag ? -t : t;
    }
    else
    {
        float n4 = acos(n3);
        float n5 = 1 / sin(n4);
        n2 = sin((1 - t) * n4) * n5;
        n1 = flag ? -sin(t * n4) * n5 : sin(t * n4) * n5;
    }
    Quaternion quaternion;
    quaternion.X = (n2 * a.X) + (n1 * b.X);
    quaternion.Y = (n2 * a.Y) + (n1 * b.Y);
    quaternion.Z = (n2 * a.Z) + (n1 * b.Z);
    quaternion.W = (n2 * a.W) + (n1 * b.W);
    return Normalized(quaternion);
}

void Quaternion::ToAngleAxis(Quaternion rotation, float &angle, Vector3 &axis)
{
    if (rotation.W > 1)
        rotation = Normalized(rotation);
    angle = 2 * acos(rotation.W);
    float s = sqrt(1 - rotation.W * rotation.W);
    if (s < 0.00001) {
        axis.X = 1;
        axis.Y = 0;
        axis.Z = 0;
    } else {
        axis.X = rotation.X / s;
        axis.Y = rotation.Y / s;
        axis.Z = rotation.Z / s;
    }
}

Vector3 Quaternion::ToEuler(Quaternion rotation)
{
    float sqw = rotation.W * rotation.W;
    float sqx = rotation.X * rotation.X;
    float sqy = rotation.Y * rotation.Y;
    float sqz = rotation.Z * rotation.Z;
    // If normalized is one, otherwise is correction factor
    float unit = sqx + sqy + sqz + sqw;
    float test = rotation.X * rotation.W - rotation.Y * rotation.Z;
    Vector3 v;
    // Singularity at north pole
    if (test > 0.4995f * unit)
    {
        v.Y = 2 * atan2(rotation.Y, rotation.X);
        v.X = M_PI_2;
        v.Z = 0;
        return v;
    }
    // Singularity at south pole
    if (test < -0.4995f * unit)
    {
        v.Y = -2 * atan2(rotation.Y, rotation.X);
        v.X = -M_PI_2;
        v.Z = 0;
        return v;
    }
    // Yaw
    v.Y = atan2(2 * rotation.W * rotation.Y + 2 * rotation.Z * rotation.X,
                1 - 2 * (rotation.X * rotation.X + rotation.Y * rotation.Y));
    // Pitch
    v.X = asin(2 * (rotation.W * rotation.X - rotation.Y * rotation.Z));
    // Roll
    v.Z = atan2(2 * rotation.W * rotation.Z + 2 * rotation.X * rotation.Y,
                1 - 2 * (rotation.Z * rotation.Z + rotation.X * rotation.X));
    return v;
}

struct Quaternion& Quaternion::operator+=(const float rhs)
{
    X += rhs;
    Y += rhs;
    Z += rhs;
    W += rhs;
    return *this;
}

struct Quaternion& Quaternion::operator-=(const float rhs)
{
    X -= rhs;
    Y -= rhs;
    Z -= rhs;
    W -= rhs;
    return *this;
}

struct Quaternion& Quaternion::operator*=(const float rhs)
{
    X *= rhs;
    Y *= rhs;
    Z *= rhs;
    W *= rhs;
    return *this;
}

struct Quaternion& Quaternion::operator/=(const float rhs)
{
    X /= rhs;
    Y /= rhs;
    Z /= rhs;
    W /= rhs;
    return *this;
}

struct Quaternion& Quaternion::operator+=(const Quaternion rhs)
{
    X += rhs.X;
    Y += rhs.Y;
    Z += rhs.Z;
    W += rhs.W;
    return *this;
}

struct Quaternion& Quaternion::operator-=(const Quaternion rhs)
{
    X -= rhs.X;
    Y -= rhs.Y;
    Z -= rhs.Z;
    W -= rhs.W;
    return *this;
}

struct Quaternion& Quaternion::operator*=(const Quaternion rhs)
{
    Quaternion q;
    q.W = W * rhs.W - X * rhs.X - Y * rhs.Y - Z * rhs.Z;
    q.X = X * rhs.W + W * rhs.X + Y * rhs.Z - Z * rhs.Y;
    q.Y = W * rhs.Y - X * rhs.Z + Y * rhs.W + Z * rhs.X;
    q.Z = W * rhs.Z + X * rhs.Y - Y * rhs.X + Z * rhs.W;
    *this = q;
    return *this;
}

Quaternion operator-(Quaternion rhs) { return rhs * -1; }
Quaternion operator+(Quaternion lhs, const float rhs) { return lhs += rhs; }
Quaternion operator-(Quaternion lhs, const float rhs) { return lhs -= rhs; }
Quaternion operator*(Quaternion lhs, const float rhs) { return lhs *= rhs; }
Quaternion operator/(Quaternion lhs, const float rhs) { return lhs /= rhs; }
Quaternion operator+(const float lhs, Quaternion rhs) { return rhs += lhs; }
Quaternion operator-(const float lhs, Quaternion rhs) { return rhs -= lhs; }
Quaternion operator*(const float lhs, Quaternion rhs) { return rhs *= lhs; }
Quaternion operator/(const float lhs, Quaternion rhs) { return rhs /= lhs; }
Quaternion operator+(Quaternion lhs, const Quaternion rhs)
{
    return lhs += rhs;
}
Quaternion operator-(Quaternion lhs, const Quaternion rhs)
{
    return lhs -= rhs;
}
Quaternion operator*(Quaternion lhs, const Quaternion rhs)
{
    return lhs *= rhs;
}

Vector3 operator*(Quaternion lhs, const Vector3 rhs)
{
    Vector3 u = Vector3(lhs.X, lhs.Y, lhs.Z);
    float s = lhs.W;
    return u * (Vector3::Dot(u, rhs) * 2)
           + rhs * (s * s - Vector3::Dot(u, u))
           + Vector3::Cross(u, rhs) * (2.0 * s);
}

bool operator==(const Quaternion lhs, const Quaternion rhs)
{
    return lhs.X == rhs.X &&
           lhs.Y == rhs.Y &&
           lhs.Z == rhs.Z &&
           lhs.W == rhs.W;
}

bool operator!=(const Quaternion lhs, const Quaternion rhs)
{
    return !(lhs == rhs);
}
