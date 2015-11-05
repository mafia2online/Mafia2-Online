/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIWindow_Impl.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"CGUIElement_Impl.h"

class CGUIWindow_Impl : public CGUIElement_Impl
{

public:
						CGUIWindow_Impl				( class CGUI_Impl * pGUI, String strCaption, CGUIElement_Impl * pParent = NULL );
						~CGUIWindow_Impl			( void );

	void				SetMovable					( bool bMovable );
	bool				IsMovable					( void );

	void				SetSizingEnabled			( bool bSizingEnabled );
	bool				IsSizingEnabled				( void );

	void				SetFrameEnabled				( bool bFrameEnabled );
	bool				IsFrameEnabled				( void );

	void				SetCloseButtonEnabled		( bool bCloseButtonEnabled );
	bool				IsCloseButtonEnabled		( void );

	void				SetTitlebarEnabled			( bool bTitlebarEnabled );
	bool				IsTitlebarEnabled			( void );

	eGUIType			GetType						( void ) { return GUI_WINDOW; }

};