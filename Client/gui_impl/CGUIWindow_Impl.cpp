/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIWindow_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"../StdInc.h"

CGUIWindow_Impl::CGUIWindow_Impl( CGUI_Impl * pGUI, String strCaption, CGUIElement_Impl * pParent )
{
	// Store the manager instance
	m_pManager = pGUI;

	// Get a unique name for cegui
	String strName = pGUI->GetUniqueName();

	// Create the window and set default settings
	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/FrameWindow", strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	m_pWindow->setRect( CEGUI::Relative, CEGUI::Rect( 0.10f, 0.10f, 0.60f, 0.90f ) );
	m_pWindow->setAlpha( 0.8f );
	((CEGUI::FrameWindow *)m_pWindow)->setRollupEnabled( false );

	// Store the pointer to this element
	m_pWindow->setUserData( (void *)this );

	// Set fixed minimum size
	m_pWindow->setMetricsMode( CEGUI::Absolute );
	m_pWindow->setMinimumSize( CEGUI::Size( 96.0f, 48.0f ) );

	// Set the window caption
	SetText( strCaption );

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

CGUIWindow_Impl::~CGUIWindow_Impl( void )
{
	// Destroy the element
	DestroyElement();
}

void CGUIWindow_Impl::SetMovable( bool bMovable )
{
	// Toggle drag moving
	((CEGUI::FrameWindow *)m_pWindow)->setDragMovingEnabled( bMovable );
}

bool CGUIWindow_Impl::IsMovable( void )
{
	return ((CEGUI::FrameWindow *)m_pWindow)->isDragMovingEnabled();
}

void CGUIWindow_Impl::SetSizingEnabled( bool bSizingEnabled )
{
	// Toggle sizing
	((CEGUI::FrameWindow *)m_pWindow)->setSizingEnabled( bSizingEnabled );
}

bool CGUIWindow_Impl::IsSizingEnabled( void )
{
	return ((CEGUI::FrameWindow *)m_pWindow)->isSizingEnabled();
}

void CGUIWindow_Impl::SetFrameEnabled( bool bFrameEnabled )
{
	// Toggle the frame
	((CEGUI::FrameWindow *)m_pWindow)->setFrameEnabled( bFrameEnabled );
}

bool CGUIWindow_Impl::IsFrameEnabled( void )
{
	return ((CEGUI::FrameWindow *)m_pWindow)->isFrameEnabled();
}

void CGUIWindow_Impl::SetCloseButtonEnabled( bool bCloseButtonEnabled )
{
	// Toggle the close button
	((CEGUI::FrameWindow *)m_pWindow)->setCloseButtonEnabled( bCloseButtonEnabled );
}

bool CGUIWindow_Impl::IsCloseButtonEnabled( void )
{
	return ((CEGUI::FrameWindow *)m_pWindow)->isCloseButtonEnabled();
}

void CGUIWindow_Impl::SetTitlebarEnabled( bool bTitlebarEnabled )
{
	// Toggle the titlebar
	((CEGUI::FrameWindow *)m_pWindow)->setTitleBarEnabled( bTitlebarEnabled );
}

bool CGUIWindow_Impl::IsTitlebarEnabled( void )
{
	return ((CEGUI::FrameWindow *)m_pWindow)->isTitleBarEnabled();
}