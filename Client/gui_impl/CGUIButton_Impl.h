/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIButton_Impl.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CGUIButton_Impl : public CGUIElement_Impl
{

public:

						CGUIButton_Impl				( class CGUI_Impl * pGUI, String strCaption, CGUIElement_Impl * pParent = NULL );
						~CGUIButton_Impl			( void );

	eGUIType			GetType						( void ) { return GUI_BUTTON; }

};