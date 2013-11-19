/*
    File from a series of OpenGL tutorials on
    www.videotutorialsrock.com
*/

#include <math.h>
#include "vec3f.h"

Vec3f::Vec3f()
{
	v[0]=0;
	v[1]=0;
	v[2]=0;
}

Vec3f::Vec3f(float x, float y, float z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

const Vec3f &Vec3f::operator*=(float scale) {
	v[0] *= scale;
	v[1] *= scale;
	v[2] *= scale;
	return *this;
}

const Vec3f &Vec3f::operator/=(float scale) {
	v[0] /= scale;
	v[1] /= scale;
	v[2] /= scale;
	return *this;
}

const Vec3f &Vec3f::operator+=(const Vec3f &other) {
	v[0] += other.v[0];
	v[1] += other.v[1];
	v[2] += other.v[2];
	return *this;
}

const Vec3f &Vec3f::operator-=(const Vec3f &other) {
	v[0] -= other.v[0];
	v[1] -= other.v[1];
	v[2] -= other.v[2];
	return *this;
}

const Vec3f &Vec3f::operator*=(const Vec3f &other) {
	v[0] *= other.v[0];
	v[1] *= other.v[1];
	v[2] *= other.v[2];
	return *this;
}

const Vec3f &Vec3f::operator=(const Vec3f &other) {
	v[0] = other.v[0];
	v[1] = other.v[1];
	v[2] = other.v[2];
	return *this;
}

float Vec3f::magnitude() const
{
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

float Vec3f::magnitudeSquared() const
{
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

Vec3f Vec3f::normalize() const
{
	float m = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	return Vec3f(v[0] / m, v[1] / m, v[2] / m);
}

float Vec3f::dot(const Vec3f &other) const
{
	return v[0] * other.v[0] + v[1] * other.v[1] + v[2] * other.v[2];
}

Vec3f Vec3f::cross(const Vec3f &other) const
{
	return Vec3f(v[1] * other.v[2] - v[2] * other.v[1],
				 v[2] * other.v[0] - v[0] * other.v[2],
				 v[0] * other.v[1] - v[1] * other.v[0]);
}
