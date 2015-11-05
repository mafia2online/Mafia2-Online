/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CQuaternion.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#ifndef M_PI
#define M_PI			3.14159265358979323846f
#endif

#include	"CVector3.h"
#include	<math.h>

class Quaternion
{

public:

	float fX;
	float fY;
	float fZ;
	float fW;

	Quaternion ( void )
	{
		fW = fX = fY = fZ = 0.0f;
	}

	Quaternion ( float _fW, float _fX, float _fY, float _fZ )
	{
		fW = _fW;
		fX = _fX;
		fY = _fY;
		fZ = _fZ;
	}

	Quaternion ( float _fX, float _fY, float _fZ )
	{
		// Calculate the cos angles
		float fCosZ = cosf( 0.5 * _fZ );
		float fCosY = cosf( 0.5 * _fY );
		float fCosX = cosf( 0.5 * _fX );

		// Calculate the sin angles
		float fSinZ = sinf( 0.5 * _fZ );
		float fSinY = sinf( 0.5 * _fY );
		float fSinX = sinf( 0.5 * _fX );

		// Compute the quaternion
		fW = fCosZ * fCosY * fCosX + fSinZ * fSinY * fSinX;
		fZ = fCosZ * fCosY * fSinX - fSinZ * fSinY * fCosX;
		fY = fCosZ * fSinY * fCosX + fSinZ * fCosY * fSinX;
		fX = fSinZ * fCosY * fCosX - fCosZ * fSinY * fSinX;
	}

	Quaternion ( CVector3 vecEular )
	{
		// Calculate the cos angles
		float fCosZ = cosf( 0.5 * vecEular.fZ );
		float fCosY = cosf( 0.5 * vecEular.fY );
		float fCosX = cosf( 0.5 * vecEular.fX );

		// Calculate the sin angles
		float fSinZ = sinf( 0.5 * vecEular.fZ );
		float fSinY = sinf( 0.5 * vecEular.fY );
		float fSinX = sinf( 0.5 * vecEular.fX );

		// Compute the quaternion
		fW = fCosZ * fCosY * fCosX + fSinZ * fSinY * fSinX;
		fZ = fCosZ * fCosY * fSinX - fSinZ * fSinY * fCosX;
		fY = fCosZ * fSinY * fCosX + fSinZ * fCosY * fSinX;
		fX = fSinZ * fCosY * fCosX - fCosZ * fSinY * fSinX;
	}

	// ===================== Operators =====================

	Quaternion operator + (const Quaternion &q) const
	{
		return Quaternion( (fW + q.fW), (fX + q.fX), (fY + q.fY), (fZ + q.fZ) );
	}

	Quaternion operator - (const Quaternion &q) const
	{
		return Quaternion( (fW - q.fW), (fX - q.fX), (fY - q.fY), (fZ - q.fZ) );
	}

	Quaternion operator * (const Quaternion &q) const
	{
		return Quaternion( (fW * q.fW) - ((fX * q.fX) + (fY * q.fY) + (fZ * q.fZ)),
			(fY * q.fZ - fZ * q.fY + fW * q.fX + fX * q.fW),
			(fZ * q.fX - fX * q.fZ + fW * q.fY + fY * q.fW),
			(fX * q.fY - fY * q.fX + fW * q.fZ + fZ * q.fW) );
	}

	Quaternion operator * (float fScale) const
	{
		return Quaternion( (fW * fScale), (fX * fScale), (fY * fScale), (fZ * fScale) );
	}

	Quaternion operator / (const Quaternion &q) const
	{
		Quaternion p( q );
		p.invert();

		return (*this * p);
	}

	Quaternion operator / (float fScale) const
	{
		return Quaternion( (fW / fScale), (fX / fScale), (fY / fScale), (fZ / fScale) );
	}

	Quaternion operator /= (float fScale)
	{
		fW /= fScale;
		fX /= fScale;
		fY /= fScale;
		fZ /= fScale;
		return *this;
	}

