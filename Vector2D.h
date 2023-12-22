#pragma once

#include <iostream>
#include <cmath>

class Vector2D
{
public:
	Vector2D() : m_x(0.f), m_y(0.f) {}

	Vector2D(const float x, const float y) : m_x(x), m_y(y) {}

	const float getX() { return m_x; }
	const float getY() { return m_y; }

	void setX(const float x) { m_x = x; }
	void setY(const float y) { m_y = y; }

	float length() 
	{ 
		return (float)sqrt(m_x * m_x + m_y * m_y); 
	}

	Vector2D operator+(const Vector2D& v2) const 
	{ 
		return Vector2D(m_x + v2.m_x, m_y + v2.m_y); 
	}

	Vector2D operator+(const float scalar) const
	{
		return Vector2D(m_x + scalar, m_y + scalar);
	}

	friend Vector2D& operator+=(Vector2D& v1, const Vector2D& v2)
	{
		v1.m_x += v2.m_x;
		v1.m_y += v2.m_y;

		return v1;
	}

	Vector2D operator-(const Vector2D& v2) const
	{ 
		return Vector2D(m_x - v2.m_x, m_y - v2.m_y); 
	}

	Vector2D operator-(const float scalar) const
	{
		return Vector2D(m_x - scalar, m_y - scalar);
	}

	friend Vector2D& operator-=(Vector2D& v1, const Vector2D& v2)
	{
		v1.m_x -= v2.m_x;
		v1.m_y -= v2.m_y;

		return v1;
	}


 	Vector2D operator*(const float scalar)
 	{
 		return Vector2D(m_x * scalar, m_y * scalar);
 	}

	Vector2D& operator*=(const float scalar)
	{
		m_x *= scalar;
		m_y *= scalar;

		return *this;
	}

	Vector2D operator/(const float scalar)
	{
		return Vector2D(m_x / scalar, m_y / scalar);
	}

	Vector2D& operator/=(const float scalar)
	{
		m_x /= scalar;
		m_y /= scalar;

		return *this;
	}

	bool operator!=(const Vector2D& vector) const
	{
		return m_x != vector.m_x || m_y != vector.m_y;
	}

	bool operator==(const Vector2D& vector) const
	{
		return m_x == vector.m_x && m_y == vector.m_y;
	}

	bool operator<(const Vector2D& vector) const
	{
		return m_x < vector.m_x && m_y < vector.m_y;
	}

	bool operator>(const Vector2D& vector) const
	{
		return m_x > vector.m_x && m_y > vector.m_y;
	}

 	static float mag(const Vector2D rhs) {
 		return sqrt(dot(rhs, rhs));
 	}

 	Vector2D operator*(float const& num) const
 	{
 		return Vector2D(m_x * num, m_y * num);
 	}

 	static Vector2D norm(const Vector2D& lhs) 
 	{
 		return (lhs*(1.f / (mag(lhs))));
 	}

 	static float dot(const Vector2D& lhs, const Vector2D& rhs) 
	{
 		return lhs.m_x * rhs.m_x + lhs.m_y * rhs.m_y;
 	}

	static Vector2D cross(const Vector2D& vec1, const Vector2D& vec2)
	{
		Vector2D resVec;
		resVec.m_x = ((vec1.m_y * vec2.m_x) - (vec1.m_x * vec2.m_y));
		resVec.m_y = ((vec1.m_x * vec2.m_y) - (vec1.m_x * vec2.m_y));

		return resVec;
	}

	Vector2D operator-() const
	{
		return Vector2D(-m_x, -m_y);
	}

private:
	float m_x;
	float m_y;
};