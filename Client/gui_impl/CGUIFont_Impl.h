/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIFont_Impl.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CGUIFont_Impl
{

protected:

	CEGUI::Font										* m_pFont;
	CEGUI::FontManager								* m_pFontManager;

public:
						CGUIFont_Impl				( CGUI_Impl * pManager, String strFontName, String strFile, unsigned int uiSize = 8, unsigned int uFlags = 0, bool bAutoScale = false );
						~CGUIFont_Impl				( void );

	void				SetAntiAliasingEnabled		( bool bAntiAliasingEnabled );
	bool				IsAntiAliasingEnabled		( void );

	void				SetAutoScalingEnabled		( bool bAutoScaleEnabled );
	bool				IsAutoScalingEnabled		( void );

	void				SetNativeResolution			( unsigned int uiWidth, unsigned int uiHeight );

	float				GetCharacterWidth			( int iChar, float fScale = 1.0f );
	float				GetFontHeight				( float fScale = 1.0f );
	float				GetTextExtent				( const char * szText, float fScale = 1.0f );

	void				DrawString					( const char * szText, CEGUI::Rect area, float fZ, CEGUI::Rect clip, CEGUI::TextFormatting format, unsigned long ulColour, CEGUI::Size scale );
	void				DrawString					( const char * szText, Vector2 vecPosition, CEGUI::TextFormatting, unsigned long ulColour, float fScale = 1.0f );

	CEGUI::Font			* GetFont					( void ) { return m_pFont; }

};