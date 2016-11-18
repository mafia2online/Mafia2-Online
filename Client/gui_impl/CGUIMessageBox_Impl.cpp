/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIMessageBox_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"
#include "CCore.h"

#include "CString.h"
#include "Math/CVector3.h"

#include "CGUI.h"

#include "CClientScriptGUIManager.h"
#include "CClientScriptingManager.h"

#include "CGUI_Impl.h"
#include "CGUIWindow_Impl.h"
#include "CGUIStaticImage_Impl.h"
#include "CGUILabel_Impl.h"
#include "CGUIButton_Impl.h"
#include "CGUIMessageBox_Impl.h"

#define	MESSAGEBOX_WIDTH			400.0f
#define	MESSAGEBOX_HEIGHT			150.0f
#define	ICON_SIZE					32
#define	PADDING						5

CGUIMessageBox_Impl::CGUIMessageBox_Impl( CGUI_Impl * pGUI, const char * szTitle, const char * szCaption, const char * szButton1, const char * szButton2 )
{
	// Reset
	m_pIcon = NULL;
	m_pCaption = NULL;
	memset ( m_pButton, 0, sizeof ( m_pButton ) );
	memset ( m_pfnButtonClickHandler, 0, sizeof ( m_pfnButtonClickHandler ) );
	memset ( m_pButtonClickUserData, 0, sizeof ( m_pButtonClickUserData ) );

	// Get the screen size
	Vector2 vecScreen = pGUI->GetResolution();

	// Calculate the window height depending on the elements being shown
	float fWindowHeight = MESSAGEBOX_HEIGHT;

	// If we have don't any buttons adjust the offset
	if ( !strlen ( szButton1 ) && !strlen ( szButton2 ) )
		fWindowHeight -= 24.0f;

	// Create the window
	m_pWindow = pGUI->CreateWnd( szTitle );
	m_pWindow->SetAlwaysOnTop( true );
	m_pWindow->SetCloseButtonEnabled( false );
	m_pWindow->SetSizingEnabled( false );
	m_pWindow->SetPosition( Vector2( (vecScreen.fX / 2) - (MESSAGEBOX_WIDTH / 2), (vecScreen.fY / 2) - (fWindowHeight / 2) ) );
	m_pWindow->SetSize( Vector2( MESSAGEBOX_WIDTH, fWindowHeight ) );

	// Create the icon
	m_pIcon = pGUI->CreateStaticImage( m_pWindow.get() );
	m_pIcon->SetPosition( Vector2( 20.0f, (fWindowHeight / 2) - (ICON_SIZE / 2) ) );
	m_pIcon->SetSize( Vector2( ICON_SIZE, ICON_SIZE ) );
	m_pIcon->SetVisible( true );

	// Create the caption
	m_pCaption = pGUI->CreateLabel ( szCaption, NULL, m_pWindow.get() );
	m_pCaption->SetHorizontalAlignment ( CEGUI::HorizontalAlignment::HA_LEFT );
	m_pCaption->SetVerticalAlignment ( CEGUI::VerticalAlignment::VA_CENTRE );
	m_pCaption->SendToBack ();
	m_pCaption->SetAutoSizeEnabled ( false );
	m_pCaption->SetZOrderingEnabled ( false );
	m_pCaption->SetPosition ( Vector2 ( ICON_SIZE + (PADDING * 2), PADDING ) );
	m_pCaption->SetSize ( Vector2( MESSAGEBOX_WIDTH - ICON_SIZE - (PADDING * 3), fWindowHeight ) );

	//
	bool bFirstButton = false;

	// Do we have a text for button 1?
	if ( strlen ( szButton1 ) )
	{
		// Create the button
		m_pButton[0] = pGUI->CreateButton ( szButton1, m_pWindow.get() );
		m_pButton[0]->SetPosition ( Vector2( MESSAGEBOX_WIDTH - 94, fWindowHeight * 0.76f ) );
		m_pButton[0]->SetSize ( Vector2 ( 84, 24 ) );
		m_pButton[0]->SetZOrderingEnabled ( false );
		m_pButton[0]->SetClickHandler ( GUI_CALLBACK ( &CGUIMessageBox_Impl::Event_OnButton1Click, this ) );

		// Mark as we have first button
		bFirstButton = true;
	}

	// Do we have text for button 2?
	if ( strlen ( szButton2 ) )
	{
		// Button 2 offset
		float fOffsetX = 0.0f;

		// Do we have a first button?
		if ( bFirstButton )
		{
			// Set the button offset
			fOffsetX = 89.0f;
		}

		// Create the button
		m_pButton[1] = pGUI->CreateButton ( szButton2, m_pWindow.get() );
		m_pButton[1]->SetPosition ( Vector2( MESSAGEBOX_WIDTH - 94, fWindowHeight * 0.76f ) );
		m_pButton[1]->SetSize ( Vector2 ( 84, 24 ) );
		m_pButton[1]->SetZOrderingEnabled ( false );
		m_pButton[1]->SetClickHandler ( GUI_CALLBACK ( &CGUIMessageBox_Impl::Event_OnButton2Click, this ) );

		// Do we have a first button?
		if ( m_pButton[0] )
		{
			// Adjust the button position
			m_pButton[0]->SetPosition ( Vector2 ( (MESSAGEBOX_WIDTH - ((94 * 2) - 5)), fWindowHeight * 0.76f ) );
		}
	}

	// Set focus to the window
	m_pWindow->Activate();
}

CGUIMessageBox_Impl::~CGUIMessageBox_Impl( void )
{
}

bool CGUIMessageBox_Impl::Event_OnButton1Click ( CGUIElement_Impl * pElement )
{
	// Do we have a button 1 click handler?
	if ( m_pfnButtonClickHandler[0] )
		m_pfnButtonClickHandler[0] ( m_pButtonClickUserData[0] );

	return true;
}

bool CGUIMessageBox_Impl::Event_OnButton2Click ( CGUIElement_Impl * pElement )
{
	// Do we have a button 2 click handler?
	if ( m_pfnButtonClickHandler[1] )
		m_pfnButtonClickHandler[1] ( m_pButtonClickUserData[1] );

	return true;
}