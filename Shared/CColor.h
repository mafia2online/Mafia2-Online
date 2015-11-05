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

	int R, G, B, A;
	DWORD dwHexColor;

	inline		CColor( void )
	{
		// Set the RGBA
		R = G = B = A = 255;

		// Set the hex
		dwHexColor = ToHex(A, R, G, B);
	}

	inline		CColor( int _R, int _G, int _B, int _A = 255 )
	{
		// Set the RGBA
		R = _R;
		G = _G;
		B = _B;
		A = _A;

		// Set the hex
		dwHexColor = ToHex(A, R, G, B);
	}

	inline		CColor( unsigned long ulColor, bool bAlphaInverse = false )
	{
		// Is the colour alpha inversed?
		if( !bAlphaInverse )
		{
			// Set the RGBA
			A = ((ulColor >> 24) & 0xFF);
			R = ((ulColor >> 16) & 0xFF);
			G = ((ulColor >> 8) & 0xFF);
			B = (ulColor & 0xFF);
		
			// Set the hex
			dwHexColor = ulColor;
		}
		else
		{
			// Set the RGBA
			R = ((ulColor >> 24) & 0xFF);
			G = ((ulColor >> 16) & 0xFF);
			B = ((ulColor >> 8) && 0xFF);
			A = (ulColor & 0xFF);

			// Set the hex
			dwHexColor = ToHex(A, R, G, B);
		}
	}

	inline		CColor( const CColor& other )
	{
		*this = other;
	}

	inline		CColor&		operator = ( const CColor& color )
	{
		// Set the RGBA
		R = color.R;
		G = color.G;
		B = color.B;
		A = color.A;

		// Set the hex
		dwHexColor = color.dwHexColor;
		return *this;
	}

	inline		CColor&		operator = ( unsigned long ulColor )
	{
		// Set the RGBA
		A = ((ulColor >> 24) & 0xFF);
		R = ((ulColor >> 16) & 0xFF);
		G = ((ulColor >> 8) & 0xFF);
		B = (ulColor & 0xFF);

		// Set the hex
		dwHexColor = ulColor;
		return *this;
	}

	bool operator != (const CColor& color)
	{
		return (A != color.A || R != color.R || G != color.G || B != color.B);
	}

};