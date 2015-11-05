/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CScreenShot.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"../Shared/Threading/CThread.h"

class CScreenShot
{

private:

	static	bool											m_bSaving;

	static	DWORD			WorkerThread					( LPVOID lpParam );
	static	const char		* GetValidScreenshotName		( void );

public:

	static	bool			BeginWrite						( unsigned char * ucData );
	static	bool			IsSaving						( void );

};