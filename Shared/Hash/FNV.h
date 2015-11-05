/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : FNV.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#ifndef _FNV_H
#define	_FNV_H

#pragma once

#define		FNV_64_INIT				0xCBF29CE484222325
#define		FNV_64_PRIME			0x100000001B3
#define		FNV_32_INIT				0x2166136261
#define		FNV_32_PRIME			0x1000193

namespace FNV
{
	unsigned int					Hash32				( const char * szString );
	unsigned __int64				Hash64				( const char * szString );
	long							Hash32Web			( const char * szString );
};

#endif