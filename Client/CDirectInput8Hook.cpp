/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CDirectInput8Hook.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern CCore						* pCore;
bool								CDirectInput8Hook::m_bInstalled = false;
DirectInput8Create_t				CDirectInput8Hook::m_pDirectInput8Create = NULL;

HRESULT WINAPI DirectInput8Create__Hook( HINSTANCE hInst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter )
{
	// Call the real DirectInput8Create
	HRESULT hr = CDirectInput8Hook::m_pDirectInput8Create( hInst, dwVersion, riidltf, ppvOut, punkOuter );

	// If the call succeeded create our proxy device
	if( SUCCEEDED( hr ) )
	{
		IDirectInput8 * pInput = (IDirectInput8 *) *ppvOut;
		*ppvOut = new CDirectInput8Proxy( pInput );
	}

	return hr;
}

void CDirectInput8Hook::Install( )
{
	// Are we not already installed?
	if( !m_bInstalled )
	{
		// Install the detour patch
		m_pDirectInput8Create = ( DirectInput8Create_t ) CPatcher::InstallDetourPatch( "dinput8.dll", "DirectInput8Create", (DWORD)DirectInput8Create__Hook );

		// Mark as installed
		m_bInstalled = true;
	}
}

void CDirectInput8Hook::Uninstall( )
{
	// Are we installed?
	if( m_bInstalled )
	{
		// Uninstall the detour patch
		CPatcher::UninstallDetourPatch( m_pDirectInput8Create, (DWORD)DirectInput8Create__Hook );

		// Mark as uninstalled
		m_bInstalled = false;
	}
}