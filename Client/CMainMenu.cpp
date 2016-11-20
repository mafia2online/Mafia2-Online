/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CMainMenu.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "Math/CVector3.h"
#include "CString.h"

#include "gui_impl/CGUI_Impl.h"
#include "gui_impl/CGUIElement_Impl.h"
#include "gui_impl/CGUIStaticImage_Impl.h"
#include "gui_impl/CGUIWindow_Impl.h"
#include "gui_impl/CGUIGridList_Impl.h"

#include "CGUI.h"
#include "CEGUI.h"
#include "CGUICallback.h"

#include "CGraphics.h"
#include "CServerBrowser.h"

#include "CSettings.h"
#include "CMenuSettings.h"

#include "CDirect3D9Hook.h"
#include "CDirectInput8Hook.h"

#include "CNetworkModule.h"
#include "CPlayerManager.h"
#include "CLocalPlayer.h"

#include "CClientScriptingManager.h"
#include "CClientScriptGUIManager.h"

#include "CMainMenu.h"

#include "SharedUtility.h"

#include "CLogFile.h"

#define	BG_SIZE_X		1920
#define	BG_SIZE_y		1080

#define	MAIN_MENU_HOLD_TIME				6000
#define	MAIN_MENU_FADE_TIME				1000
#define	MAIN_MENU_IMAGES				5

D3DXIMAGE_INFO							SrcInfo;
D3DCOLOR                                colourKey = 0xFFFF00FF;
LPDIRECT3DTEXTURE9                      pBackgroundTexture [ MAIN_MENU_IMAGES ];
LPD3DXSPRITE							pSprite;
D3DXVECTOR3                             ImagePos;

std::vector<String>						images = { "1.png", "2.png", "3.png", "4.png", "5.png" };

CMainMenu::CMainMenu( CGUI_Impl * pGUI )
{
	m_pGUI = pGUI;

	m_bVisible = false;
	m_bPreviousMouseState = false;
	m_bPreviousControlState = false;
	m_pConnect = NULL;
	m_pDisconnect = NULL;
	m_bFadeBackgroundOut = false;
	m_bFadeBackgroundIn = false;
	m_uiBackgroundIndex = 2;
	m_iFadeAlpha = 255;

	float fX = pGUI->GetResolution().fX;
	float fY = pGUI->GetResolution().fY;

	m_pLogo = pGUI->CreateStaticImage();
	m_pLogo->LoadFromFile( "logo.png", SharedUtility::GetAbsolutePath( "data\\gui\\images" ) );
	m_pLogo->SetProperty( "InheritsAlpha", "False" );
	m_pLogo->SendToBack();
	m_pLogo->SetPosition( Vector2( 40.0f, 40.0f ) );
	m_pLogo->SetSize( Vector2( 249, 83 ) );
	m_pLogo->SetVisible( false );
	m_pLogo->SetAlwaysOnTop( true );

	D3DXCreateSprite(CCore::Instance()->GetGraphics()->GetDevice(), &pSprite);

	LoadBackgroundImages ( fX, fY );

	float renderX = (fX - 40.0f);

	m_pQuit = CreateItem( "quit.png", Vector2( (renderX - 46), 61.5f ), false, Vector2( 46, 23 ), GUI_CALLBACK( &CMainMenu::OnQuitClick, this ) );
	renderX -= 86;

	m_pSettings = CreateItem( "settings.png", Vector2( (renderX - 94), 61.5f ), false, Vector2( 94, 21 ), GUI_CALLBACK( &CMainMenu::OnSettingsClick, this ) );
	renderX -= 134;

	m_pRefresh = CreateItem( "refresh.png", Vector2( (renderX - 87), 61.5f ), false, Vector2( 87, 21 ), GUI_CALLBACK( &CMainMenu::OnRefreshClick, this ) );
	renderX -= 127;

	m_pConnect = CreateItem( "connect.png", Vector2( (renderX - 90), 61.5f ), false, Vector2( 90, 21 ), GUI_CALLBACK( &CMainMenu::OnConnectClick, this ) );
	renderX -= 130;

	m_pDisconnect = CreateItem( "disconnect.png", Vector2( (renderX - 125), 61.5f ), false, Vector2( 125, 21 ), GUI_CALLBACK( &CMainMenu::OnDisconnectClick, this ) );
	renderX -= 165;

	m_pQuickConnect = CreateItem ( "quick_connect.png", Vector2( (renderX - 26), 61.5f ), false, Vector2( 26, 22 ), GUI_CALLBACK( &CMainMenu::OnQuickConnectClick, this ) );

	CCore::Instance()->GetGUI()->GetServerBrowser()->SetupUI(40, 160, (fX - 80), (fY - 240));
}

