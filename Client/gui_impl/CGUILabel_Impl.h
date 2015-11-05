/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUILabel_Impl.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CGUILabel_Impl : public CGUIElement_Impl
{

protected:

	CGUIFont_Impl									* m_pFont;
	bool											m_bAutoSizeEnabled;

public:

						CGUILabel_Impl				( class CGUI_Impl * pGUI, String strText, CGUIFont_Impl * pFont = NULL, CGUIElement_Impl * pParent = NULL );
						~CGUILabel_Impl				( void );

	void				SetText						( String strText );

	void				SetVerticalAlignment		( CEGUI::VerticalAlignment alignment );
	CEGUI::VerticalAlignment GetVerticalAlignment	( void );

	void				SetHorizontalAlignment		( CEGUI::HorizontalAlignment alignment );
	CEGUI::HorizontalAlignment GetHorizontalAlignment ( void );

	void				SetTextColour				( CColor colour );
	CColor				GetTextColour				( void );

	float				GetFontHeight				( void );
	float				GetTextExtent				( void );

	void				SetAutoSizeEnabled			( bool bAutoSizeEnabled ) { m_bAutoSizeEnabled = bAutoSizeEnabled; }

	eGUIType			GetType						( void ) { return GUI_LABEL; }

};