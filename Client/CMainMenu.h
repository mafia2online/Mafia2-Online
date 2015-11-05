/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CMainMenu.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CUpdater;
class CMainMenu
{

private:

	CGUI_Impl														* m_pGUI;
	bool															m_bVisible;

	int																m_iFadeAlpha;
	CGUIStaticImage_Impl											* m_pLogo;

	std::list< CGUIStaticImage_Impl* >								m_items;

	bool							OnQuickConnectClick				( CGUIElement_Impl * pElement );
	bool							OnDisconnectClick				( CGUIElement_Impl * pElement );
	bool							OnConnectClick					( CGUIElement_Impl * pElement );
	bool							OnRefreshClick					( CGUIElement_Impl * pElement );
	bool							OnSettingsClick					( CGUIElement_Impl * pElement );
	bool							OnQuitClick						( CGUIElement_Impl * pElement );

	bool							OnItemEnter						( CGUIElement_Impl * pElement );
	bool							OnItemLeave						( CGUIElement_Impl * pElement );

	// Store states
	bool															m_bPreviousMouseState;
	bool															m_bPreviousControlState;

	// Connect and disconnect items
	CGUIStaticImage_Impl											* m_pQuickConnect;
	CGUIStaticImage_Impl											* m_pConnect;
	CGUIStaticImage_Impl											* m_pDisconnect;
	CGUIStaticImage_Impl											* m_pRefresh;
	CGUIStaticImage_Impl											* m_pSettings;
	CGUIStaticImage_Impl											* m_pQuit;

	// Background fader
	unsigned long													m_ulLastFadeTime;
	bool															m_bFadeBackgroundOut;
	bool															m_bFadeBackgroundIn;
	unsigned int													m_uiBackgroundIndex;

public:

									CMainMenu						( CGUI_Impl * pGUI );
									~CMainMenu						( void );

	CGUIStaticImage_Impl			* CreateItem					( String strLocation, Vector2 vecPosition, bool bRelativePosition, Vector2 vecSize, GUI_CALLBACK pfnHandler );

	void							SetVisible						( bool bVisible );
	bool							IsVisible						( void ) { return m_bVisible; }

	void							Render							( void );

	void							OnScreenSizeChange				( float fX, float fY );
	void							OnDeviceLost					( void );

	void							SetConnectButtonVisible			( bool bVisible ) { if( m_pConnect ) { m_pConnect->SetVisible( bVisible ); } }
	bool							IsConnectionButtonVisible		( void ) { return (m_pConnect ? m_pConnect->IsVisible() : false); }

	void							SetDisconnectButtonVisible		( bool bVisible ) { if( m_pDisconnect ) { m_pDisconnect->SetVisible( bVisible ); } }
	bool							IsDisconnectionButtonVisible	( void ) { return (m_pDisconnect ? m_pDisconnect->IsVisible() : false); }

	void							LoadBackgroundImages			( float fWidth, float fHeight );
	void							ReleaseBackgroundImages			( void );

};