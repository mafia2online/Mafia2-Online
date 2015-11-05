/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIScrollBar_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"../StdInc.h"

CGUIScrollBar_Impl::CGUIScrollBar_Impl( CGUI_Impl * pGUI, bool bHorizontal, CGUIElement_Impl * pParent )
{
	// Store the manager instance
	m_pManager = pGUI;

	// Get a unique name for cegui
	String strName = pGUI->GetUniqueName();

	// Create the window and set default settings
	m_pWindow = pGUI->GetWindowManager()->createWindow( (bHorizontal ? "CGUI/HorizontalScrollbar" : "CGUI/VerticalScrollbar"), strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	m_pWindow->setVisible( true );

	// Store the pointer to this element
	m_pWindow->setUserData( (void *)this );

	// Register our events
	m_pWindow->subscribeEvent( CEGUI::Scrollbar::EventScrollPositionChanged, CEGUI::Event::Subscriber( &CGUIScrollBar_Impl::Event_OnScroll, this ) );
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

CGUIScrollBar_Impl::~CGUIScrollBar_Impl( void )
{
	// Destroy the element
	DestroyElement();
}

void CGUIScrollBar_Impl::SetScrollPosition( float fPosition )
{
	((CEGUI::Scrollbar *)m_pWindow)->setScrollPosition( fPosition );
}

float CGUIScrollBar_Impl::GetScrollPosition( void )
{
	return ((CEGUI::Scrollbar *)m_pWindow)->getScrollPosition();
}

bool CGUIScrollBar_Impl::Event_OnScroll( const CEGUI::EventArgs &e )
{
	// Is the event handler valid?
	if( m_pfnOnScroll )
		m_pfnOnScroll( this );

	return true;
}