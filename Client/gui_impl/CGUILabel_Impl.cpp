/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUILabel_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"
#include "CCore.h"

#include "CColor.h"
#include "CString.h"
#include "Math/CVector3.h"

#include "CGUI.h"
#include "CGUICallback.h"
#include "CGUITypes.h"

#include "CClientScriptGUIManager.h"
#include "CClientScriptingManager.h"

#include "CGUI_Impl.h"
#include "CGUIFont_Impl.h"
#include "CGUIWindow_Impl.h"

#include "CGUILabel_Impl.h"

CGUILabel_Impl::CGUILabel_Impl( CGUI_Impl * pGUI, String strText, CGUIFont_Impl * pFont, CGUIElement_Impl * pParent )
	: CGUIElement_Impl(pGUI)
{
	m_pFont = (pFont == NULL ? pGUI->GetDefaultFont().get() : pFont);

	String strName = pGUI->GetUniqueName();

	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/StaticText", strName.Get() );
	reinterpret_cast<CEGUI::StaticText *>(m_pWindow)->setBackgroundEnabled( false );
	reinterpret_cast<CEGUI::StaticText *>(m_pWindow)->setFrameEnabled( false );
	m_pWindow->setDestroyedByParent( false );
	m_pWindow->setZOrderingEnabled( false );
	m_pWindow->setVisible( true );
	m_pWindow->setFont( m_pFont->GetFont() );

	m_pWindow->setUserData( reinterpret_cast<void *>(this) );

	SetVerticalAlignment( CEGUI::VerticalAlignment::VA_TOP );
	SetHorizontalAlignment( CEGUI::HorizontalAlignment::HA_LEFT );
	SetText( strText );

	AddEvents();
	if( pParent )
		SetParent( pParent );
	else
	{
		pGUI->AddChild( this );
		SetParent( NULL );
	}
}

CGUILabel_Impl::~CGUILabel_Impl( void )
{
	DestroyElement();
}

void CGUILabel_Impl::SetText( String strText )
{
	m_pWindow->setText( CGUI_Impl::GetUTFString( strText.Get() ) );
	if ( m_bAutoSizeEnabled )
		SetSize( Vector2( m_pFont->GetTextExtent( CGUI_Impl::GetUTFString( strText.Get() ).c_str() ), m_pFont->GetFontHeight() ) );
}

void CGUILabel_Impl::SetVerticalAlignment( CEGUI::VerticalAlignment alignment )
{
	reinterpret_cast<CEGUI::StaticText *>(m_pWindow)->setVerticalAlignment( alignment );
}

CEGUI::VerticalAlignment CGUILabel_Impl::GetVerticalAlignment( void )
{
	return reinterpret_cast<CEGUI::StaticText *>(m_pWindow)->getVerticalAlignment();
}

void CGUILabel_Impl::SetHorizontalAlignment( CEGUI::HorizontalAlignment alignment )
{
	reinterpret_cast<CEGUI::StaticText *>(m_pWindow)->setHorizontalAlignment( alignment );
}

CEGUI::HorizontalAlignment CGUILabel_Impl::GetHorizontalAlignment( void )
{
	return reinterpret_cast<CEGUI::StaticText *>(m_pWindow)->getHorizontalAlignment();
}

void CGUILabel_Impl::SetTextColour( CColor colour )
{
	reinterpret_cast<CEGUI::StaticText *>(m_pWindow)->setTextColours( CEGUI::colour( (1.0f / 255.0f * colour.R), (1.0f / 255.0f * colour.G), (1.0f / 255.0f * colour.B), (1.0f / 255.0f * colour.A) ) );
}

CColor CGUILabel_Impl::GetTextColour( void )
{
	CEGUI::colour colour = reinterpret_cast<CEGUI::StaticText *>(m_pWindow)->getTextColours().getColourAtPoint(0, 0);

	return CColor( (unsigned char)(colour.getRed() * 255), (unsigned char)(colour.getGreen() * 255), (unsigned char)(colour.getBlue() * 255), (unsigned char)(colour.getAlpha() * 255) );
}

float CGUILabel_Impl::GetFontHeight( void )
{
	return m_pFont->GetFontHeight();
}

float CGUILabel_Impl::GetTextExtent( void )
{
	try
	{
		return m_pFont->GetTextExtent(reinterpret_cast<CEGUI::StaticText *>(m_pWindow)->getText().c_str() );
	}
	catch ( CEGUI::Exception ) {}

	return 0.0f;
}