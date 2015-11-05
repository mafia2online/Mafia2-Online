/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CZlib.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include <stdio.h>
#include "CString.h"

class CZlib
{

public:

	static	int			Inflate( FILE * pSource, FILE * pDestination );
	static	int			Decompress( String strFileName, String strOutput );

};