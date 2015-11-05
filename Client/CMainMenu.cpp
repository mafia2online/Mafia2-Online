/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CMainMenu.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern CCore			* pCore;

#define	BG_SIZE_X		1920
#define	BG_SIZE_y		1080

#define	MAIN_MENU_HOLD_TIME				6000
#define	MAIN_MENU_FADE_TIME				1000
#define	MAIN_MENU_IMAGES				4

D3DXIMAGE_INFO							SrcInfo;
D3DCOLOR                                colourKey = 0xFFFF00FF;
LPDIRECT3DTEXTURE9                      pBackgroundTexture [ MAIN_MENU_IMAGES ];
LPD3DXSPRITE							pSprite;
D3DXVECTOR3                             ImagePos;

CMainMenu::CMainMenu( CGUI_Impl * pGUI )
{
	// Store the gui pointer
	m_pGUI = pGUI;

	// Reset
	m_bVisible = false;
	m_bPreviousMouseState = false;
	m_bPreviousControlState = false;
	m_pConnect = NULL;
	m_pDisconnect = NULL;
	m_bFadeBackgroundOut = false;
	m_bFadeBackgroundIn = false;
	m_uiBackgroundIndex = 2;
	m_iFadeAlpha = 255;

	// Get the resolution height
	float fX = pGUI->GetResolution().fX;
	float fY = pGUI->GetResolution().fY;

	// Create the logo
	m_pLogo = pGUI->CreateStaticImage();
	m_pLogo->LoadFromFile( "logo.png", SharedUtility::GetAbsolutePath( "data\\gui\\images" ) );
	m_pLogo->SetProperty( "InheritsAlpha", "False" );
	m_pLogo->SendToBack();
	m_pLogo->SetPosition( Vector2( 40.0f, 40.0f ) );
	m_pLogo->SetSize( Vector2( 249, 83 ) );
	m_pLogo->SetVisible( false );
	m_pLogo->SetAlwaysOnTop( true );

	// Create the sprite
	D3DXCreateSprite ( pCore->GetGraphics()->GetDevice(), &pSprite );

	// Load all background images
	LoadBackgroundImages ( fX, fY );
	
	// Get the item render position
	float renderX = (fX - 40.0f);

	// Create the quit button
	m_pQuit = CreateItem( "quit.png", Vector2( (renderX - 46), 61.5f ), false, Vector2( 46, 23 ), GUI_CALLBACK( &CMainMenu::OnQuitClick, this ) );
	renderX -= 86;

	// Create the settings button
	m_pSettings = CreateItem( "settings.png", Vector2( (renderX - 94), 61.5f ), false, Vector2( 94, 21 ), GUI_CALLBACK( &CMainMenu::OnSettingsClick, this ) );
	renderX -= 134;

	// Create the refresh button
	m_pRefresh = CreateItem( "refresh.png", Vector2( (renderX - 87), 61.5f ), false, Vector2( 87, 21 ), GUI_CALLBACK( &CMainMenu::OnRefreshClick, this ) );
	renderX -= 127;

	// Create the connect button
	m_pConnect = CreateItem( "connect.png", Vector2( (renderX - 90), 61.5f ), false, Vector2( 90, 21 ), GUI_CALLBACK( &CMainMenu::OnConnectClick, this ) );
	renderX -= 130;

	// Create the disconnect button
	m_pDisconnect = CreateItem( "disconnect.png", Vector2( (renderX - 125), 61.5f ), false, Vector2( 125, 21 ), GUI_CALLBACK( &CMainMenu::OnDisconnectClick, this ) );
	renderX -= 165;

	// Create the quick connect button
	m_pQuickConnect = CreateItem ( "quick_connect.png", Vector2( (renderX - 26), 61.5f ), false, Vector2( 26, 22 ), GUI_CALLBACK( &CMainMenu::OnQuickConnectClick, this ) );

	// Setup the serverbrowser UI
	pCore->GetGUI()->GetServerBrowser()->SetupUI( 40, 160, (fX - 80), (fY - 240) );
}

CMainMenu::~CMainMenu( void )
{
	// Delete the logo image
	SAFE_DELETE( m_pLogo );

	// Loop over each item
	for( std::list< CGUIStaticImage_Impl* >::iterator iter = m_items.begin(); iter != m_items.end(); iter++ )
	{
		// Get a pointer to the current item
		CGUIStaticImage_Impl * pItem = *iter;

		// Remove the current item from the list
		m_items.remove( pItem );

		// Delete the current item
		SAFE_DELETE( pItem );
	}
}

