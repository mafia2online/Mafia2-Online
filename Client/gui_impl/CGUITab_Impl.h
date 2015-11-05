/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUITab_Impl.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CGUITab_Impl : public CGUIElement_Impl
{

public:

						CGUITab_Impl				( class CGUI_Impl * pGUI, String strCaption, CGUIElement_Impl * pParent );
						~CGUITab_Impl				( void );

	void				SetCaption					( String strCaption );

	void				SetVisible					( bool bVisible );
	bool				IsVisible					( void );

	void				SetEnabled					( bool bEnabled );
	bool				IsEnabled					( void );

	eGUIType			GetType						( void ) { return GUI_TAB; }

};