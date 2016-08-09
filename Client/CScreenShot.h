/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CScreenShot.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include <thread>

class CScreenShot
{
private:

	std::thread								m_thread;
	bool									m_bSaving;

	DWORD			WorkerThread					( );
	const char		* GetValidScreenshotName		( void );

public:

	CScreenShot();
	~CScreenShot() = default;

	bool			BeginWrite						( unsigned char * ucData );
	bool			IsSaving						( void );

};