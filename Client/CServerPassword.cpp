/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CServerPassword.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern		CCore				* pCore;

bool CServerPassword::Event_OnSubmitClick ( CGUIElement_Impl * pElement )
{
	// Do we have a submit button handler?
	if ( m_pfnSubmitHandler )
	{
		// Call the submit button handler
		m_pfnSubmitHandler ( m_pEdit->GetText (), m_pSubmitHandlerUserData );
	}

	return true;
}

bool CServerPassword::Event_OnCancelClick ( CGUIElement_Impl * pElement )
{
	// Hide the server password window
	SetVisible ( false );

	return true;
}

CServerPassword::CServerPassword ( CGUI_Impl * pGUI )
{
	// Create the main window
	m_pWindow = pGUI->CreateWnd ( "Password Protected Server" );
	m_pWindow->SetSize ( Vector2 ( 260, 120 ) );
	m_pWindow->SetPosition ( Vector2 ( (pGUI->GetResolution().fX / 2) - 150, (pGUI->GetResolution().fY / 2) - 95 ) );
	m_pWindow->SetVisible ( false );
	m_pWindow->SetAlwaysOnTop ( true );
	m_pWindow->SetSizingEnabled ( false );
	m_pWindow->SetAlpha ( 1.0f );

	// Create the label
	m_pLabel = pGUI->CreateLabel ( "Please enter the server password:", NULL, m_pWindow );
	m_pLabel->SetSize ( Vector2 ( 200, 20 ) );
	m_pLabel->SetPosition ( Vector2 ( 20, 25 ) );

	// Create the edit
	m_pEdit = pGUI->CreateEdit ( "", m_pWindow );
	m_pEdit->SetSize ( Vector2( 220, 24 ) );
	m_pEdit->SetPosition ( Vector2 ( 18, 50 ) );
	m_pEdit->SetMasked ( true );

	// Create the submit button
	m_pSubmit = pGUI->CreateButton ( "Connect", m_pWindow );
	m_pSubmit->SetSize ( Vector2 ( 75, 20 ) );
	m_pSubmit->SetPosition ( Vector2 ( 18, 85 ) );
	m_pSubmit->SetClickHandler ( GUI_CALLBACK( &CServerPassword::Event_OnSubmitClick, this ) );

	// Create the cancel button
	m_pCancel = pGUI->CreateButton ( "Cancel", m_pWindow );
	m_pCancel->SetSize ( Vector2 ( 75, 20 ) );
	m_pCancel->SetPosition ( Vector2 ( 100, 85 ) );
	m_pCancel->SetClickHandler ( GUI_CALLBACK( &CServerPassword::Event_OnCancelClick, this ) );
}

CServerPassword::~CServerPassword ( void )
{
	// Delete the gui items
	SAFE_DELETE ( m_pCancel );
	SAFE_DELETE ( m_pSubmit );
	SAFE_DELETE ( m_pEdit );
	SAFE_DELETE ( m_pLabel );
	SAFE_DELETE ( m_pWindow );
}

void CServerPassword::SetVisible ( bool bVisible )
{
	// Set the main window visible
	m_pWindow->SetVisible ( bVisible );

	// Are we showing the window?
	if ( bVisible )
	{
		// Bring it to the front
		m_pWindow->BringToFront ();
	}
	else
	{
		// Clear the text input
		m_pEdit->SetText ( "" );
	}
}

void CServerPassword::OnScreenSizeChange ( float fX, float fY )
{
	// Adjust the window position
	m_pWindow->SetPosition ( Vector2 ( (fX / 2) - 150.0f, (fY / 2) - 95.0f ) );
}