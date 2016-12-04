/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIScrollBar_Impl.cpp
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
#include "CGUIScrollBar_Impl.h"

CGUIScrollBar_Impl::CGUIScrollBar_Impl( CGUI_Impl * pGUI, bool bHorizontal, CGUIElement_Impl * pParent )
	: CGUIElement_Impl(pGUI)
{
	String strName = pGUI->GetUniqueName();

	m_pWindow = pGUI->GetWindowManager()->createWindow( (bHorizontal ? "CGUI/HorizontalScrollbar" : "CGUI/VerticalScrollbar"), strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	m_pWindow->setVisible( true );
	m_pWindow->setUserData( reinterpret_cast<void *>(this) );
	m_pWindow->subscribeEvent( CEGUI::Scrollbar::EventScrollPositionChanged, CEGUI::Event::Subscriber( &CGUIScrollBar_Impl::Event_OnScroll, this ) );

	AddEvents();
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
	DestroyElement();
}

void CGUIScrollBar_Impl::SetScrollPosition( float fPosition )
{
	reinterpret_cast<CEGUI::Scrollbar *>(m_pWindow)->setScrollPosition( fPosition );
}

float CGUIScrollBar_Impl::GetScrollPosition( void )
{
	return reinterpret_cast<CEGUI::Scrollbar *>(m_pWindow)->getScrollPosition();
}

bool CGUIScrollBar_Impl::Event_OnScroll( const CEGUI::EventArgs &e )
{
	if( m_pfnOnScroll )
		m_pfnOnScroll( this );

	return true;
}