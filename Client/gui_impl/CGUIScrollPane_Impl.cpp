/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIScrollPane_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"../StdInc.h"

CGUIScrollPane_Impl::CGUIScrollPane_Impl( CGUI_Impl * pGUI, CGUIElement_Impl * pParent )
{
	// Store the manager instance
	m_pManager = pGUI;

	// Get a unique name for cegui
	String strName = pGUI->GetUniqueName();

	// Create the window and set default settings
	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/ScrollablePane", strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	m_pWindow->setRect( CEGUI::Relative, CEGUI::Rect( 0.9f, 0.9f, 0.9f, 0.9f ) );
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

	// Set defaults
	SetHorizontalScrollStepSize( 100.0f );
	SetVerticalScrollStepSize( 100.0f );
}

CGUIScrollPane_Impl::~CGUIScrollPane_Impl( void )
{
	// Destroy the element
	DestroyElement();
}

void CGUIScrollPane_Impl::SetHorizontalScrollBar( bool bEnabled )
{
	((CEGUI::ScrollablePane *)m_pWindow)->setShowHorzScrollbar( bEnabled );
}

void CGUIScrollPane_Impl::SetVerticalScrollBar( bool bEnabled )
{
	((CEGUI::ScrollablePane *)m_pWindow)->setShowVertScrollbar( bEnabled );
}

void CGUIScrollPane_Impl::SetHorizontalScrollBarPosition( float fPosition )
{
	((CEGUI::ScrollablePane *)m_pWindow)->setHorizontalScrollPosition( fPosition );
}

float CGUIScrollPane_Impl::GetHorizontalScrollBarPosition( void )
{
	return ((CEGUI::ScrollablePane *)m_pWindow)->getHorizontalScrollPosition();
}

void CGUIScrollPane_Impl::SetVerticalScrollBarPosition( float fPosition )
{
	((CEGUI::ScrollablePane *)m_pWindow)->setVerticalScrollPosition( fPosition );
}

float CGUIScrollPane_Impl::GetVerticalScrollBarPosition( void )
{
	return ((CEGUI::ScrollablePane *)m_pWindow)->getVerticalScrollPosition();
}

void CGUIScrollPane_Impl::SetHorizontalScrollStepSize( float fPosition )
{
	((CEGUI::ScrollablePane *)m_pWindow)->setHorizontalStepSize( fPosition );
}

float CGUIScrollPane_Impl::GetHorizontalScrollStepSize( void )
{
	return ((CEGUI::ScrollablePane *)m_pWindow)->getHorizontalStepSize();
}

void CGUIScrollPane_Impl::SetVerticalScrollStepSize( float fPosition )
{
	((CEGUI::ScrollablePane *)m_pWindow)->setVerticalStepSize( fPosition );
}

float CGUIScrollPane_Impl::GetVerticalScrollStepSize( void )
{
	return ((CEGUI::ScrollablePane *)m_pWindow)->getVerticalStepSize();
}