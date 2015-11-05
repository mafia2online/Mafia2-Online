/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUILabel_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"../StdInc.h"

CGUILabel_Impl::CGUILabel_Impl( CGUI_Impl * pGUI, String strText, CGUIFont_Impl * pFont, CGUIElement_Impl * pParent )
{
	// Store the manager instance
	m_pManager = pGUI;

	// Get the default font
	m_pFont = (pFont == NULL ? pGUI->GetDefaultFont() : pFont);

	// Get a unique name for cegui
	String strName = pGUI->GetUniqueName();

	// Create the window and set default settings
	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/StaticText", strName.Get() );
	((CEGUI::StaticText *)m_pWindow)->setBackgroundEnabled( false );
	((CEGUI::StaticText *)m_pWindow)->setFrameEnabled( false );
	m_pWindow->setDestroyedByParent( false );
	m_pWindow->setZOrderingEnabled( false );
	m_pWindow->setVisible( true );
	m_pWindow->setFont( m_pFont->GetFont() );

	// Store the pointer to this element
	m_pWindow->setUserData( (void *)this );

	// Set default variables
	SetVerticalAlignment( CEGUI::VerticalAlignment::VA_TOP );
	SetHorizontalAlignment( CEGUI::HorizontalAlignment::HA_LEFT );
	SetText( strText );

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

CGUILabel_Impl::~CGUILabel_Impl( void )
{
	// Destroy the element
	DestroyElement();
}

void CGUILabel_Impl::SetText( String strText )
{
	// Set the element text
	m_pWindow->setText( CGUI_Impl::GetUTFString( strText.Get() ) );

	// Rescale the label if we have auto-sizing enabled
	if ( m_bAutoSizeEnabled )
		SetSize( Vector2( m_pFont->GetTextExtent( CGUI_Impl::GetUTFString( strText.Get() ).c_str() ), m_pFont->GetFontHeight() ) );
}

void CGUILabel_Impl::SetVerticalAlignment( CEGUI::VerticalAlignment alignment )
{
	((CEGUI::StaticText *)m_pWindow)->setVerticalAlignment( alignment );
}

CEGUI::VerticalAlignment CGUILabel_Impl::GetVerticalAlignment( void )
{
	return ((CEGUI::StaticText *)m_pWindow)->getVerticalAlignment();
}

void CGUILabel_Impl::SetHorizontalAlignment( CEGUI::HorizontalAlignment alignment )
{
	((CEGUI::StaticText *)m_pWindow)->setHorizontalAlignment( alignment );
}

CEGUI::HorizontalAlignment CGUILabel_Impl::GetHorizontalAlignment( void )
{
	return ((CEGUI::StaticText *)m_pWindow)->getHorizontalAlignment();
}

void CGUILabel_Impl::SetTextColour( CColor colour )
{
	((CEGUI::StaticText *)m_pWindow)->setTextColours( CEGUI::colour( (1.0f / 255.0f * colour.R), (1.0f / 255.0f * colour.G), (1.0f / 255.0f * colour.B), (1.0f / 255.0f * colour.A) ) );
}

CColor CGUILabel_Impl::GetTextColour( void )
{
	// Get the text colour
	CEGUI::colour colour = ((CEGUI::StaticText *)m_pWindow)->getTextColours().getColourAtPoint(0, 0);

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
		return m_pFont->GetTextExtent( ((CEGUI::StaticText *)m_pWindow)->getText().c_str() );
	}
	catch ( CEGUI::Exception ) {}

	return 0.0f;
}