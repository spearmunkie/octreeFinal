/*
    File from a series of OpenGL tutorials on
    www.videotutorialsrock.com
*/

#ifndef VEC3F_H
#define VEC3F_H

class Vec3f
{
	public:
		Vec3f();
		Vec3f(float x, float y, float z);

        //treat it like an array
		float &operator[](int index) {return v[index];}
		float operator[](int index) const {return v[index];}

        //does all standard operations like a good vector should
		Vec3f operator*(float scale) const {return Vec3f(v[0] * scale, v[1] * scale, v[2] * scale);}
		Vec3f operator/(float scale) const {return Vec3f(v[0] / scale, v[1] / scale, v[2] / scale);}
		Vec3f operator+(const Vec3f &other) const {return Vec3f(v[0] + other.v[0], v[1] + other.v[1], v[2] + other.v[2]);}
		Vec3f operator-(const Vec3f &other) const {return Vec3f(v[0] - other.v[0], v[1] - other.v[1], v[2] - other.v[2]);}
		Vec3f operator-() const {return Vec3f(-v[0], -v[1], -v[2]);}

        //does standard operations
		const Vec3f &operator*=(float scale);
		const Vec3f &operator/=(float scale);
		const Vec3f &operator*=(const Vec3f &other);
		const Vec3f &operator+=(const Vec3f &other);
		const Vec3f &operator-=(const Vec3f &other);
		const Vec3f &operator=(const Vec3f &other);

        //does vector specific operations
		float magnitude() const;
		float magnitudeSquared() const;
		Vec3f normalize() const;
		float dot(const Vec3f &other) const;
		Vec3f cross(const Vec3f &other) const;
    private:
		float v[3];
};
#endif
