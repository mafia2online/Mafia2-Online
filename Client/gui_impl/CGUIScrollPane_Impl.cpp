/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIScrollPane_Impl.cpp
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
#include "CGUIScrollPane_Impl.h"

CGUIScrollPane_Impl::CGUIScrollPane_Impl( CGUI_Impl * pGUI, CGUIElement_Impl * pParent )
	: CGUIElement_Impl(pGUI)
{
	String strName = pGUI->GetUniqueName();

	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/ScrollablePane", strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	m_pWindow->setRect( CEGUI::Relative, CEGUI::Rect( 0.9f, 0.9f, 0.9f, 0.9f ) );
	m_pWindow->setVisible( true );
	m_pWindow->setUserData( reinterpret_cast<void *>(this) );

	AddEvents();
	if( pParent )
		SetParent( pParent );
	else
	{
		pGUI->AddChild( this );
		SetParent( NULL );
	}
	SetHorizontalScrollStepSize( 100.0f );
	SetVerticalScrollStepSize( 100.0f );
}

CGUIScrollPane_Impl::~CGUIScrollPane_Impl( void )
{
	DestroyElement();
}

void CGUIScrollPane_Impl::SetHorizontalScrollBar( bool bEnabled )
{
	reinterpret_cast<CEGUI::ScrollablePane *>(m_pWindow)->setShowHorzScrollbar( bEnabled );
}

void CGUIScrollPane_Impl::SetVerticalScrollBar( bool bEnabled )
{
	reinterpret_cast<CEGUI::ScrollablePane *>(m_pWindow)->setShowVertScrollbar( bEnabled );
}

void CGUIScrollPane_Impl::SetHorizontalScrollBarPosition( float fPosition )
{
	reinterpret_cast<CEGUI::ScrollablePane *>(m_pWindow)->setHorizontalScrollPosition( fPosition );
}

float CGUIScrollPane_Impl::GetHorizontalScrollBarPosition( void )
{
	return reinterpret_cast<CEGUI::ScrollablePane *>(m_pWindow)->getHorizontalScrollPosition();
}

void CGUIScrollPane_Impl::SetVerticalScrollBarPosition( float fPosition )
{
	reinterpret_cast<CEGUI::ScrollablePane *>(m_pWindow)->setVerticalScrollPosition( fPosition );
}

float CGUIScrollPane_Impl::GetVerticalScrollBarPosition( void )
{
	return reinterpret_cast<CEGUI::ScrollablePane *>(m_pWindow)->getVerticalScrollPosition();
}

void CGUIScrollPane_Impl::SetHorizontalScrollStepSize( float fPosition )
{
	reinterpret_cast<CEGUI::ScrollablePane *>(m_pWindow)->setHorizontalStepSize( fPosition );
}

float CGUIScrollPane_Impl::GetHorizontalScrollStepSize( void )
{
	return reinterpret_cast<CEGUI::ScrollablePane *>(m_pWindow)->getHorizontalStepSize();
}

void CGUIScrollPane_Impl::SetVerticalScrollStepSize( float fPosition )
{
	reinterpret_cast<CEGUI::ScrollablePane *>(m_pWindow)->setVerticalStepSize( fPosition );
}

float CGUIScrollPane_Impl::GetVerticalScrollStepSize( void )
{
	return reinterpret_cast<CEGUI::ScrollablePane *>(m_pWindow)->getVerticalStepSize();
}