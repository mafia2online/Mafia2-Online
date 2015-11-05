/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUITabPanel_Impl.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include "CGUITab_Impl.h"

class CGUITabPanel_Impl : public CGUIElement_Impl
{

protected:

	GUI_CALLBACK									m_pfnHandler;
	bool				Event_OnSelectionChange		( const CEGUI::EventArgs &e );

public:

						CGUITabPanel_Impl			( class CGUI_Impl * pGUI, CGUIElement_Impl * pParent = NULL );
						~CGUITabPanel_Impl			( void );

	CGUITab_Impl		* CreateTab					( String strCaption );
	void				DeleteTab					( CGUITab_Impl * pTab );

	void				SetSelectedTab				( CGUITab_Impl * pTab );
	CGUITab_Impl		* GetSelectedTab			( void );
	bool				IsTabSelected				( CGUITab_Impl * pTab );

	void				SetSelectionHandler			( GUI_CALLBACK pfnHandler ) { m_pfnHandler = pfnHandler; }

	eGUIType			GetType						( void ) { return GUI_TABPANEL; }

};