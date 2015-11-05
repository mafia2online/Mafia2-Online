/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIElement_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"../StdInc.h"

extern	CCore			* pCore;

// Nodrawing zones
#define CGUI_NODRAW_LEFT	9.0f
#define CGUI_NODRAW_RIGHT	9.0f
#define CGUI_NODRAW_TOP		9.0f
#define CGUI_NODRAW_BOTTOM	9.0f

CGUIElement_Impl::CGUIElement_Impl( void )
{
	// Reset
	m_pWindow = NULL;
	m_pParent = NULL;
	m_pManager = NULL;
}

void CGUIElement_Impl::AddEvents( void )
{
	// Register default events
	m_pWindow->subscribeEvent( CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber( &CGUIElement_Impl::Event_OnClick, this ) );
	m_pWindow->subscribeEvent( CEGUI::Window::EventMoved, CEGUI::Event::Subscriber( &CGUIElement_Impl::Event_OnMove, this ) );
	m_pWindow->subscribeEvent( CEGUI::Window::EventSized, CEGUI::Event::Subscriber( &CGUIElement_Impl::Event_OnResize, this ) );
	m_pWindow->subscribeEvent( CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber( &CGUIElement_Impl::Event_OnMouseEnter, this ) );
	m_pWindow->subscribeEvent( CEGUI::Window::EventMouseLeaves, CEGUI::Event::Subscriber( &CGUIElement_Impl::Event_OnMouseLeave, this ) );
}

void CGUIElement_Impl::DestroyElement( void )
{
	// Remove the element from the redraw queue
	m_pManager->RemoveFromRedrawQueue( (CGUIElement_Impl *)m_pWindow->getUserData() );

	// Reset the user data
	m_pWindow->setUserData ( NULL );

	// Destroy the control
	m_pManager->GetWindowManager()->destroyWindow( m_pWindow );
}

void CGUIElement_Impl::SetVisible( bool bVisible )
{
	// Toggle the element visibility
	m_pWindow->setVisible( bVisible );
}

bool CGUIElement_Impl::IsVisible( void )
{
	return m_pWindow->isVisible();
}

void CGUIElement_Impl::SetEnabled( bool bEnabled )
{
	// Toggle the element
	m_pWindow->setEnabled( bEnabled );
}

bool CGUIElement_Impl::IsEnabled( void )
{
	return !m_pWindow->isDisabled();
}

void CGUIElement_Impl::SetZOrderingEnabled( bool bZOrderingEnabled )
{
	// Toggle the element z-ordering
	m_pWindow->setZOrderingEnabled( bZOrderingEnabled );
}

bool CGUIElement_Impl::IsZOrderingEnabled( void )
{
	return m_pWindow->isZOrderingEnabled();
}

void CGUIElement_Impl::BringToFront( void )
{
	// Bring the element to the front
    m_pWindow->moveToFront();
}

void CGUIElement_Impl::SendToBack( void )
{
	// Send the element to the back
    m_pWindow->moveToBack();
}

void CGUIElement_Impl::SetParent( CGUIElement_Impl * pParent )
{
	// Is the parent instance valid?
	if( pParent )
		((CGUIElement_Impl *)pParent)->m_pWindow->addChildWindow( m_pWindow );

	// Store the parent
	m_pParent = pParent;
}

void CGUIElement_Impl::SetText( String strText )
{
	// Set the text
	m_pWindow->setText( CGUI_Impl::GetUTFString( strText ) );
}

String CGUIElement_Impl::GetText( void )
{
	return String( CGUI_Impl::GetUTFString( m_pWindow->getText().c_str() ).c_str() );
}

void CGUIElement_Impl::SetProperty( String strProperty, String strValue )
{
	m_pWindow->setProperty( CGUI_Impl::GetUTFString( strProperty ), CGUI_Impl::GetUTFString( strValue ) );
}

String CGUIElement_Impl::GetProperty( String strProperty )
{
	return String( CGUI_Impl::GetUTFString( m_pWindow->getProperty( strProperty.Get() ).c_str() ).c_str() );
}

void CGUIElement_Impl::SetPosition( Vector2 vecPosition, bool bRelative )
{
	// Convert the position to a cegui point
	CEGUI::Point position( vecPosition.fX, vecPosition.fY );

	// Set the element position
	if( bRelative )
		m_pWindow->setPosition( CEGUI::Relative, position );
	else
		m_pWindow->setPosition( CEGUI::Absolute, position );

	// Correct the element edges
	CorrectEdges();
}

