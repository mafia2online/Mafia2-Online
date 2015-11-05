/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIScrollPane_Impl.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CGUIScrollPane_Impl : public CGUIElement_Impl
{

public:

						CGUIScrollPane_Impl			( class CGUI_Impl * pGUI, CGUIElement_Impl * pParent = NULL );
						~CGUIScrollPane_Impl		( void );

	void				SetHorizontalScrollBar		( bool bEnabled );
	void				SetVerticalScrollBar		( bool bEnabled );

	void				SetHorizontalScrollBarPosition	( float fPosition );
	float				GetHorizontalScrollBarPosition	( void );

	void				SetVerticalScrollBarPosition ( float fPosition );
	float				GetVerticalScrollBarPosition ( void );

	void				SetHorizontalScrollStepSize	( float fStepSize );
	float				GetHorizontalScrollStepSize	( void );

	void				SetVerticalScrollStepSize	( float fStepSize );
	float				GetVerticalScrollStepSize	( void );

	eGUIType			GetType						( void ) { return GUI_SCROLLPANE; }

};