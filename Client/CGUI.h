/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUI.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

// Silence the build messages about that CEGUI is undefining min/max macro.
#undef		min
#undef		max

#include	<CEGUI.h>
#include	<renderers/directx9GUIRenderer/d3d9renderer.h>
#include	<renderers/directx9GUIRenderer/d3d9texture.h>

#include "CDirectInput8Hook.h"

enum eMouseType
{
	MOVE = 0,
	POSITION,
	SCROLL,
	BUTTON_DOWN,
	BUTTON_UP
};

enum eMouseButton
{
	NONE = 0,
	LEFT,
	MIDDLE,
	RIGHT
};

class CGUI_Impl;
class CDownloadProgress;
class CMainMenu;
class CServerBrowser;
class CMenuSettings;

#include "ExplicitSingleton.h"

class CGUI : public ExplicitSingleton<CGUI>
{

private:

	//
	IDirect3DDevice9						* m_pDevice;

	//
	CGUI_Impl								* m_pGUI;
	CDownloadProgress						* m_pDownloadProgress;
	CMainMenu								* m_pMainMenu;
	CServerBrowser							* m_pServerBrowser;
	CMenuSettings							* m_pSettings;

	// Mouse input
	DIMOUSESTATE2							m_MouseState;
	RECT									m_vecCursorPosition;
	BYTE									m_byteButtonClicked[8];

	//
	bool									m_bPreviousCameraState;

public:
											CGUI( IDirect3DDevice9 * pDevice );
											~CGUI( void );

	void									SetupGUI( void );

	void									SetCursorVisible( bool bVisible );
	bool									IsCursorVisible( void ) const;

	static bool									DeleteAllClientScriptGUI( void );

	void									Render( void );
	void									ProcessMouse( IDirectInputDevice8 *pMouseDevice );

	void									OnDeviceLost( void );
	void									OnDeviceReset( void );

	bool									InputGoesToGUI( void );
	unsigned long							TranslateFromScanCode( DWORD dwCharacter );
	bool									ProcessInput( UINT uMsg, WPARAM wParam, LPARAM lParam );

	CGUI_Impl								* GetCEGUI( void ) { return m_pGUI; }
	CDownloadProgress						* GetDownloadProgress( void ) { return m_pDownloadProgress; }
	CMainMenu								* GetMainMenu( void ) { return m_pMainMenu; }
	CServerBrowser							* GetServerBrowser( void ) { return m_pServerBrowser; }
	CMenuSettings							* GetSettings( void ) { return m_pSettings; }

};