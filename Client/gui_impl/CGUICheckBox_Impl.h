/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUICheckBox_Impl.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CGUICheckBox_Impl : public CGUIElement_Impl
{

public:

						CGUICheckBox_Impl			( class CGUI_Impl * pGUI, String strCaption, CGUIElement_Impl * pParent = NULL );
						~CGUICheckBox_Impl			( void );

	void				SetChecked					( bool bChecked );
	bool				IsChecked					( void );

	eGUIType			GetType						( void ) { return GUI_CHECKBOX; }

};