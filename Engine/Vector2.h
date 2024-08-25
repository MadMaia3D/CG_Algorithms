/******************************************************************************************
* Chili DirectX Framework Version 16.10.01             *
* Vec2.h                      *
* Copyright 2016 PlanetChili <http://www.planetchili.net>          *
*                        *
* This file is part of The Chili DirectX Framework.           *
*                        *
* The Chili DirectX Framework is free software: you can redistribute it and/or modify   *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation, either version 3 of the License, or       *
* (at your option) any later version.               *
*                        *
* The Chili DirectX Framework is distributed in the hope that it will be useful,    *
* but WITHOUT ANY WARRANTY; without even the implied warranty of        *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU General Public License for more details.            *
*                        *
* You should have received a copy of the GNU General Public License       *
* along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once

#include "ChiliMath.h"

template <typename T>
class Vector2 {
public:
	Vector2() {}
	constexpr Vector2(T x, T y)
		:
		x(x),
		y(y) {
	}
	template <typename T2>
	explicit operator Vector2<T2>() const {
		return { (T2)x,(T2)y };
	}
	T  LenSq() const {
		return sq(*this);
	}
	T  Len() const {
		return sqrt(LenSq());
	}
	Vector2& Normalize() {
		const T length = Len();
		x /= length;
		y /= length;
		return *this;
	}
	Vector2 GetNormalized() const {
		Vector2 norm = *this;
		norm.Normalize();
		return norm;
	}
	Vector2 operator-() const {
		return Vector2(-x, -y);
	}
	Vector2& operator+=(const Vector2 &rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Vector2& operator-=(const Vector2 &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	T  operator*(const Vector2 &rhs) const {
		return x * rhs.x + y * rhs.y;
	}
	Vector2 operator+(const Vector2 &rhs) const {
		return Vector2(*this) += rhs;
	}
	Vector2 operator-(const Vector2 &rhs) const {
		return Vector2(*this) -= rhs;
	}
	Vector2& operator*=(const T &rhs) {
		x *= rhs;
		y *= rhs;
		return *this;
	}
	Vector2 operator*(const T &rhs) const {
		return Vector2(*this) *= rhs;
	}
	Vector2& operator/=(const T &rhs) {
		x /= rhs;
		y /= rhs;
		return *this;
	}
	Vector2 operator/(const T &rhs) const {
		return Vector2(*this) /= rhs;
	}
	bool operator==(const Vector2 &rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const Vector2 &rhs) const {
		return !(*this == rhs);
	}
	Vector2 Rotated(float radians) const {
		const float sinTheta = sin(radians);
		const float cosTheta = cos(radians);
		return { cosTheta * x - sinTheta * y, sinTheta * x + cosTheta * y };
	}
	Vector2 Rotated90Clockwise() const {
		return { y, -x };
	}
	Vector2 Rotated90CounterClockwise() const {
		return -Rotated90Clockwise();
	}
public:
	T x;
	T y;
public:
	static const Vector2 ZERO;
	static const Vector2 LEFT;
	static const Vector2 RIGHT;
	static const Vector2 UP;
	static const Vector2 DOWN;
};

typedef Vector2<float> Vec2;
typedef Vector2<double> Ved2;
typedef Vector2<int> Vei2;

template<typename T>
const Vector2<T> Vector2<T>::ZERO = { (T)0, (T)0 };
template<typename T>
const Vector2<T> Vector2<T>::LEFT = { (T)-1, (T)0 };
template<typename T>
const Vector2<T> Vector2<T>::RIGHT = { (T)1, (T)0 };
template<typename T>
const Vector2<T> Vector2<T>::UP = { (T)0, (T)-1 };
template<typename T>
const Vector2<T> Vector2<T>::DOWN = { (T)0, (T)1 };