/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CWPMHook.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CWPMHook
{

private:

	static	bool										m_bInstalled;

public:

	static	bool				Install					( void );
	static	bool				Uninstall				( void );

};