/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIElement_Impl.cpp
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
#include "CGUIElement_Impl.h"

// Nodrawing zones
#define CGUI_NODRAW_LEFT	9.0f
#define CGUI_NODRAW_RIGHT	9.0f
#define CGUI_NODRAW_TOP		9.0f
#define CGUI_NODRAW_BOTTOM	9.0f

CGUIElement_Impl::CGUIElement_Impl( CGUI_Impl *gui )
{
	m_pWindow = nullptr;
	m_pParent = nullptr;
	m_pManager = gui;

	m_pManager->NotifyElementCreate(this);
}

CGUIElement_Impl::~CGUIElement_Impl( void )
{
	assert(m_pManager);
	m_pManager->NotifyElementDestroy(this);
}

void CGUIElement_Impl::AddEvents( void )
{
	m_pWindow->subscribeEvent( CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber( &CGUIElement_Impl::Event_OnClick, this ) );
	m_pWindow->subscribeEvent( CEGUI::Window::EventMouseDoubleClick, CEGUI::Event::Subscriber( &CGUIElement_Impl::Event_OnDoubleClick, this));
	m_pWindow->subscribeEvent( CEGUI::Window::EventMoved, CEGUI::Event::Subscriber( &CGUIElement_Impl::Event_OnMove, this ) );
	m_pWindow->subscribeEvent( CEGUI::Window::EventSized, CEGUI::Event::Subscriber( &CGUIElement_Impl::Event_OnResize, this ) );
	m_pWindow->subscribeEvent( CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber( &CGUIElement_Impl::Event_OnMouseEnter, this ) );
	m_pWindow->subscribeEvent( CEGUI::Window::EventMouseLeaves, CEGUI::Event::Subscriber( &CGUIElement_Impl::Event_OnMouseLeave, this ) );
}

void CGUIElement_Impl::DestroyElement( void )
{
	m_pManager->RemoveFromRedrawQueue( reinterpret_cast<CGUIElement_Impl *>(m_pWindow->getUserData()) );
	m_pWindow->setUserData ( NULL );
	m_pManager->GetWindowManager()->destroyWindow( m_pWindow );
}

void CGUIElement_Impl::SetVisible( bool bVisible )
{
	m_pWindow->setVisible( bVisible );
}

bool CGUIElement_Impl::IsVisible( void )
{
	return m_pWindow->isVisible();
}

void CGUIElement_Impl::SetEnabled( bool bEnabled )
{
	m_pWindow->setEnabled( bEnabled );
}

bool CGUIElement_Impl::IsEnabled( void )
{
	return !m_pWindow->isDisabled();
}

void CGUIElement_Impl::SetZOrderingEnabled( bool bZOrderingEnabled )
{
	m_pWindow->setZOrderingEnabled( bZOrderingEnabled );
}

bool CGUIElement_Impl::IsZOrderingEnabled( void )
{
	return m_pWindow->isZOrderingEnabled();
}

void CGUIElement_Impl::BringToFront( void )
{
    m_pWindow->moveToFront();
}

void CGUIElement_Impl::SendToBack( void )
{
    m_pWindow->moveToBack();
}

void CGUIElement_Impl::SetParent( CGUIElement_Impl * pParent )
{
	if (pParent)
	{
		CGUIElement_Impl* pElement = dynamic_cast < CGUIElement_Impl* > (pParent);
		if (pElement)
			pElement->m_pWindow->addChildWindow(m_pWindow);
	}
	m_pParent = pParent;
}

void CGUIElement_Impl::SetText( String strText )
{
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
	CEGUI::Point position( vecPosition.fX, vecPosition.fY );

	if( bRelative )
		m_pWindow->setPosition( CEGUI::Relative, position );
	else
		m_pWindow->setPosition( CEGUI::Absolute, position );

	CorrectEdges();
}

Vector2 CGUIElement_Impl::GetPosition( bool bRelative )
{
	CEGUI::Point position;

	if( bRelative )
		position = m_pWindow->getPosition( CEGUI::Relative );
	else
		position = m_pWindow->getPosition( CEGUI::Absolute );

	return Vector2( position.d_x, position.d_y );
}

void CGUIElement_Impl::SetSize( Vector2 vecSize, bool bRelative )
{
	CEGUI::Size size( vecSize.fX, vecSize.fY );

	if( bRelative )
		m_pWindow->setSize( CEGUI::Relative, size );
	else
		m_pWindow->setSize( CEGUI::Absolute, size );

	CorrectEdges();
}

Vector2 CGUIElement_Impl::GetSize( bool bRelative )
{
	CEGUI::Size size;

	if( bRelative )
		size = m_pWindow->getSize( CEGUI::Relative );
	else
		size = m_pWindow->getSize( CEGUI::Absolute );

	return Vector2( size.d_width, size.d_height );
}

