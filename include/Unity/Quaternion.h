#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#define SMALL_float 0.0000000001

/**
 * Attempt to include a header file if the file exists.
 * If the file does not exist, create a dummy data structure for that type.
 * If it cannot be determined if it exists, just attempt to include it.
 */
#ifdef __has_include
#   if __has_include("Vector3.h")
#       include "Vector3.h"
#   elif !defined(GMATH_VECTOR3)
#define GMATH_VECTOR3
        struct Vector3
        {
            union
            {
                struct
                {
                    float X;
                    float Y;
                    float Z;
                };
                float data[3];
            };

            Vector3() : X(0), Y(0), Z(0) {}
            Vector3(float data[]) : X(data[0]), Y(data[1]), Z(data[2])
                {}
            Vector3(float value) : X(value), Y(value), Z(value) {}
            Vector3(float x, float y) : X(x), Y(y), Z(0) {}
            Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {}

            static Vector3 Cross(Vector3 lhs, Vector3 rhs)
            {
                float x = lhs.Y * rhs.Z - lhs.Z * rhs.Y;
                float y = lhs.Z * rhs.X - lhs.X * rhs.Z;
                float z = lhs.X * rhs.Y - lhs.Y * rhs.X;
                return Vector3(x, y, z);
            }

            static float Dot(Vector3 lhs, Vector3 rhs)
            {
                return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;
            }

            static Vector3 Normalized(Vector3 v)
            {
                float mag = sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
                if (mag == 0)
                    return Vector3::Zero();
                return v / mag;
            }

            static Vector3 Orthogonal(Vector3 v)
            {
                return v.Z < v.X ?
                    Vector3(v.Y, -v.X, 0) : Vector3(0, -v.Z, v.Y);
            }

            static float SqrMagnitude(Vector3 v)
            {
                return v.X * v.X + v.Y * v.Y + v.Z * v.Z;
            }
        };


        Vector3 operator+(Vector3 lhs, const Vector3 rhs)
        {
            return Vector3(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z);
        }

        Vector3 operator*(Vector3 lhs, const float rhs)
        {
            return Vector3(lhs.X * rhs, lhs.Y * rhs, lhs.Z * rhs);
        }
#   endif
#else
#   include "Vector3.hpp"
#endif

struct Quaternion
{
    union
    {
        struct
        {
            float X;
            float Y;
            float Z;
            float W;
        };
        float data[4];
    };


    /**
     * Constructors.
     */
    Quaternion();
    Quaternion(float data[]);
    Quaternion(Vector3 vector, float scalar);
    Quaternion(float x, float y, float z, float w);


    /**
     * Constants for common quaternions.
     */
    static Quaternion Identity();


    /**
     * Returns the angle between two quaternions.
     * The quaternions must be normalized.
     * @param a: The first quaternion.
     * @param b: The second quaternion.
     * @return: A scalar value.
     */
    static float Angle(Quaternion a, Quaternion b);

    /**
     * Returns the conjugate of a quaternion.
     * @param rotation: The quaternion in question.
     * @return: A new quaternion.
     */
    static Quaternion Conjugate(Quaternion rotation);

    /**
     * Returns the dot product of two quaternions.
     * @param lhs: The left side of the multiplication.
     * @param rhs: The right side of the multiplication.
     * @return: A scalar value.
     */
    static float Dot(Quaternion lhs, Quaternion rhs);

    /**
     * Creates a new quaternion from the angle-axis representation of
     * a rotation.
     * @param angle: The rotation angle in radians.
     * @param axis: The vector about which the rotation occurs.
     * @return: A new quaternion.
     */
    static Quaternion FromAngleAxis(float angle, Vector3 axis);

    /**
     * Create a new quaternion from the euler angle representation of
     * a rotation. The z, x and y values represent rotations about those
     * axis in that respective order.
     * @param rotation: The x, y and z rotations.
     * @return: A new quaternion.
     */
    static Quaternion FromEuler(Vector3 rotation);

    /**
     * Create a new quaternion from the euler angle representation of
     * a rotation. The z, x and y values represent rotations about those
     * axis in that respective order.
     * @param x: The rotation about the x-axis in radians.
     * @param y: The rotation about the y-axis in radians.
     * @param z: The rotation about the z-axis in radians.
     * @return: A new quaternion.
     */
    static Quaternion FromEuler(float x, float y, float z);

    /**
     * Create a quaternion rotation which rotates "fromVector" to "toVector".
     * @param fromVector: The vector from which to start the rotation.
     * @param toVector: The vector at which to end the rotation.
     * @return: A new quaternion.
     */
    static Quaternion FromToRotation(Vector3 fromVector,
                                            Vector3 toVector);

    /**
     * Returns the inverse of a rotation.
     * @param rotation: The quaternion in question.
     * @return: A new quaternion.
     */
    static Quaternion Inverse(Quaternion rotation);

