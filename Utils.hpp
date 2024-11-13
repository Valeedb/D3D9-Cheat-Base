#pragma once
#include "Common.hpp"

struct Vec2
{
	inline void Clear( )
	{
		x = y = 0.f;
	}

	float x, y;
};

struct Vec3
{
	inline void Clear( )
	{
		x = y = z = 0.f;
	}

	Vec3 operator+( const Vec3& other ) const
	{
		return Vec3{ x + other.x, y + other.y, z + other.z };
	}

	Vec3 operator-( const Vec3& other ) const
	{
		return Vec3{ x - other.x, y - other.y, z - other.z };
	}

	Vec3 operator*( float scalar ) const
	{
		return Vec3{ x * scalar, y * scalar, z * scalar };
	}

	Vec3 operator+( float value ) const
	{
		return Vec3{ x + value, y + value, z + value };
	}

	Vec3 operator-( float value ) const
	{
		return Vec3{ x - value, y - value, z - value };
	}

	Vec3& operator+=( const Vec3& other )
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vec3& operator-=( const Vec3& other )
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vec3& operator+=( float value )
	{
		x += value;
		y += value;
		z += value;
		return *this;
	}

	Vec3& operator-=( float value )
	{
		x -= value;
		y -= value;
		z -= value;
		return *this;
	}

	float x, y, z;
};

struct Color
{
	inline D3DCOLOR GetD3DColor( ) const
	{
		return D3DCOLOR_RGBA( r, g, b, a );
	}

	uint8_t r, g, b, a;
};

namespace Utils
{
	uint8_t* FindPattern( HMODULE hHandle, const char* szPattern, const char* szMask );
}
