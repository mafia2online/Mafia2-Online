/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIFont_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"../StdInc.h"

CGUIFont_Impl::CGUIFont_Impl( CGUI_Impl * pManager, String strFontName, String strFile, unsigned int uiSize, unsigned int uFlags, bool bAutoScale )
{
	// Store the font manager
	m_pFontManager = pManager->GetFontManager();

	try
	{
		// Create the font
		m_pFont = m_pFontManager->createFont( strFontName.Get(), strFile.Get(), uiSize, uFlags );
	}
	catch ( CEGUI::RendererException ) {}

	// Set default attributes
	SetNativeResolution( 1024, 768 );
	SetAutoScalingEnabled( bAutoScale );
}

CGUIFont_Impl::~CGUIFont_Impl( void )
{
	// Destroy the font
	m_pFontManager->destroyFont( m_pFont );
}

void CGUIFont_Impl::SetAntiAliasingEnabled( bool bAntiAliasingEnabled )
{
	// Set the font anti-aliased
	m_pFont->setAntiAliased( bAntiAliasingEnabled );
}

bool CGUIFont_Impl::IsAntiAliasingEnabled( void )
{
	return m_pFont->isAntiAliased();
}

void CGUIFont_Impl::SetAutoScalingEnabled( bool bAutoScalingEnabled )
{
	// Set the font auto-scaled
	m_pFont->setAutoScalingEnabled( bAutoScalingEnabled );
}

bool CGUIFont_Impl::IsAutoScalingEnabled( void )
{
	return m_pFont->isAutoScaled();
}

void CGUIFont_Impl::SetNativeResolution( unsigned int uiWidth, unsigned int uiHeight )
{
	// Set the font native resolution
	m_pFont->setNativeResolution( CEGUI::Size( (float)uiWidth, (float)uiHeight) );
}

float CGUIFont_Impl::GetCharacterWidth( int iChar, float fScale )
{
	char szBuf[2];
	szBuf[0] = iChar;
	szBuf[1] = 0;

	return m_pFont->getTextExtent( szBuf, fScale );
}

float CGUIFont_Impl::GetFontHeight( float fScale )
{
	return (m_pFont->getFontHeight( fScale ) + 2.0f);
}

float CGUIFont_Impl::GetTextExtent( const char * szText, float fScale )
{
	CEGUI::String strText;

	if( szText )
		strText.assign( szText );

	return m_pFont->getTextExtent( strText, fScale );
}

void CGUIFont_Impl::DrawString(const char * szText, CEGUI::Rect area, float fZ, CEGUI::Rect clip, CEGUI::TextFormatting format, unsigned long ulColour, CEGUI::Size scale )
{
	m_pFont->drawText( CEGUI::String( szText ), area, fZ, clip, format, CEGUI::ColourRect( CEGUI::colour( (CEGUI::argb_t)ulColour ) ), scale.d_width, scale.d_height );
}

void CGUIFont_Impl::DrawString( const char * szText, Vector2 vecPosition, CEGUI::TextFormatting format, unsigned long ulColour, float fScale )
{
	CEGUI::Rect drawRect( vecPosition.fX, vecPosition.fY, vecPosition.fX + 1000.0f, vecPosition.fY + 1000.0f );
	DrawString( szText, drawRect, 0.0f, drawRect, format, ulColour, CEGUI::Size( fScale, fScale ) );
}