CGUIStaticImage_Impl * CMainMenu::CreateItem( String strLocation, Vector2 vecPosition, bool bRelativePosition, Vector2 vecSize, GUI_CALLBACK pfnHandler )
{
	// Create the label
	CGUIStaticImage_Impl * pItem = m_pGUI->CreateStaticImage();

	// Did the item fail to create?
	if( !pItem )
	{
		CLogFile::Printf( "Failed to create main menu item! (Location: %d, Callback: 0x%p)", strLocation.Get(), pfnHandler );
		return NULL;
	}

	// Set the label position
	pItem->SetPosition( vecPosition, bRelativePosition );

	// Set the label size
	pItem->SetSize( vecSize );

	// Set the item always on top
	pItem->SetAlwaysOnTop( true );

	// Set the properties
	pItem->SetProperty( "InheritsAlpha", "False" );

	// Set the image
	pItem->LoadFromFile( strLocation, SharedUtility::GetAbsolutePath( "data\\gui\\images" ) );

	// Set the handlers
	pItem->SetMouseEnterHandler( GUI_CALLBACK( &CMainMenu::OnItemEnter, this ) );
	pItem->SetMouseLeaveHandler( GUI_CALLBACK( &CMainMenu::OnItemLeave, this ) );

	// Set the click handler
	if( pfnHandler )
		pItem->SetClickHandler( pfnHandler );

	// Hide the item
	pItem->SetVisible( false );

	// Store the item
	m_items.push_back( pItem );
	return pItem;
}

bool CMainMenu::OnQuickConnectClick( CGUIElement_Impl * pElement )
{
	// Is their nickname not set?
	if( pCore->GetNick().IsEmpty() || !pCore->GetNick().Compare( "Player" ) )
	{
		// Bring the settings window to the foreground
		pCore->GetGUI()->GetSettings()->SetVisible( true );
		return true;
	}

	// Show the quick conncet window
	pCore->GetGUI()->GetServerBrowser()->m_pQuickConnectWindow->SetVisible ( !pCore->GetGUI()->GetServerBrowser()->m_pQuickConnectWindow->IsVisible() );
	pCore->GetGUI()->GetServerBrowser()->m_pQuickConnectWindow->BringToFront ();
	return true;
}

bool CMainMenu::OnDisconnectClick( CGUIElement_Impl * pElement )
{
	// Are we not connected to a server?
	if( !pCore->GetNetworkModule() || !pCore->GetNetworkModule()->IsConnected() )
	{
		// Hide the item
		pElement->SetVisible( false );
		return true;
	}

	// Disconnect from the network
	pCore->GetNetworkModule()->Disconnect();

	// Put the player far away to stop any input lag
	pCore->GetPlayerManager()->GetLocalPlayer()->Teleport( CVector3( 0, 0, -200 ) );

	// Stop multiplayer activities
	pCore->StopMultiplayer();

	// Start multiplayer activities
	pCore->StartMultiplayer();

	// Show the main menu
	SetVisible( true );

	return true;
}

bool CMainMenu::OnConnectClick( CGUIElement_Impl * pElement )
{
	// Is their nickname not set?
	if( pCore->GetNick().IsEmpty() || !pCore->GetNick().Compare( "Player" ) )
	{
		// Bring the settings window to the foreground
		pCore->GetGUI()->GetSettings()->SetVisible( true );
		return true;
	}

	// Hide the settings
	if( pCore->GetGUI()->GetSettings() && pCore->GetGUI()->GetSettings()->IsVisilbe() )
		pCore->GetGUI()->GetSettings()->SetVisible( false );

	// Connect to the selected server
	if( pCore->GetGUI()->GetServerBrowser() )
		pCore->GetGUI()->GetServerBrowser()->ConnectToSelectedServer();

	return true;
}

bool CMainMenu::OnRefreshClick( CGUIElement_Impl * pElement )
{
	// Hide the settings
	if( pCore->GetGUI()->GetSettings() && pCore->GetGUI()->GetSettings()->IsVisilbe() )
		pCore->GetGUI()->GetSettings()->SetVisible( false );

	// Refresh the server browser
	if( pCore->GetGUI()->GetServerBrowser() )
		pCore->GetGUI()->GetServerBrowser()->Refresh();

	return true;
}

bool CMainMenu::OnSettingsClick( CGUIElement_Impl * pElement )
{
	// Show the settings screen
	if( pCore->GetGUI()->GetSettings() )
		pCore->GetGUI()->GetSettings()->SetVisible( !pCore->GetGUI()->GetSettings()->IsVisilbe() );

	return true;
}

