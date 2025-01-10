#pragma once
#include <imgui.h>

class Vector2 final
{
public:
	Vector2() = default;
	Vector2(float x, float y)
		: x(x), y(y)
	{}
	Vector2(int x, int y)
		: Vector2(static_cast<float>(x), static_cast<float>(y))
	{}

	operator ImVec2()
	{
		return { x, y };
	}
	
	float x{}, y{};
	
	bool empty()
	{
		return x == 0.f && y == 0.f;
	}

	Vector2 operator*(const float amount)
	{
		return { x * amount, y * amount };
	}
	Vector2 operator*(const Vector2 vec)
	{
		return { x * vec.x, y * vec.y };
	}

	Vector2 operator/(const float amount)
	{
		return { x / amount, y / amount };
	}
	Vector2 operator/(const Vector2 vec)
	{
		return { x / vec.x, y / vec.y };
	}

	Vector2 operator+(const float amount)
	{
		return { x + amount, y + amount };
	}
	Vector2 operator+(const Vector2 vec)
	{
		return { x + vec.x, y + vec.y };
	}

	Vector2 operator-(const float amount)
	{
		return { x - amount, y - amount };
	}
	Vector2 operator-(const Vector2 vec)
	{
		return { x - vec.x, y - vec.y };
	}

	Vector2& operator+=(const float& amount)
	{
		x += amount;
		y += amount;
		return *this;
	}
	Vector2& operator+=(const Vector2& vec)
	{
		x += vec.x;
		y += vec.y;
		return *this;
	}

	Vector2& operator-=(const float& amount)
	{
		x -= amount;
		y -= amount;
		return *this;
	}
	Vector2& operator-=(const Vector2& vec)
	{
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	Vector2& operator/=(const float& amount)
	{
		x /= amount;
		y /= amount;
		return *this;
	}
	Vector2& operator/=(const Vector2& vec)
	{
		x /= vec.x;
		y /= vec.y;
		return *this;
	}

	Vector2& operator*=(const float& amount)
	{
		x *= amount;
		y *= amount;
		return *this;
	}
	Vector2& operator*=(const Vector2& vec)
	{
		x *= vec.x;
		y *= vec.y;
		return *this;
	}

	bool operator==(const Vector2& vec)
	{
		return vec.x == x && vec.y == y;
	}
	bool operator!=(const Vector2& vec)
	{
		return vec.x != x || vec.y != y;
	}
};
class Vector3
{
public:
	Vector3() = default;
	Vector3(float x, float y, float z)
		: x(x), y(y), z(z)
	{}
public:
	float x{}, y{}, z{};
public:
	bool empty()
	{
		return x == 0.f && y == 0.f && z == 0.f;
	}
	bool valid()
	{
		return x == 0.f || y == 0.f || z == 0.f;
	}
	void set(float sx, float sy, float sz)
	{
		x = sx;
		y = sy;
		z = sz;
	}
public:
	Vector3 operator*(const float amount) 
	{
		return { x * amount, y * amount, z * amount };
	}
	Vector3 operator*(const Vector3 vec)
	{
		return { x * vec.x, y * vec.y, z * vec.z };
	}

	Vector3 operator/(const float amount)
	{
		return { x / amount, y / amount, z / amount };
	}
	Vector3 operator/(const Vector3 vec)
	{
		return { x / vec.x, y / vec.y, z / vec.z };
	}

	Vector3 operator+(const float amount)
	{
		return { x + amount, y + amount, z + amount };
	}
	Vector3 operator+(const Vector3 vec)
	{
		return { x + vec.x, y + vec.y, z + vec.z };
	}

	Vector3 operator-(const float amount)
	{
		return { x - amount, y - amount, z - amount };
	}
	Vector3 operator-(const Vector3 vec)
	{
		return { x - vec.x, y - vec.y, z - vec.z };
	}

	Vector3& operator+=(const float& amount)
	{
		x += amount;
		y += amount;
		z += amount;
		return *this;
	}
	Vector3& operator+=(const Vector3& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	Vector3& operator-=(const float& amount)
	{
		x -= amount;
		y -= amount;
		z -= amount;
		return *this;
	}
	Vector3& operator-=(const Vector3& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	Vector3& operator/=(const float& amount)
	{
		x /= amount;
		y /= amount;
		z /= amount;
		return *this;
	}
	Vector3& operator/=(const Vector3& vec)
	{
		x /= vec.x;
		y /= vec.y;
		z /= vec.z;
		return *this;
	}

	Vector3& operator*=(const float& amount)
	{
		x *= amount;
		y *= amount;
		z *= amount;
		return *this;
	}
	Vector3& operator*=(const Vector3& vec)
	{
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
		return *this;
	}

	bool operator==(const Vector3& vec)
	{
		return vec.x == x && vec.y == y && vec.z == z;
	}

	bool operator!=(const Vector3& vec)
	{
		return vec.x != x || vec.y != y || vec.z != z;
	}
};
class Vector4 final
{
public:
	Vector4() = default;
	Vector4(float x, float y, float z, float w) 
		: x(x), y(y), z(z), w(w)
	{}
public:
	float x{}, y{}, z{}, w{};
public:
	bool empty()
	{
		return x == 0.f && y == 0.f && z == 0.f && w == 0.f;
	}
public:
	Vector4 operator*(const float amount)
	{
		return { x * amount, y * amount, z * amount, w * amount };
	}
	Vector4 operator*(const Vector4 vec)
	{
		return { x * vec.x, y * vec.y, z * vec.z, w * vec.w };
	}

	Vector4 operator/(const float amount) 
	{
		return { x / amount, y / amount, z / amount, w / amount };
	}
	Vector4 operator/(const Vector4 vec)
	{
		return { x / vec.x, y / vec.y, z / vec.z, w / vec.w };
	}

	Vector4 operator+(const float amount)
	{
		return { x + amount, y + amount, z + amount, w + amount };
	}
	Vector4 operator+(const Vector4 vec)
	{
		return { x + vec.x, y + vec.y, z + vec.z, w + vec.w };
	}

	Vector4 operator-(const float amount)
	{
		return { x - amount, y - amount, z - amount, w - amount };
	}
	Vector4 operator-(const Vector4 vec)
	{
		return { x - vec.x, y - vec.y, z - vec.z, w - vec.w };
	}

	Vector4& operator+=(const float& amount)
	{
		x += amount;
		y += amount;
		z += amount;
		w += amount;
		return *this;
	}
	Vector4& operator+=(const Vector4& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	Vector4& operator-=(const float& amount)
	{
		x -= amount;
		y -= amount;
		z -= amount;
		w -= amount;
		return *this;
	}
	Vector4& operator-=(const Vector4& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		w -= vec.w;
		return *this;
	}

	Vector4& operator/=(const float& amount)
	{
		x /= amount;
		y /= amount;
		z /= amount;
		w /= amount;
		return *this;
	}
	Vector4& operator/=(const Vector4& vec)
	{
		x /= vec.x;
		y /= vec.y;
		z /= vec.z;
		w /= vec.w;
		return *this;
	}

	Vector4& operator*=(const float& amount)
	{
		x *= amount;
		y *= amount;
		z *= amount;
		w *= amount;
		return *this;
	}
	Vector4& operator*=(const Vector4& vec)
	{
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
		w *= vec.w;
		return *this;
	}

	bool operator==(const Vector4& vec)
	{
		return vec.x == x && vec.y == y && vec.z == z && vec.w != w;
	}

	bool operator!=(const Vector4& vec)
	{
		return vec.x != x || vec.y != y || vec.z != z || vec.w != w;
	}
};