CMainMenu::~CMainMenu( void )
{
}

std::shared_ptr<CGUIStaticImage_Impl> CMainMenu::CreateItem( String strLocation, Vector2 vecPosition, bool bRelativePosition, Vector2 vecSize, GUI_CALLBACK pfnHandler )
{
	std::shared_ptr<CGUIStaticImage_Impl> pItem = m_pGUI->CreateStaticImage();

	if( !pItem )
	{
		CLogFile::Printf( "Failed to create main menu item! (Location: %d, Callback: 0x%p)", strLocation.Get(), pfnHandler );
		return NULL;
	}

	pItem->SetPosition( vecPosition, bRelativePosition );
	pItem->SetSize( vecSize );
	pItem->SetAlwaysOnTop( true );
	pItem->SetProperty( "InheritsAlpha", "False" );

	pItem->LoadFromFile( strLocation, SharedUtility::GetAbsolutePath( "data\\gui\\images" ) );

	pItem->SetMouseEnterHandler( GUI_CALLBACK( &CMainMenu::OnItemEnter, this ) );
	pItem->SetMouseLeaveHandler( GUI_CALLBACK( &CMainMenu::OnItemLeave, this ) );

	if( pfnHandler )
		pItem->SetClickHandler( pfnHandler );

	pItem->SetVisible( false );

	m_items.push_back( pItem );
	return pItem;
}

bool CMainMenu::OnQuickConnectClick( CGUIElement_Impl * pElement )
{
	if( CCore::Instance()->GetNick().IsEmpty() || !CCore::Instance()->GetNick().Compare( "Player" ) )
	{
		CCore::Instance()->GetGUI()->GetSettings()->SetVisible(true);
		return true;
	}

	CCore::Instance()->GetGUI()->GetServerBrowser()->m_pQuickConnectWindow->SetVisible(!CCore::Instance()->GetGUI()->GetServerBrowser()->m_pQuickConnectWindow->IsVisible());
	CCore::Instance()->GetGUI()->GetServerBrowser()->m_pQuickConnectWindow->BringToFront();
	return true;
}

bool CMainMenu::OnDisconnectClick( CGUIElement_Impl * pElement )
{
	if( !CCore::Instance()->GetNetworkModule() || !CCore::Instance()->GetNetworkModule()->IsConnected() )
	{
		pElement->SetVisible( false );
		return true;
	}

	CCore::Instance()->GetNetworkModule()->Disconnect();
	CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->Teleport(CVector3(0, 0, -200));

	CCore::Instance()->StopMultiplayer();
	CCore::Instance()->StartMultiplayer();

	SetVisible( true );

	return true;
}

bool CMainMenu::OnConnectClick( CGUIElement_Impl * pElement )
{
	if( CCore::Instance()->GetNick().IsEmpty() || !CCore::Instance()->GetNick().Compare( "Player" ) )
	{
		CCore::Instance()->GetGUI()->GetSettings()->SetVisible(true);
		return true;
	}

	if (CCore::Instance()->GetGUI()->GetSettings() && CCore::Instance()->GetGUI()->GetSettings()->IsVisilbe())
		CCore::Instance()->GetGUI()->GetSettings()->SetVisible(false);

	if (CCore::Instance()->GetGUI()->GetServerBrowser())
		CCore::Instance()->GetGUI()->GetServerBrowser()->ConnectToSelectedServer();

	return true;
}