void CGUIElement_Impl::SetMinimumSize( Vector2 vecSize )
{
	m_pWindow->setMetricsMode( CEGUI::Absolute );
	m_pWindow->setMinimumSize( CEGUI::Size( vecSize.fX, vecSize.fY ) );
}

Vector2 CGUIElement_Impl::GetMinimumSize( void )
{
	CEGUI::Size minSize = m_pWindow->getMinimumSize();
	return Vector2( minSize.d_width, minSize.d_height );
}

void CGUIElement_Impl::SetMaximumSize( Vector2 vecSize )
{
	m_pWindow->setMetricsMode( CEGUI::Absolute );
	m_pWindow->setMaximumSize( CEGUI::Size( vecSize.fX, vecSize.fY ) );
}

Vector2 CGUIElement_Impl::GetMaximumSize( void )
{
	CEGUI::Size maxSize = m_pWindow->getMaximumSize();
	return Vector2( maxSize.d_width, maxSize.d_height );
}

void CGUIElement_Impl::SetAlpha( float fAlpha )
{
	m_pWindow->setAlpha( fAlpha );
}

float CGUIElement_Impl::GetAlpha( void )
{
	return m_pWindow->getAlpha();
}

void CGUIElement_Impl::SetInheritsAlpha( bool bInheritsAlpha )
{
	m_pWindow->setInheritsAlpha( bInheritsAlpha );
}

bool CGUIElement_Impl::InheritsAlpha( void )
{
	return m_pWindow->inheritsAlpha();
}

void CGUIElement_Impl::SetAlwaysOnTop( bool bAlwaysOnTop )
{
	m_pWindow->setAlwaysOnTop( bAlwaysOnTop );
}

bool CGUIElement_Impl::IsAlwaysOnTop( void )
{
	return m_pWindow->isAlwaysOnTop();
}

void CGUIElement_Impl::Activate( void )
{
	m_pWindow->activate();
}

void CGUIElement_Impl::Deactivate( void )
{
	m_pWindow->deactivate();
}

bool CGUIElement_Impl::IsActive( void )
{
	return m_pWindow->isActive();
}

void CGUIElement_Impl::ForceRedraw( void )
{
	m_pWindow->forceRedraw();
}

void CGUIElement_Impl::CorrectEdges( void )
{
	if( m_pWindow->getType() != "CGUI/FrameWindow" || m_pWindow->getType() == "CGUI/StaticText")
		return;

	CEGUI::Point currentPoint = m_pWindow->getPosition( CEGUI::Absolute );
	CEGUI::Size currentSize = m_pWindow->getSize( CEGUI::Absolute );
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

	m_pWindow->setPosition( CEGUI::Absolute, currentPoint );
	m_pWindow->setSize( CEGUI::Absolute, currentSize );
}

void CGUIElement_Impl::SetFont( const char * szFontName )
{
	try
	{
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
	if( m_pfnClickHandler )
		m_pfnClickHandler(this);

	if( CCore::Instance()->GetClientScriptingManager() && CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager() )
		CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager()->HandleEvent( "onGuiElementClick", this );

	return true;
}

bool CGUIElement_Impl::Event_OnDoubleClick(const CEGUI::EventArgs &e)
{
	if (m_pfnDoubleClickHandler)
		m_pfnDoubleClickHandler(this);

	if (CCore::Instance()->GetClientScriptingManager() && CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager())
		CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager()->HandleEvent("onGuiElementDoubleClick", this);

	return true;
}

bool CGUIElement_Impl::Event_OnMove( const CEGUI::EventArgs &e )
{
	if( m_pfnMoveHandler )
		m_pfnMoveHandler( this );

	if( CCore::Instance()->GetClientScriptingManager() && CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager() )
		CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager()->HandleEvent( "onGuiElementMove", this );

	return true;
}

bool CGUIElement_Impl::Event_OnResize( const CEGUI::EventArgs &e )
{
	if( m_pfnResizeHandler )
		m_pfnResizeHandler( this );

	if( CCore::Instance()->GetClientScriptingManager() && CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager() )
		CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager()->HandleEvent( "onGuiElementResize", this );

	return true;
}

bool CGUIElement_Impl::Event_OnMouseEnter( const CEGUI::EventArgs &e )
{
	if( m_pfnMouseEnterHandler )
		m_pfnMouseEnterHandler( this );

	if( CCore::Instance()->GetClientScriptingManager() && CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager() )
		CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager()->HandleEvent( "onGuiElementMouseEnter", this );

	return true;
}

bool CGUIElement_Impl::Event_OnMouseLeave( const CEGUI::EventArgs &e )
{
	if( m_pfnMouseLeaveHandler )
		m_pfnMouseLeaveHandler( this );

	if( CCore::Instance()->GetClientScriptingManager() && CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager() )
		CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager()->HandleEvent( "onGuiElementMouseLeave", this );

	return true;
}