bool CMainMenu::OnQuitClick( CGUIElement_Impl * pElement )
{
	// Shutdown
	pCore->Shutdown();

	return true;
}

bool CMainMenu::OnItemEnter( CGUIElement_Impl * pElement )
{
	// Set the element alpha
	if( pElement )
		pElement->SetAlpha( 0.6f );

	return true;
}

bool CMainMenu::OnItemLeave( CGUIElement_Impl * pElement )
{
	// Set the element alpha
	if( pElement )
		pElement->SetAlpha( 1.0f );

	return true;
}

void CMainMenu::SetVisible( bool bVisible )
{
	// Are we showing the main menu?
	if( bVisible )
	{
		// Get the previous mouse state
		m_bPreviousMouseState = pCore->GetGUI()->IsCursorVisible();

		// Get the previous control state
		if( pCore->GetPlayerManager() && pCore->GetPlayerManager()->GetLocalPlayer() )
			m_bPreviousControlState = pCore->GetPlayerManager()->GetLocalPlayer()->AreControlsLocked();
		else
			m_bPreviousControlState = true;

		// Hide all the clientscript gui elements
		if( pCore->GetClientScriptingManager() && pCore->GetClientScriptingManager()->GetScriptGUIManager() )
			pCore->GetClientScriptingManager()->GetScriptGUIManager()->Hide();
	}

	// Update the variable
	m_bVisible = bVisible;

	// Set the last fade time
	m_ulLastFadeTime = SharedUtility::GetTime ();

	// Toggle the background
	//if( m_pBackground )
	//	m_pBackground->SetVisible( bVisible );

	// Toggle the logo
	if( m_pLogo )
		m_pLogo->SetVisible( bVisible );

	// Loop over each item
	for( std::list< CGUIStaticImage_Impl* >::iterator iter = m_items.begin(); iter != m_items.end(); iter++ )
	{
		// Toggle the current items visiblity
		if( *iter )
			(*iter)->SetVisible( bVisible );
	}

	// Are we showing the main menu?
	if( bVisible )
	{
		// Toggle the connect button
		m_pConnect->SetVisible ( pCore->GetGUI()->GetServerBrowser()->GetServerGridList ( pCore->GetGUI()->GetServerBrowser()->GetCurrentServerBrowserType () )->GetSelectedCount() > 0 );

		// Toggle the disconnect button
		if( m_pDisconnect )
			m_pDisconnect->SetVisible( pCore->GetNetworkModule()->IsConnected() );
	}

	// Toggle the serverbrowser
	if( pCore->GetGUI()->GetServerBrowser() )
		pCore->GetGUI()->GetServerBrowser()->SetVisible( bVisible );

	// Toggle the mouse cursor
	pCore->GetGUI()->SetCursorVisible( bVisible );

	// Are we hiding the main menu?
	if( !bVisible )
	{
		// Restore the previous mouse state
		pCore->GetGUI()->SetCursorVisible( m_bPreviousMouseState );

		// Hide the menu settings if they're showing
		if( pCore->GetGUI()->GetSettings()->IsVisilbe() )
			pCore->GetGUI()->GetSettings()->SetVisible( false );

		// Restore the previous control state
		if( pCore->GetPlayerManager() && pCore->GetPlayerManager()->GetLocalPlayer() )
			pCore->GetPlayerManager()->GetLocalPlayer()->LockControls( m_bPreviousControlState );

		// Show all the clientscript gui elements that were hidden
		if( pCore->GetClientScriptingManager() && pCore->GetClientScriptingManager()->GetScriptGUIManager() )
			pCore->GetClientScriptingManager()->GetScriptGUIManager()->Show();
	}
}

