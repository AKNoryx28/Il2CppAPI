#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

struct Vector2
{
    union
    {
        struct
        {
            float X;
            float Y;
        };
        float data[2];
    };

    // Constuctors
    Vector2();
    Vector2(float data[2]);
    Vector2(float value);
    Vector2(float x, float y);

    // Const for common vectors
    static Vector2 Zero();
    static Vector2 One();
    static Vector2 Right();
    static Vector2 Left();
    static Vector2 Up();
    static Vector2 Down();

    /**
     * Returns the angle between two vectors in radians.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A scalar value.
     */
    static float Angle(Vector2 a, Vector2 b);

    /**
     * Returns a vector with its magnitude clamped to maxLength.
     * @param vector: The target vector.
     * @param maxLength: The maximum length of the return vector.
     * @return: A new vector.
     */
    static Vector2 ClampMagnitude(Vector2 vector, float maxLength);

    /**
     * Returns the component of a in the direction of b (scalar projection).
     * @param a: The target vector.
     * @param b: The vector being compared against.
     * @return: A scalar value.
     */
    static float Component(Vector2 a, Vector2 b);

    /**
     * Returns the distance between a and b.
     * @param a: The first point.
     * @param b: The second point.
     * @return: A scalar value.
     */
    static float Distance(Vector2 a, Vector2 b);

    /**
     * Returns the dot product of two vectors.
     * @param lhs: The left side of the multiplication.
     * @param rhs: The right side of the multiplication.
     * @return: A scalar value.
     */
    static float Dot(Vector2 lhs, Vector2 rhs);

    /**
     * Converts a polar representation of a vector into cartesian
     * coordinates.
     * @param rad: The magnitude of the vector.
     * @param theta: The angle from the X axis.
     * @return: A new vector.
     */
    static Vector2 FromPolar(float rad, float theta);

    /**
     * Returns a vector linearly interpolated between a and b, moving along
     * a straight line. The vector is clamped to never go beyond the end points.
     * @param a: The starting point.
     * @param b: The ending point.
     * @param t: The interpolation value [0-1].
     * @return: A new vector.
     */
    static Vector2 Lerp(Vector2 a, Vector2 b, float t);

    /**
     * Returns a vector linearly interpolated between a and b, moving along
     * a straight line.
     * @param a: The starting point.
     * @param b: The ending point.
     * @param t: The interpolation value [0-1] (no actual bounds).
     * @return: A new vector.
     */
    static Vector2 LerpUnclamped(Vector2 a, Vector2 b, float t);

    /**
     * Returns the magnitude of a vector.
     * @param v: The vector in question.
     * @return: A scalar value.
     */
    static float Magnitude(Vector2 v);

    /**
     * Returns a vector made from the largest components of two other vectors.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A new vector.
     */
    static Vector2 Max(Vector2 a, Vector2 b);

    /**
     * Returns a vector made from the smallest components of two other vectors.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A new vector.
     */
    static Vector2 Min(Vector2 a, Vector2 b);

    /**
     * Returns a vector "maxDistanceDelta" units closer to the target. This
     * interpolation is in a straight line, and will not overshoot.
     * @param current: The current position.
     * @param target: The destination position.
     * @param maxDistanceDelta: The maximum distance to move.
     * @return: A new vector.
     */
    static Vector2 MoveTowards(Vector2 current, Vector2 target,
                               float maxDistanceDelta);

    /**
     * Returns a new vector with magnitude of one.
     * @param v: The vector in question.
     * @return: A new vector.
     */
    static Vector2 Normalized(Vector2 v);

    /**
     * Creates a new coordinate system out of the two vectors.
     * Normalizes "normal" and normalizes "tangent" and makes it orthogonal to
     * "normal"..
     * @param normal: A reference to the first axis vector.
     * @param tangent: A reference to the second axis vector.
     */
    static void OrthoNormalize(Vector2 &normal, Vector2 &tangent);

    /**
     * Returns the vector projection of a onto b.
     * @param a: The target vector.
     * @param b: The vector being projected onto.
     * @return: A new vector.
     */
    static Vector2 Project(Vector2 a, Vector2 b);

