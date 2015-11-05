/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CVector3.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#ifndef M_PI
#define M_PI			3.14159265358979323846f
#endif

#include	<limits>

#ifdef max
#undef max
#endif

template<class T>
static T Clamp_( const T& min, const T& a, const T& max )
{
	if( a < min )
		return min;

	if( a > max )
		return max;

	return a;
}

class Vector2
{

public:

	float fX, fY;

	Vector2()
	{
		fX = fY = 0.0f;
	}

	Vector2(float _fX, float _fY)
	{
		fX = _fX;
		fY = _fY;
	}

};

class CVector3
{

public:

	float fX;
	float fY;
	float fZ;

	CVector3()
	{
		fX = fY = fZ = 0.0f;
	}

	CVector3(float _fX, float _fY, float _fZ)
	{
		fX = _fX; fY = _fY; fZ = _fZ;
	}

	bool IsEmpty() const
	{
		return (fX == 0 && fY == 0 && fZ == 0);
	}

	float Length() const
	{
		return sqrt((fX * fX) + (fY * fY) + (fZ * fZ));
	}

	void Normalize( void )
	{
		float fLength = (1.0f / Length());
		fX *= fLength;
		fY *= fLength;
		fZ *= fLength;
	}

	void Clamp( float fMin, float fMax )
	{
		fX = Clamp_< const float >( fMin, fX, fMax );
		fY = Clamp_< const float >( fMin, fY, fMax );
		fZ = Clamp_< const float >( fMin, fZ, fMax );
	}

	void ToRadians( void )
	{
		fX = (fX * (M_PI / 180.0f));
		fY = (fY * (M_PI / 180.0f));
		fZ = (fZ * (M_PI / 180.0f));
	}

	void FromRadians( void )
	{
		fX = (fX * (180.0f / M_PI));
		fY = (fY * (180.0f / M_PI));
		fZ = (fZ * (180.0f / M_PI));
	}

	CVector3 operator+ (const CVector3& vecRight) const
	{
		return CVector3(fX + vecRight.fX, fY + vecRight.fY, fZ + vecRight.fZ);
	}

	CVector3 operator+ (float fRight) const
	{
		return CVector3(fX + fRight, fY + fRight, fZ + fRight);
	}

	CVector3 operator- (const CVector3& vecRight) const
	{
		return CVector3(fX - vecRight.fX, fY - vecRight.fY, fZ - vecRight.fZ);
	}

	CVector3 operator- (float fRight) const
	{
		return CVector3(fX - fRight, fY - fRight, fZ - fRight);
	}

	CVector3 operator* (const CVector3& vecRight) const
	{
		return CVector3(fX * vecRight.fX, fY * vecRight.fY, fZ * vecRight.fZ);
	}

	CVector3 operator* (float fRight) const
	{
		return CVector3(fX * fRight, fY * fRight, fZ * fRight);
	}

	CVector3 operator/ (const CVector3& vecRight) const
	{
		return CVector3(fX / vecRight.fX, fY / vecRight.fY, fZ / vecRight.fZ);
	}

	CVector3 operator/ (float fRight) const
	{
		return CVector3(fX / fRight, fY / fRight, fZ / fRight);
	}

	CVector3 operator - () const
	{
		return CVector3(-fX, -fY, -fZ);
	}

	void operator += (float fRight)
	{
		fX += fRight;
		fY += fRight;
		fZ += fRight;
	}

	void operator += (const CVector3 vector)
	{
		fX += vector.fX;
		fY += vector.fY;
		fZ += vector.fZ;
	}

	void operator -= (float fRight)
	{
		fX -= fRight;
		fY -= fRight;
		fZ -= fRight;
	}

	void operator -= (const CVector3 vector)
	{
		fX -= vector.fX;
		fY -= vector.fY;
		fZ -= vector.fZ;
	}

	void operator *= (float fRight)
	{
		fX *= fRight;
		fY *= fRight;
		fZ *= fRight;
	}

	void operator *= (const CVector3 vector)
	{
		fX *= vector.fX;
		fY *= vector.fY;
		fZ *= vector.fZ;
	}

	void operator /= (float fRight)
	{
		fX /= fRight;
		fY /= fRight;
		fZ /= fRight;
	}

	void operator /= (const CVector3 vector)
	{
		fX /= vector.fX;
		fY /= vector.fY;
		fZ /= vector.fZ;
	}

	bool operator != (const CVector3& vecVector)
	{
		return (fX != vecVector.fX && fY != vecVector.fY && fZ != vecVector.fZ);
	}

};