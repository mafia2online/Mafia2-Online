/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUI.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	<CEGUI.h>
#include	<renderers/directx9GUIRenderer/d3d9renderer.h>
#include	<renderers/directx9GUIRenderer/d3d9texture.h>

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

class CGUI
{

private:

	//
	IDirect3DDevice9						* m_pDevice;
	IDirectInput8							* m_pInput;
	IDirectInputDevice8						* m_pInputDevice;

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

	void									SetupInputDevice( void );
	void									SetupGUI( void );

	void									SetCursorVisible( bool bVisible );
	bool									IsCursorVisible( void );

	void									Render( void );
	void									ProcessMouse( void );

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