    /**
     * Returns a vector reflected about the provided line.
     * This behaves as if there is a plane with the line as its normal, and the
     * vector comes in and bounces off this plane.
     * @param vector: The vector traveling inward at the imaginary plane.
     * @param line: The line about which to reflect.
     * @return: A new vector pointing outward from the imaginary plane.
     */
    static Vector2 Reflect(Vector2 vector, Vector2 line);

    /**
     * Returns the vector rejection of a on b.
     * @param a: The target vector.
     * @param b: The vector being projected onto.
     * @return: A new vector.
     */
    static Vector2 Reject(Vector2 a, Vector2 b);

    /**
     * Rotates vector "current" towards vector "target" by "maxRadiansDelta".
     * This treats the vectors as directions and will linearly interpolate
     * between their magnitudes by "maxMagnitudeDelta". This function does not
     * overshoot. If a negative delta is supplied, it will rotate away from
     * "target" until it is pointing the opposite direction, but will not
     * overshoot that either.
     * @param current: The starting direction.
     * @param target: The destination direction.
     * @param maxRadiansDelta: The maximum number of radians to rotate.
     * @param maxMagnitudeDelta: The maximum delta for magnitude interpolation.
     * @return: A new vector.
     */
    static Vector2 RotateTowards(Vector2 current, Vector2 target,
                                 float maxRadiansDelta,
                                 float maxMagnitudeDelta);

    /**
     * Multiplies two vectors component-wise.
     * @param a: The lhs of the multiplication.
     * @param b: The rhs of the multiplication.
     * @return: A new vector.
     */
    static Vector2 Scale(Vector2 a, Vector2 b);

    /**
     * Returns a vector rotated towards b from a by the percent t.
     * Since interpolation is done spherically, the vector moves at a constant
     * angular velocity. This rotation is clamped to 0 <= t <= 1.
     * @param a: The starting direction.
     * @param b: The ending direction.
     * @param t: The interpolation value [0-1].
     */
    static Vector2 Slerp(Vector2 a, Vector2 b, float t);

    /**
     * Returns a vector rotated towards b from a by the percent t.
     * Since interpolation is done spherically, the vector moves at a constant
     * angular velocity. This rotation is unclamped.
     * @param a: The starting direction.
     * @param b: The ending direction.
     * @param t: The interpolation value [0-1].
     */
    static Vector2 SlerpUnclamped(Vector2 a, Vector2 b, float t);

    /**
     * Returns the squared magnitude of a vector.
     * This is useful when comparing relative lengths, where the exact length
     * is not important, and much time can be saved by not calculating the
     * square root.
     * @param v: The vector in question.
     * @return: A scalar value.
     */
    static float SqrMagnitude(Vector2 v);

    /**
     * Calculates the polar coordinate space representation of a vector.
     * @param vector: The vector to convert.
     * @param rad: The magnitude of the vector.
     * @param theta: The angle from the X axis.
     */
    static void ToPolar(Vector2 vector, float &rad, float &theta);


    /**
     * Operator overloading.
     */
    struct Vector2& operator+=(const float rhs);
    struct Vector2& operator-=(const float rhs);
    struct Vector2& operator*=(const float rhs);
    struct Vector2& operator/=(const float rhs);
    struct Vector2& operator+=(const Vector2 rhs);
    struct Vector2& operator-=(const Vector2 rhs);
};

Vector2 operator-(Vector2 rhs);
Vector2 operator+(Vector2 lhs, const float rhs);
Vector2 operator-(Vector2 lhs, const float rhs);
Vector2 operator*(Vector2 lhs, const float rhs);
Vector2 operator/(Vector2 lhs, const float rhs);
Vector2 operator+(const float lhs, Vector2 rhs);
Vector2 operator-(const float lhs, Vector2 rhs);
Vector2 operator*(const float lhs, Vector2 rhs);
Vector2 operator/(const float lhs, Vector2 rhs);
Vector2 operator+(Vector2 lhs, const Vector2 rhs);
Vector2 operator-(Vector2 lhs, const Vector2 rhs);
bool operator==(const Vector2 lhs, const Vector2 rhs);
bool operator!=(const Vector2 lhs, const Vector2 rhs);