Vector2 CGUIElement_Impl::GetPosition( bool bRelative )
{
	CEGUI::Point position;

	// Get the element position
	if( bRelative )
		position = m_pWindow->getPosition( CEGUI::Relative );
	else
		position = m_pWindow->getPosition( CEGUI::Absolute );

	// Return the position as a Vector2 class
	return Vector2( position.d_x, position.d_y );
}

void CGUIElement_Impl::SetSize( Vector2 vecSize, bool bRelative )
{
	// Convert the size to a cegui size
	CEGUI::Size size( vecSize.fX, vecSize.fY );

	// Set the element size
	if( bRelative )
		m_pWindow->setSize( CEGUI::Relative, size );
	else
		m_pWindow->setSize( CEGUI::Absolute, size );

	// Correct the element edges
	CorrectEdges();
}

Vector2 CGUIElement_Impl::GetSize( bool bRelative )
{
	CEGUI::Size size;

	// Get the element position
	if( bRelative )
		size = m_pWindow->getSize( CEGUI::Relative );
	else
		size = m_pWindow->getSize( CEGUI::Absolute );

	// Return the size as a Vector2 class
	return Vector2( size.d_width, size.d_height );
}

void CGUIElement_Impl::SetMinimumSize( Vector2 vecSize )
{
	// Set the element metrics mode
	m_pWindow->setMetricsMode( CEGUI::Absolute );

	// Set the element minimum size
	m_pWindow->setMinimumSize( CEGUI::Size( vecSize.fX, vecSize.fY ) );
}

Vector2 CGUIElement_Impl::GetMinimumSize( void )
{
	// Get the element minimum size
	CEGUI::Size minSize = m_pWindow->getMinimumSize();
	return Vector2( minSize.d_width, minSize.d_height );
}

void CGUIElement_Impl::SetMaximumSize( Vector2 vecSize )
{
	// Set the element metrics mode
	m_pWindow->setMetricsMode( CEGUI::Absolute );

	// Set the element maximum size
	m_pWindow->setMaximumSize( CEGUI::Size( vecSize.fX, vecSize.fY ) );
}

Vector2 CGUIElement_Impl::GetMaximumSize( void )
{
	// Get the element minimum size
	CEGUI::Size maxSize = m_pWindow->getMaximumSize();
	return Vector2( maxSize.d_width, maxSize.d_height );
}

void CGUIElement_Impl::SetAlpha( float fAlpha )
{
	// Set the elment alpha
	m_pWindow->setAlpha( fAlpha );
}

float CGUIElement_Impl::GetAlpha( void )
{
	return m_pWindow->getAlpha();
}

void CGUIElement_Impl::SetInheritsAlpha( bool bInheritsAlpha )
{
	// Toggle inherits alpha
	m_pWindow->setInheritsAlpha( bInheritsAlpha );
}

bool CGUIElement_Impl::InheritsAlpha( void )
{
	return m_pWindow->inheritsAlpha();
}

void CGUIElement_Impl::SetAlwaysOnTop( bool bAlwaysOnTop )
{
	// Toggle always on top
	m_pWindow->setAlwaysOnTop( bAlwaysOnTop );
}

bool CGUIElement_Impl::IsAlwaysOnTop( void )
{
	return m_pWindow->isAlwaysOnTop();
}

void CGUIElement_Impl::Activate( void )
{
	// Activate the window
	m_pWindow->activate();
}

void CGUIElement_Impl::Deactivate( void )
{
	// Deactivate the window
	m_pWindow->deactivate();
}

bool CGUIElement_Impl::IsActive( void )
{
	return m_pWindow->isActive();
}

void CGUIElement_Impl::ForceRedraw( void )
{
	// Force the element to redraw
	m_pWindow->forceRedraw();
}

