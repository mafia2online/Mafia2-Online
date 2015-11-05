/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CDirect3D9Hook.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

bool								CDirect3D9Hook::m_bInstalled = false;
CDirect3D9Hook::Direct3DCreate9_t	CDirect3D9Hook::m_pfnDirect3DCreate9 = NULL;

IDirect3D9 * WINAPI CDirect3D9Hook::Direct3DCreate9_Hook( UINT SDKVersion )
{
	// Call the real device
	IDirect3D9 * pD3D = m_pfnDirect3DCreate9( SDKVersion );

	// If the call was successful return our proxy device
	if( pD3D )
	{
		return new CDirect3D9Proxy( pD3D );
	}

	// Something bad happend
	TerminateProcess( GetCurrentProcess(), 0 );

	return NULL;
}

bool CDirect3D9Hook::Install( )
{
	if( !m_bInstalled )
	{
		m_pfnDirect3DCreate9 = ( Direct3DCreate9_t ) CPatcher::InstallDetourPatch( "d3d9.dll", "Direct3DCreate9", (DWORD)Direct3DCreate9_Hook );

		return m_bInstalled = true;
	}

	return false;
}

void CDirect3D9Hook::Uninstall( )
{
	if( m_bInstalled )
	{
		CPatcher::UninstallDetourPatch( m_pfnDirect3DCreate9, (DWORD)Direct3DCreate9_Hook );
	}
}