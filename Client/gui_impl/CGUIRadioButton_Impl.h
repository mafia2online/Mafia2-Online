/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIRadioButton_Impl.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CGUIRadioButton_Impl : public CGUIElement_Impl
{

public:

						CGUIRadioButton_Impl		( class CGUI_Impl * pGUI, String strCaption, CGUIElement_Impl * pParent = NULL );
						~CGUIRadioButton_Impl		( void );

	void				SetChecked					( bool bChecked );
	bool				IsChecked					( void );

	eGUIType			GetType						( void ) { return GUI_RADIOBUTTON; }

};