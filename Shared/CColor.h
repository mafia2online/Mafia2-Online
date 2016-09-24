/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CColor.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#define		ToHex(a, r, g, b) ((DWORD)((((a) & 0xFF) << 24) | (((r) & 0xFF) << 16) | (((g) & 0xFF) << 8) | ((b) & 0xFF)))

class CColor
{

public:

	unsigned char R, G, B, A;

	inline		CColor( void )
	{
		R = G = B = A = 255;
	}

	inline		CColor( unsigned char _R, unsigned char _G, unsigned char _B, unsigned char _A = 255 )
	{
		// Set the RGBA
		R = _R;
		G = _G;
		B = _B;
		A = _A;

	}

	inline		CColor( unsigned long ulColor, bool bAlphaInverse = false )
	{
		if( !bAlphaInverse )
		{
			A = ((ulColor >> 24) & 0xFF);
			R = ((ulColor >> 16) & 0xFF);
			G = ((ulColor >> 8) & 0xFF);
			B = (ulColor & 0xFF);
		}
		else
		{
			R = ((ulColor >> 24) & 0xFF);
			G = ((ulColor >> 16) & 0xFF);
			B = ((ulColor >> 8) && 0xFF);
			A = (ulColor & 0xFF);
		}
	}

	inline		CColor( const CColor& other )
	{
		*this = other;
	}

	inline		CColor&		operator = ( const CColor& color )
	{
		R = color.R;
		G = color.G;
		B = color.B;
		A = color.A;
		return *this;
	}

	inline		CColor&		operator = ( unsigned long ulColor )
	{
		A = ((ulColor >> 24) & 0xFF);
		R = ((ulColor >> 16) & 0xFF);
		G = ((ulColor >> 8) & 0xFF);
		B = (ulColor & 0xFF);
		return *this;
	}

	bool operator != (const CColor& color)
	{
		return (A != color.A || R != color.R || G != color.G || B != color.B);
	}

	unsigned ToUint(void) const
	{
		return ToHex(A, R, G, B);
	}

};