	Quaternion operator -() const
	{
		return Quaternion( -fW, -fX, -fY, -fZ );
	}

	void operator = (const Quaternion &q)
	{
		fW = q.fW;
		fX = q.fX;
		fY = q.fY;
		fZ = q.fZ;
	}

	void operator += (const Quaternion &q)
	{
		fW += q.fW;
		fX += q.fX;
		fY += q.fY;
		fZ += q.fZ;
	}

	void operator -= (const Quaternion &q)
	{
		fW -= q.fW;
		fX -= q.fX;
		fY -= q.fY;
		fZ -= q.fZ;
	}

	void operator *= (const Quaternion &q)
	{
		fW *= q.fW;
		fX *= q.fX;
		fY *= q.fY;
		fZ *= q.fZ;
	}

	void operator /= (const Quaternion &q)
	{
		fW /= q.fW;
		fX /= q.fX;
		fY /= q.fY;
		fZ /= q.fZ;
	}

	// ===================== Functions =====================

	float length ( void )
	{
		return (float)sqrt( fW * fW + fX * fX + fY * fY + fZ * fZ );
	}

	float length_squared ( void )
	{
		return (float)( fW * fW + fX * fX + fY * fY + fZ * fZ );
	}

	void normalize ( void )
	{
		fW /= length();
		fX /= length();
		fY /= length();
		fZ /= length();
	}
	
	void conjugate ( void )
	{
		fX = -fX;
		fY = -fY;
		fZ = -fZ;
	}

	void invert ( void )
	{
		conjugate();
		*this /= length_squared();
	}

	float dot ( Quaternion q1, Quaternion q2 )
	{
		return (q1.fX * q2.fX + q1.fY * q2.fY + q1.fZ + q2.fZ + q1.fW * q2.fW);
	}

	void lerp ( Quaternion quatTarget, float delta )
	{
		*this = (*this * (1.0f - delta) + quatTarget * delta);
	}

	void slerp ( Quaternion quatTarget, float delta )
	{
		Quaternion quatTemp;
		float fDot = dot(*this, quatTarget);

		if( fDot < 0 )
		{
			fDot = -fDot;
			quatTemp = -quatTarget;
		}
		else
			quatTemp = quatTarget;

		if( fDot < 0.95f )
		{
			float fAngle = acosf( fDot );
			*this = (*this * sinf( fAngle * (1.0f - delta) ) + quatTemp * sinf( fAngle * delta )) / sinf( fAngle );
		}
		else
			lerp( quatTemp, delta );
	}

	void slerpNoInvert ( Quaternion quatTarget, float delta )
	{
		float fDot = dot( *this, quatTarget );

		if( fDot > -0.95f && fDot < 0.95f )
		{
			float fAngle = acosf( fDot );
			*this = (*this * sinf( fAngle * (1.0f - delta)) + quatTarget * sinf( fAngle * delta )) / sinf( fAngle );
		}
		else
			lerp( quatTarget, delta );
	}

	void toAxisAngle ( CVector3 * vecAxis, float * fAngle )
	{
		*fAngle = acosf( fW );
		float sinf_inv = (1.0f / sinf( *fAngle ));

		vecAxis->fX = (fX * sinf_inv);
		vecAxis->fY = (fY * sinf_inv);
		vecAxis->fZ = (fZ * sinf_inv);

		*fAngle *= 2;
	}

	CVector3 toEularAngles ( void ) const
	{
		float sqw = (fW * fW);
		float sqx = (fX * fX);
		float sqy = (fY * fY);
		float sqz = (fZ * fZ);

		CVector3 vecEular;
		vecEular.fX = atan2f( 2.f * (fX * fY + fZ * fW), sqx - sqy - sqz + sqw );
		vecEular.fY = asinf( -2.f * (fX * fZ - fY * fW) );
		vecEular.fZ = atan2f( 2.f * (fY * fZ + fX * fW), -sqx - sqy + sqz + sqw );

		return vecEular;
	}

};