bool CMainMenu::OnRefreshClick( CGUIElement_Impl * pElement )
{
	if( CCore::Instance()->GetGUI()->GetSettings() && CCore::Instance()->GetGUI()->GetSettings()->IsVisilbe() )
		CCore::Instance()->GetGUI()->GetSettings()->SetVisible(false);

	if( CCore::Instance()->GetGUI()->GetServerBrowser() )
		CCore::Instance()->GetGUI()->GetServerBrowser()->Refresh();

	return true;
}

bool CMainMenu::OnSettingsClick( CGUIElement_Impl * pElement )
{
	if( CCore::Instance()->GetGUI()->GetSettings() )
		CCore::Instance()->GetGUI()->GetSettings()->SetVisible( !CCore::Instance()->GetGUI()->GetSettings()->IsVisilbe() );

	return true;
}

bool CMainMenu::OnQuitClick( CGUIElement_Impl * pElement )
{
	CCore::Instance()->Shutdown();
	return true;
}

bool CMainMenu::OnItemEnter( CGUIElement_Impl * pElement )
{
	if( pElement )
		pElement->SetAlpha( 0.6f );

	return true;
}

bool CMainMenu::OnItemLeave( CGUIElement_Impl * pElement )
{
	if( pElement )
		pElement->SetAlpha( 1.0f );

	return true;
}

void CMainMenu::SetVisible( bool bVisible )
{
	if( bVisible )
	{
		m_bPreviousMouseState = CCore::Instance()->GetGUI()->IsCursorVisible();

		if (CCore::Instance()->GetPlayerManager() && CCore::Instance()->GetPlayerManager()->GetLocalPlayer())
			m_bPreviousControlState = CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->AreControlsLocked();
		else
			m_bPreviousControlState = true;

		if (CCore::Instance()->GetClientScriptingManager() && CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager())
			CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager()->Hide();
	}

	m_bVisible = bVisible;

	m_ulLastFadeTime = SharedUtility::GetTime ();

	if( m_pLogo )
		m_pLogo->SetVisible( bVisible );

	for (std::shared_ptr<CGUIStaticImage_Impl> img : m_items)
		img->SetVisible( bVisible );

	if( bVisible )
	{
		m_pConnect->SetVisible ( CCore::Instance()->GetGUI()->GetServerBrowser()->GetServerGridList ( CCore::Instance()->GetGUI()->GetServerBrowser()->GetCurrentServerBrowserType () )->GetSelectedCount() > 0 );
		if( m_pDisconnect )
			m_pDisconnect->SetVisible( CCore::Instance()->GetNetworkModule()->IsConnected() );
	}

	if( CCore::Instance()->GetGUI()->GetServerBrowser() )
		CCore::Instance()->GetGUI()->GetServerBrowser()->SetVisible( bVisible );

	CCore::Instance()->GetGUI()->SetCursorVisible(bVisible);
	if( !bVisible )
	{
		CCore::Instance()->GetGUI()->SetCursorVisible(m_bPreviousMouseState);

		if (CCore::Instance()->GetGUI()->GetSettings()->IsVisilbe())
			CCore::Instance()->GetGUI()->GetSettings()->SetVisible(false);

		if (CCore::Instance()->GetPlayerManager() && CCore::Instance()->GetPlayerManager()->GetLocalPlayer())
			CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->LockControls(m_bPreviousControlState);

		if (CCore::Instance()->GetClientScriptingManager() && CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager())
			CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager()->Show();
	}
}