void CGUIElement_Impl::CorrectEdges( void )
{
	// Is this not a framewindow?
	if( m_pWindow->getType() != "CGUI/FrameWindow" )
		return;

	// Get the element position
	CEGUI::Point currentPoint = m_pWindow->getPosition( CEGUI::Absolute );

	// Get the element size
	CEGUI::Size currentSize = m_pWindow->getSize( CEGUI::Absolute );

	// Get the parent size
	CEGUI::Size parentSize = m_pWindow->getParent()->getSize( CEGUI::Absolute );

	// Ajust the left size
	if( currentPoint.d_x < CGUI_NODRAW_LEFT )
		currentPoint.d_x += (CGUI_NODRAW_LEFT - currentPoint.d_x);

	// Adjust the top
	if( currentPoint.d_y < CGUI_NODRAW_TOP )
		currentPoint.d_y += (CGUI_NODRAW_TOP - currentPoint.d_y);

	// Ajust the width
	if( (currentSize.d_width + currentPoint.d_x) > (parentSize.d_width - CGUI_NODRAW_RIGHT) )
		currentSize.d_width -= ((currentSize.d_width + currentPoint.d_x) - (parentSize.d_width - CGUI_NODRAW_RIGHT));

	// Ajust the height
	if( (currentSize.d_height + currentPoint.d_y) > (parentSize.d_height - CGUI_NODRAW_BOTTOM) )
		currentSize.d_height -= ((currentSize.d_height + currentPoint.d_y) - (parentSize.d_height - CGUI_NODRAW_BOTTOM));

	// Update the element position and size
	m_pWindow->setPosition( CEGUI::Absolute, currentPoint );
	m_pWindow->setSize( CEGUI::Absolute, currentSize );
}

void CGUIElement_Impl::SetFont( const char * szFontName )
{
	try
	{
		// Set the font
		m_pWindow->setFont( CEGUI::String( szFontName ) );
	}
	catch ( CEGUI::Exception ) {}
}

const char * CGUIElement_Impl::GetFont( void )
{
	try
	{
		return m_pWindow->getFont()->getName().c_str();
	}
	catch ( CEGUI::Exception ) {}

	return "";
}

bool CGUIElement_Impl::Event_OnClick( const CEGUI::EventArgs &e )
{
	// Call the click handler
	if( m_pfnClickHandler )
		m_pfnClickHandler( this );

	// Pass the event to the client gui manager
	if( pCore->GetClientScriptingManager() && pCore->GetClientScriptingManager()->GetScriptGUIManager() )
		pCore->GetClientScriptingManager()->GetScriptGUIManager()->HandleEvent( "onGuiElementClick", this );

	return true;
}

bool CGUIElement_Impl::Event_OnMove( const CEGUI::EventArgs &e )
{
	// Call the move handler
	if( m_pfnMoveHandler )
		m_pfnMoveHandler( this );

	// Pass the event to the client gui manager
	if( pCore->GetClientScriptingManager() && pCore->GetClientScriptingManager()->GetScriptGUIManager() )
		pCore->GetClientScriptingManager()->GetScriptGUIManager()->HandleEvent( "onGuiElementMove", this );

	return true;
}

bool CGUIElement_Impl::Event_OnResize( const CEGUI::EventArgs &e )
{
	// Call the resize handler
	if( m_pfnResizeHandler )
		m_pfnResizeHandler( this );

	// Pass the event to the client gui manager
	if( pCore->GetClientScriptingManager() && pCore->GetClientScriptingManager()->GetScriptGUIManager() )
		pCore->GetClientScriptingManager()->GetScriptGUIManager()->HandleEvent( "onGuiElementResize", this );

	return true;
}

bool CGUIElement_Impl::Event_OnMouseEnter( const CEGUI::EventArgs &e )
{
	// Call the mouse enter handler
	if( m_pfnMouseEnterHandler )
		m_pfnMouseEnterHandler( this );

	// Pass the event to the client gui manager
	if( pCore->GetClientScriptingManager() && pCore->GetClientScriptingManager()->GetScriptGUIManager() )
		pCore->GetClientScriptingManager()->GetScriptGUIManager()->HandleEvent( "onGuiElementMouseEnter", this );

	return true;
}

bool CGUIElement_Impl::Event_OnMouseLeave( const CEGUI::EventArgs &e )
{
	// Call the mouse leave handler
	if( m_pfnMouseLeaveHandler )
		m_pfnMouseLeaveHandler( this );

	// Pass the event to the client gui manager
	if( pCore->GetClientScriptingManager() && pCore->GetClientScriptingManager()->GetScriptGUIManager() )
		pCore->GetClientScriptingManager()->GetScriptGUIManager()->HandleEvent( "onGuiElementMouseLeave", this );

	return true;
}