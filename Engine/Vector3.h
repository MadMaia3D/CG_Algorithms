#pragma once
#include "Colors.h"

template<typename T>
class Vector3 {
public:
	Vector3() : x((T)0), y((T)0), z((T)0) {}
	Vector3(T x, T y, T z) : x(x), y(y), z(z){}

	explicit Vector3(Color c) : x(c.GetR() / (T)255), y(c.GetG() / (T)255), z(c.GetB() / (T)255) {}
	explicit operator Color() const {
		return { (unsigned char)(x * 255), (unsigned char)(y * 255), (unsigned char)(z * 255) };
	}

	Vector3& operator+=(const Vector3 &rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	Vector3& operator-=(const Vector3 &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	Vector3& operator*=(const T &rhs) {
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}
	Vector3& operator/=(const T &rhs) {
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}
	Vector3 operator+(const Vector3 &rhs) const {
		return Vector3(*this) += rhs;
	}
	Vector3 operator-(const Vector3 &rhs) const {
		return Vector3(*this) -= rhs;
	}
	Vector3 operator*(const T &rhs) const {
		return Vector3(*this) *= rhs;
	}
	Vector3 operator/(const T &rhs) const {
		return Vector3(*this) /= rhs;
	}
	Vector3 operator-() const {
		return Vector3(-x, -y, -z);
	}
	bool operator==(const Vector3 &rhs) const {
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}
	bool operator!=(const Vector3 &rhs) const {
		return !(*this == rhs);
	}
public:
	T x;
	T y;
	T z;
};

typedef Vector3<float> Vec3;
typedef Vector3<double> Ved3;
typedef Vector3<int> Vei3;