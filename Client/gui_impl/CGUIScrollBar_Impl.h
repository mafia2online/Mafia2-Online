/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIScrollBar_Impl.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CGUIScrollBar_Impl : public CGUIElement_Impl
{

protected:

	GUI_CALLBACK									m_pfnOnScroll;

	bool				Event_OnScroll				( const CEGUI::EventArgs &e );

public:

						CGUIScrollBar_Impl			( class CGUI_Impl * pGUI, bool bHorizontal, CGUIElement_Impl * pParent = NULL );
						~CGUIScrollBar_Impl			( void );

	void				SetOnScrollHandler			( GUI_CALLBACK pfnOnScroll ) { m_pfnOnScroll = pfnOnScroll; }

	void				SetScrollPosition			( float fPosition );
	float				GetScrollPosition			( void );

	eGUIType			GetType						( void ) { return GUI_SCROLLBAR; }

};