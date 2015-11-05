/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : InterfaceCommon.h
*
***************************************************************/

#pragma once

#include	<math.h>

#if defined(WIN32) || defined(_WIN32)
#include	<windows.h>
#else
#include	<string.h>
typedef bool BOOL;
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef char *PCHAR;
typedef void *PVOID;
typedef unsigned char BYTE;
#define TRUE (1)
#define FALSE (0)
#endif

typedef unsigned short EntityId;

struct CVector3
{
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

	void operator -= (float fRight)
	{
		fX -= fRight;
		fY -= fRight;
		fZ -= fRight;
	}

	void operator *= (float fRight)
	{
		fX *= fRight;
		fY *= fRight;
		fZ *= fRight;
	}

	void operator /= (float fRight)
	{
		fX /= fRight;
		fY /= fRight;
		fZ /= fRight;
	}

	bool operator != (const CVector3& vecVector)
	{
		return (fX != vecVector.fX && fY != vecVector.fY && fZ != vecVector.fZ);
	}
};

#include	"CPlayerManagerInterface.h"
#include	"CBlipManagerInterface.h"
#include	"CVehicleManagerInterface.h"
#include	"CEventsInterface.h"
#include	"CBanManagerInterface.h"
#include	"CResourceManagerInterface.h"

#include	"CSquirrelInterface.h"