void CMainMenu::Render( void )
{
	// Is the main menu visible?
	if ( m_bVisible )
	{
		// Cover the screen with a black box
		pCore->GetGraphics()->DrawBox ( 0, 0, pCore->GetGUI()->GetCEGUI()->GetResolution().fX, pCore->GetGUI()->GetCEGUI()->GetResolution().fY, 0xFF000000 );

		// Is the sprite and background texture valid?
		if ( pSprite && pBackgroundTexture )
		{
			// Begin the sprite
			pSprite->Begin ( D3DXSPRITE_ALPHABLEND );

			// Draw the background image
			pSprite->Draw ( pBackgroundTexture[ m_uiBackgroundIndex - 1 ], NULL, NULL, &ImagePos, D3DCOLOR_ARGB(m_iFadeAlpha, 255, 255, 255) );

			// End the sprite
			pSprite->End ();
		}

		// Get the current time
		unsigned long ulCurrentTime = SharedUtility::GetTime ();

		// Should we start fading the background?
		if ( !m_bFadeBackgroundOut && !m_bFadeBackgroundIn && (ulCurrentTime - m_ulLastFadeTime) >= MAIN_MENU_HOLD_TIME )
		{
			// Mark as should fade background
			m_bFadeBackgroundOut = true;

			// Set the last fade time
			m_ulLastFadeTime = ulCurrentTime;
		}

		// Should we fade the background
		if ( m_bFadeBackgroundOut )
		{
			// Decrease the fade alpha
			m_iFadeAlpha --;

			// Are we done?
			if ( m_iFadeAlpha <= 0 )
			{
				// Update flags
				m_bFadeBackgroundIn = true;
				m_bFadeBackgroundOut = false;

				// Increase the background index
				m_uiBackgroundIndex++;

				// If we've looped over all images, reset the background index
				if ( m_uiBackgroundIndex > 4 )
					m_uiBackgroundIndex = 1;
			}
		}
		else if ( m_bFadeBackgroundIn )
		{
			// Increase the fade alpha
			m_iFadeAlpha ++;

			if ( m_iFadeAlpha >= 255 )
			{
				// Reset flags
				m_bFadeBackgroundIn = false;
				m_bFadeBackgroundOut = false;
				m_ulLastFadeTime = SharedUtility::GetTime ();

				// Reset alpha
				m_iFadeAlpha = 255;
			}
		}
	}
}

void CMainMenu::OnScreenSizeChange ( float fX, float fY )
{
	// Reload the sprite
	pSprite->OnResetDevice ();

	// Reload the background textures
	LoadBackgroundImages ( fX, fY );

	// Get the item render position
	float renderX = (fX - 40.0f);

	// Adjust all button positions
	m_pQuit->SetPosition ( Vector2 ( renderX - 46, 61.5f ) );				renderX -= 86;
	m_pSettings->SetPosition ( Vector2 ( renderX - 94, 61.5f ) );			renderX -= 134;
	m_pRefresh->SetPosition ( Vector2 ( renderX - 87, 61.5f ) );			renderX -= 127;
	m_pConnect->SetPosition ( Vector2 ( renderX - 90, 61.5f ) );			renderX -= 130;
	m_pDisconnect->SetPosition ( Vector2 ( renderX - 125, 61.5f ) );		renderX -= 165;
	m_pQuickConnect->SetPosition ( Vector2 ( renderX - 26, 61.5f ) );
	
	// Adjust the server browser UI
	pCore->GetGUI()->GetServerBrowser()->OnScreenSizeChange ( fX, fY );
}

void CMainMenu::OnDeviceLost ( void )
{
	// Release the background images
	ReleaseBackgroundImages ();

	// Notify the sprite
	pSprite->OnLostDevice ();
}

void CMainMenu::LoadBackgroundImages ( float fWidth, float fHeight )
{
	// Load all the background sprites
	D3DXCreateTextureFromFileEx ( pCore->GetGraphics()->GetDevice(), "data\\gui\\images\\1.jpg", fWidth, fHeight, NULL, NULL, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, colourKey, &SrcInfo, NULL, &pBackgroundTexture[0] );
	D3DXCreateTextureFromFileEx ( pCore->GetGraphics()->GetDevice(), "data\\gui\\images\\2.jpg", fWidth, fHeight, NULL, NULL, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, colourKey, &SrcInfo, NULL, &pBackgroundTexture[1] );
	D3DXCreateTextureFromFileEx ( pCore->GetGraphics()->GetDevice(), "data\\gui\\images\\3.jpg", fWidth, fHeight, NULL, NULL, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, colourKey, &SrcInfo, NULL, &pBackgroundTexture[2] );
	D3DXCreateTextureFromFileEx ( pCore->GetGraphics()->GetDevice(), "data\\gui\\images\\4.jpg", fWidth, fHeight, NULL, NULL, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, colourKey, &SrcInfo, NULL, &pBackgroundTexture[3] );
}

void CMainMenu::ReleaseBackgroundImages ( void )
{
	// Release all background textures
	SAFE_RELEASE ( pBackgroundTexture[0] );
	SAFE_RELEASE ( pBackgroundTexture[1] );
	SAFE_RELEASE ( pBackgroundTexture[2] );
	SAFE_RELEASE ( pBackgroundTexture[3] );
}