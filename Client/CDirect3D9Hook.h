/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CDirect3D9Hook.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CDirect3D9Hook
{

private:

	typedef	IDirect3D9 * (WINAPI * Direct3DCreate9_t)( UINT );
	static	Direct3DCreate9_t			m_pfnDirect3DCreate9;

	static	IDirect3D9 * WINAPI		Direct3DCreate9_Hook(UINT SDKVersion);

	static	bool					m_bInstalled;

public:

	CDirect3D9Hook( ) { };
	~CDirect3D9Hook( ) { };

	static	bool					Install( );
	static	void					Uninstall( );

};