/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CMaths.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	<math.h>
#include	<limits>
#include	"CVector3.h"
#include	"CQuaternion.h"

#ifdef max
#undef max
#endif

namespace Math
{

	static float WrapAround( float fValue, float fHigh )
	{
		return fValue - (fHigh * floor((float)(fValue / fHigh)));
	}

	template<typename T>
	static T Lerp( const T& start, float fAlpha, const T& end )
	{
		return (end - start) * fAlpha + start;
	}

	template<class T>
	static T Clamp( const T& min, const T& a, const T& max )
	{
		if( a < min )
			return min;

		if( a > max )
			return max;

		return a;
	}

	static inline float GetOffsetDegrees( float a, float b )
	{
		float c = ( b > a ) ? b - a : 0.0f - ( a - b );
		if ( c > 180.0f )
			c = 0.0f - ( 360.0f - c );
		else if ( c <= -180.0f )
			c = ( 360.0f + c );
		return c;
	}

	static inline CVector3 GetOffsetDegrees ( CVector3 a, CVector3 b )
	{
		return CVector3 ( GetOffsetDegrees ( a.fX, b.fX ), GetOffsetDegrees ( a.fY, b.fY ), GetOffsetDegrees ( a.fZ, b.fZ ) );
	}

	static const float Unlerp( const double fStart, const double fPos, const double fEnd )
	{
		if( fStart == fEnd )
			return 1.0f;

		return (float)((fPos - fStart) / (fEnd - fStart));
	}

	static const float UnlerpClamped( const double fStart, const double fPos, const double fEnd )
	{
		return Clamp( 0.0f, Unlerp( fStart, fPos, fEnd ), 1.0f );
	}

	static float GetDistanceBetweenPoints( float fX, float fY, float fXX, float fYY )
	{
		float newx = (fXX - fX);
		float newy = (fYY - fY);

		return sqrt( newx * newx + newy * newy );
	}

	static float GetDistanceBetweenPoints( float fX, float fY, float fZ, float fXX, float fYY, float fZZ )
	{
		float newx = (fXX - fX);
		float newy = (fYY - fY);
		float newz = (fZZ - fZ);

		return sqrt( newx * newx + newy * newy + newz * newz );
	}

	static float GetDistanceBetweenPoints( CVector3 vecPos, CVector3 vecPos2 )
	{
		return GetDistanceBetweenPoints( vecPos.fX, vecPos.fY, vecPos.fZ, vecPos2.fX, vecPos2.fY, vecPos2.fZ );
	}

	static bool	IsPointInArea( CVector3 vecArea, CVector3 vecArea2, CVector3 vecPoint )
	{
		if( (vecPoint.fX >= vecArea.fX && vecPoint.fX <= vecArea2.fX) && (vecPoint.fY >= vecArea.fY && vecPoint.fY <= vecArea2.fY) && (vecPoint.fZ >= vecArea.fX && vecPoint.fZ <= vecArea2.fZ) )
			return true;

        return false;
	}

	static bool IsPointInArea( float x, float y, float xx, float yy, float pointX, float pointY )
	{
		if( (pointX >= x && pointX <= xx) && (pointY >= y && pointY <= yy) )
			return true;

        return false;
	}

	static bool IsPointInBall( CVector3 vecCircle, float fRadius, CVector3 vecPoint )
	{
		if( GetDistanceBetweenPoints( vecCircle, vecPoint ) < fRadius )
			return true;

		return false;
	}

	static bool IsPointInBall( float x, float y, float fRadius, float pointX, float pointY )
	{
		if( GetDistanceBetweenPoints( x, y, pointX, pointY ) < fRadius )
			return true;

		return false;
	}

	static bool	IsPointInTube( CVector3 vecTube, float fTubeHeight, float fTubeRadius, CVector3 vecPoint )
	{
		if( GetDistanceBetweenPoints( vecTube, vecPoint ) < fTubeRadius && vecPoint.fZ < (vecTube.fZ + fTubeHeight) && vecPoint.fZ >= vecTube.fZ )
			return true;

		return false;
	}

	template < typename T >
	static bool IsInifity ( const T &value )
	{
		T max_value = std::numeric_limits < T >::max ();
		T min_value = - max_value;
		return ! ( min_value <= value && value <= max_value );
	}

	template < typename T >
	static bool IsNAN ( const T &value )
	{
		return (value != value);
	}

	static bool IsValidFloat ( float fValue )
	{
		return (!IsInifity ( fValue ) && !IsNAN ( fValue ) );
	}

	static bool IsValidVector ( CVector3 vector )
	{
		return (IsValidFloat( vector.fX ) && IsValidFloat( vector.fY ) && IsValidFloat( vector.fZ ));
	}

};