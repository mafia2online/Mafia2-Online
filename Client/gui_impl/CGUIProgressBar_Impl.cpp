/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIProgressBar_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"../StdInc.h"

CGUIProgressBar_Impl::CGUIProgressBar_Impl( CGUI_Impl * pGUI, CGUIElement_Impl * pParent )
{
	// Store the manager instance
	m_pManager = pGUI;

	// Get a unique name for cegui
	String strName = pGUI->GetUniqueName();

	// Create the progress bar and set default settings
	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/ProgressBar", strName.Get() );
	m_pWindow->setDestroyedByParent( false );

	// Store the pointer to this element
	m_pWindow->setUserData( (void *)this );

	// Register our events
	AddEvents();

	// Set the default progress
	SetProgress( 0 );

	// If a parent is set, add it to it's childs list, if not add it as a child to the gui manager
	if( pParent )
		SetParent( pParent );
	else
	{
		pGUI->AddChild( this );
		SetParent( NULL );
	}
}

CGUIProgressBar_Impl::~CGUIProgressBar_Impl( void )
{
	// Destroy the element
	DestroyElement();
}

void CGUIProgressBar_Impl::SetProgress( float fProgress )
{
	((CEGUI::ProgressBar *)m_pWindow)->setProgress( fProgress );
}

float CGUIProgressBar_Impl::GetProgress( void )
{
	return ((CEGUI::ProgressBar *)m_pWindow)->getProgress();
}