    /**
     * Interpolates between a and b by t, which is clamped to the range [0-1].
     * The result is normalized before being returned.
     * @param a: The starting rotation.
     * @param b: The ending rotation.
     * @return: A new quaternion.
     */
    static Quaternion Lerp(Quaternion a, Quaternion b, float t);

    /**
     * Interpolates between a and b by t. This normalizes the result when
     * complete.
     * @param a: The starting rotation.
     * @param b: The ending rotation.
     * @param t: The interpolation value.
     * @return: A new quaternion.
     */
    static Quaternion LerpUnclamped(Quaternion a, Quaternion b,
                                           float t);

    /**
     * Creates a rotation with the specified forward direction. This is the
     * same as calling LookRotation with (0, 1, 0) as the upwards vector.
     * The output is undefined for parallel vectors.
     * @param forward: The forward direction to look toward.
     * @return: A new quaternion.
     */
    static Quaternion LookRotation(Vector3 forward);

    /**
     * Creates a rotation with the specified forward and upwards directions.
     * The output is undefined for parallel vectors.
     * @param forward: The forward direction to look toward.
     * @param upwards: The direction to treat as up.
     * @return: A new quaternion.
     */
    static Quaternion LookRotation(Vector3 forward, Vector3 upwards);

    /**
     * Returns the norm of a quaternion.
     * @param rotation: The quaternion in question.
     * @return: A scalar value.
     */
    static float Norm(Quaternion rotation);

    /**
     * Returns a quaternion with identical rotation and a norm of one.
     * @param rotation: The quaternion in question.
     * @return: A new quaternion.
     */
    static Quaternion Normalized(Quaternion rotation);

    /**
     * Returns a new Quaternion created by rotating "from" towards "to" by
     * "maxRadiansDelta". This will not overshoot, and if a negative delta is
     * applied, it will rotate till completely opposite "to" and then stop.
     * @param from: The rotation at which to start.
     * @param to: The rotation at which to end.
     # @param maxRadiansDelta: The maximum number of radians to rotate.
     * @return: A new Quaternion.
     */
    static Quaternion RotateTowards(Quaternion from, Quaternion to,
                                           float maxRadiansDelta);

    /**
     * Returns a new quaternion interpolated between a and b, using spherical
     * linear interpolation. The variable t is clamped to the range [0-1]. The
     * resulting quaternion will be normalized.
     * @param a: The starting rotation.
     * @param b: The ending rotation.
     * @param t: The interpolation value.
     * @return: A new quaternion.
     */
    static Quaternion Slerp(Quaternion a, Quaternion b, float t);

    /**
     * Returns a new quaternion interpolated between a and b, using spherical
     * linear interpolation. The resulting quaternion will be normalized.
     * @param a: The starting rotation.
     * @param b: The ending rotation.
     * @param t: The interpolation value.
     * @return: A new quaternion.
     */
    static Quaternion SlerpUnclamped(Quaternion a, Quaternion b,
                                            float t);

    /**
     * Outputs the angle axis representation of the provided quaternion.
     * @param rotation: The input quaternion.
     * @param angle: The output angle.
     * @param axis: The output axis.
     */
    static void ToAngleAxis(Quaternion rotation, float &angle,
                                   Vector3 &axis);

    /**
     * Returns the Euler angle representation of a rotation. The resulting
     * vector contains the rotations about the z, x and y axis, in that order.
     * @param rotation: The quaternion to convert.
     * @return: A new vector.
     */
    static Vector3 ToEuler(Quaternion rotation);

    /**
     * Operator overloading.
     */
    struct Quaternion& operator+=(const float rhs);
    struct Quaternion& operator-=(const float rhs);
    struct Quaternion& operator*=(const float rhs);
    struct Quaternion& operator/=(const float rhs);
    struct Quaternion& operator+=(const Quaternion rhs);
    struct Quaternion& operator-=(const Quaternion rhs);
    struct Quaternion& operator*=(const Quaternion rhs);
};

Quaternion operator-(Quaternion rhs);
Quaternion operator+(Quaternion lhs, const float rhs);
Quaternion operator-(Quaternion lhs, const float rhs);
Quaternion operator*(Quaternion lhs, const float rhs);
Quaternion operator/(Quaternion lhs, const float rhs);
Quaternion operator+(const float lhs, Quaternion rhs);
Quaternion operator-(const float lhs, Quaternion rhs);
Quaternion operator*(const float lhs, Quaternion rhs);
Quaternion operator/(const float lhs, Quaternion rhs);
Quaternion operator+(Quaternion lhs, const Quaternion rhs);
Quaternion operator-(Quaternion lhs, const Quaternion rhs);
Quaternion operator*(Quaternion lhs, const Quaternion rhs);
Vector3 operator*(Quaternion lhs, const Vector3 rhs);
bool operator==(const Quaternion lhs, const Quaternion rhs);
bool operator!=(const Quaternion lhs, const Quaternion rhs);