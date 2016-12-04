/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIFont_Impl.cpp
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
#include "CGUIFont_Impl.h"

#include "Strings/UnicodeRanges.h"
#include "Strings/Unicode.h"

CGUIFont_Impl::CGUIFont_Impl( CGUI_Impl * pManager, String strFontName, String strFile, unsigned int uiSize, unsigned int uFlags, bool bAutoScale )
{
	m_pFontManager = pManager->GetFontManager();

	try
	{
		m_pFont = m_pFontManager->createFont( strFontName.Get(), strFile.Get(), uiSize, uFlags );

		CEGUI::String codepoints;

		for (size_t i = 0; i < ARRAYSIZE(unicodeRanges); ++i) {
			const size_t codepointsCount = static_cast<size_t>(unicodeRanges[i][1] - unicodeRanges[i][0]);
			codepoints.reserve(codepointsCount);
			for (unsigned cp = unicodeRanges[i][0]; cp < unicodeRanges[i][1]; ++cp) {
				codepoints.append(1, static_cast<CEGUI::utf32>(cp));
			}
		}
		m_pFont->defineFontGlyphs(codepoints);
	}
	catch ( CEGUI::RendererException ) {
		MessageBox(NULL, "Failed to create GUI font!", "Fatal error", MB_ICONERROR);
	}

	SetNativeResolution( 1024, 768 );
	SetAutoScalingEnabled( bAutoScale );
}

CGUIFont_Impl::~CGUIFont_Impl( void )
{
	m_pFontManager->destroyFont( m_pFont );
}

void CGUIFont_Impl::SetAntiAliasingEnabled( bool bAntiAliasingEnabled )
{
	m_pFont->setAntiAliased( bAntiAliasingEnabled );
}

bool CGUIFont_Impl::IsAntiAliasingEnabled( void )
{
	return m_pFont->isAntiAliased();
}

void CGUIFont_Impl::SetAutoScalingEnabled( bool bAutoScalingEnabled )
{
	m_pFont->setAutoScalingEnabled( bAutoScalingEnabled );
}

bool CGUIFont_Impl::IsAutoScalingEnabled( void )
{
	return m_pFont->isAutoScaled();
}

void CGUIFont_Impl::SetNativeResolution( unsigned int uiWidth, unsigned int uiHeight )
{
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

const size_t COLOR_CODE_LENGTH = 7;

static bool IsColorCode(const char *const text)
{
	if (strnlen(text, COLOR_CODE_LENGTH) < COLOR_CODE_LENGTH) {
		return false;
	}

	if (text[0] != '#') {
		return false;
	}

	for (size_t i = 0; i < 6; ++i) {
		if (!isxdigit(text[i + 1])) {
			return false;
		}
	}
	return true;
}


float CGUIFont_Impl::GetTextExtent( const char * szText, float fScale, const bool removeColorCodes /*= false*/ )
{
	CEGUI::String strText;

	const char *text = szText;
	while (*text) {

		if (removeColorCodes && IsColorCode(text)) {
			text += COLOR_CODE_LENGTH;
			continue;
		}

		unsigned advance = 0;
		const unsigned cp = Unicode::GetUTF8CodePoint(text, advance);
		if (IsNil(cp)) {
			break;
		}

		strText.append((CEGUI::utf8 *)text, advance);
		text += advance;
	}

	return m_pFont->getTextExtent( strText, fScale );
}

void CGUIFont_Impl::DrawString(const char * szText, CEGUI::Rect area, float fZ, CEGUI::Rect clip, CEGUI::TextFormatting format, unsigned long ulColour, CEGUI::Size scale )
{
	m_pFont->drawText( CEGUI::String( (CEGUI::utf8*)szText ), area, fZ, clip, format, CEGUI::ColourRect( CEGUI::colour( (CEGUI::argb_t)ulColour ) ), scale.d_width, scale.d_height );
}

void CGUIFont_Impl::DrawString( const char * szText, Vector2 vecPosition, CEGUI::TextFormatting format, unsigned long ulColour, float fScale )
{
	CEGUI::Rect drawRect( vecPosition.fX, vecPosition.fY, vecPosition.fX + 1000.0f, vecPosition.fY + 1000.0f );
	DrawString( szText, drawRect, 0.0f, drawRect, format, ulColour, CEGUI::Size( fScale, fScale ) );
}

bool ParseColor(CColor &color, const char *const text)
{
	if (!IsColorCode(text)) {
		return false;
	}

	unsigned red=0;
	unsigned green=0;
	unsigned blue=0;

	if (sscanf(text, "#%02X%02X%02X", &red, &green, &blue) != 3) {
		return false;
	}

	color = CColor(static_cast<unsigned char>(red), static_cast<unsigned char>(green), static_cast<unsigned char>(blue), 255);
	return true;
}

void CGUIFont_Impl::DrawColoredString( const char * szText, const Vector2 &vecPosition, CEGUI::TextFormatting format, unsigned long ulColour, float fScale /*= 1.0f*/ )
{
	String text;
	unsigned currentColor = ulColour;

	Vector2 currentPosition(vecPosition);
	unsigned advance = 0;
	for (size_t i = 0; ; i += advance) {
		if (IsColorCode(szText + i)) {
			if (! text.IsEmpty()) {
				DrawString(text, currentPosition, format, currentColor, fScale);
				currentPosition.fX += m_pFont->getTextExtent((CEGUI::utf8*)text.Get(), fScale);

				text.clear();
			}

			CColor color;
			ParseColor(color, szText + i);
			currentColor = color.ToUint();
			advance = COLOR_CODE_LENGTH;
			continue;
		}

		const unsigned cp = Unicode::GetUTF8CodePoint(szText, advance, i);
		if (IsNil(cp) || cp == 0) {
			break;
		}

		text.Append(szText + i, advance);
	}

	if (! text.IsEmpty()) {
		DrawString(text, currentPosition, format, currentColor, fScale);
		currentPosition.fX += m_pFont->getTextExtent((CEGUI::utf8*)text.Get(), fScale);
	}
}
