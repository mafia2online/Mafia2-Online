/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : FNV.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#ifdef _WIN32
#include <Windows.h>
#endif
#include "FNV.h"

namespace FNV
{
	unsigned int Hash32( const char * szString )
	{
		unsigned int hash = (unsigned int)FNV_32_INIT;

		for( unsigned int i = 0; i < strlen(szString); i++ )
		{
			hash *= (unsigned int)FNV_32_PRIME;
			hash ^= szString[i];
		}

		return hash;
	}

	unsigned __int64 Hash64( const char * szString )
	{
		unsigned __int64 hash = FNV_64_INIT;

		for( unsigned __int64 i = 0; i < strlen(szString); i++ )
		{
			hash *= FNV_64_PRIME;
			hash ^= szString[i];
		}

		return hash;
	}

	long Hash32Web( const char * szString )
	{
		long hash = (long)FNV_32_INIT;

		for( long i = 0; i < strlen(szString); i++ )
		{
			hash += (hash << 1) + (hash << 4) + (hash << 7) + (hash << 8) + (hash << 24);
			hash ^= szString[i];
		}

		return hash;
	}
};