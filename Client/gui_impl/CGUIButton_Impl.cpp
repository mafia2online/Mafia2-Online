/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIButton_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"../StdInc.h"

CGUIButton_Impl::CGUIButton_Impl( CGUI_Impl * pGUI, String strCaption, CGUIElement_Impl * pParent )
{
	// Store the manager instance
	m_pManager = pGUI;

	// Get a unique name for cegui
	String strName = pGUI->GetUniqueName();

	// Create the window and set default settings
	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/Button", strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	SetText( strCaption );
	m_pWindow->setSize( CEGUI::Absolute, CEGUI::Size( 128.0f, 24.0f ) );
	m_pWindow->setVisible( true );

	// Store the pointer to this element
	m_pWindow->setUserData( (void *)this );

	// Register our events
	AddEvents();

	// If a parent is set, add it to it's childs list, if not add it as a child to the gui manager
	if( pParent )
		SetParent( pParent );
	else
	{
		pGUI->AddChild( this );
		SetParent( NULL );
	}
}

CGUIButton_Impl::~CGUIButton_Impl( void )
{
	// Destroy the element
	DestroyElement();
}