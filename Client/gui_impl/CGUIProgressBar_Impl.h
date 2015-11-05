/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIProgressBar_Impl.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CGUIProgressBar_Impl : public CGUIElement_Impl
{

public:

						CGUIProgressBar_Impl		( class CGUI_Impl * pGUI, CGUIElement_Impl * pParent = NULL );
						~CGUIProgressBar_Impl		( void );

	void				SetProgress					( float fProgress );
	float				GetProgress					( void );

	eGUIType			GetType						( void ) { return GUI_PROGRESSBAR; }

};