void CMainMenu::Render( void )
{
	if ( m_bVisible )
	{
		CCore::Instance()->GetGraphics()->DrawBox(0, 0, CCore::Instance()->GetGUI()->GetCEGUI()->GetResolution().fX, CCore::Instance()->GetGUI()->GetCEGUI()->GetResolution().fY, 0xFF000000);

		if ( pSprite && pBackgroundTexture )
		{
			pSprite->Begin ( D3DXSPRITE_ALPHABLEND );
			pSprite->Draw ( pBackgroundTexture[ m_uiBackgroundIndex - 1 ], NULL, NULL, &ImagePos, D3DCOLOR_ARGB(m_iFadeAlpha, 255, 255, 255) );
			pSprite->End ();
		}

		unsigned long ulCurrentTime = SharedUtility::GetTime ();

		if ( !m_bFadeBackgroundOut && !m_bFadeBackgroundIn && (ulCurrentTime - m_ulLastFadeTime) >= MAIN_MENU_HOLD_TIME )
		{
			m_bFadeBackgroundOut = true;
			m_ulLastFadeTime = ulCurrentTime;
		}
		if ( m_bFadeBackgroundOut )
		{
			m_iFadeAlpha --;

			if ( m_iFadeAlpha <= 0 )
			{
				m_bFadeBackgroundIn = true;
				m_bFadeBackgroundOut = false;
				m_uiBackgroundIndex++;

				if ( m_uiBackgroundIndex > MAIN_MENU_IMAGES)
					m_uiBackgroundIndex = 1;
			}
		}
		else if ( m_bFadeBackgroundIn )
		{
			m_iFadeAlpha ++;
			if ( m_iFadeAlpha >= 255 )
			{
				m_bFadeBackgroundIn = false;
				m_bFadeBackgroundOut = false;
				m_ulLastFadeTime = SharedUtility::GetTime ();

				m_iFadeAlpha = 255;
			}
		}
	}
}

void CMainMenu::OnScreenSizeChange ( float fX, float fY )
{
	pSprite->OnResetDevice ();

	LoadBackgroundImages ( fX, fY );

	float renderX = (fX - 40.0f);

	m_pQuit->SetPosition ( Vector2 ( renderX - 46, 61.5f ) );				renderX -= 86;
	m_pSettings->SetPosition ( Vector2 ( renderX - 94, 61.5f ) );			renderX -= 134;
	m_pRefresh->SetPosition ( Vector2 ( renderX - 87, 61.5f ) );			renderX -= 127;
	m_pConnect->SetPosition ( Vector2 ( renderX - 90, 61.5f ) );			renderX -= 130;
	m_pDisconnect->SetPosition ( Vector2 ( renderX - 125, 61.5f ) );		renderX -= 165;
	m_pQuickConnect->SetPosition ( Vector2 ( renderX - 26, 61.5f ) );

	CCore::Instance()->GetGUI()->GetServerBrowser()->OnScreenSizeChange(fX, fY);
}

void CMainMenu::OnDeviceLost ( void )
{
	ReleaseBackgroundImages ();
	pSprite->OnLostDevice ();
}

void CMainMenu::LoadBackgroundImages ( float fWidth, float fHeight )
{
	std::vector<String>::iterator it;
	int count = 0;

	for (it = images.begin(); it < images.end(); it++, count++)
	{
		std::string path2 = "data\\gui\\images\\";
		path2.append(it->Get());

		D3DXCreateTextureFromFileEx(CCore::Instance()->GetGraphics()->GetDevice(), path2.c_str(), fWidth, fHeight, NULL, NULL, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, colourKey, &SrcInfo, NULL, &pBackgroundTexture[count]);
	}
}

void CMainMenu::ReleaseBackgroundImages ( void )
{
	std::vector<String>::iterator it;
	int count = 0;

	for (it = images.begin(); it < images.end(); it++, count++)
	{
		SAFE_RELEASE(pBackgroundTexture[count]);
	}
}