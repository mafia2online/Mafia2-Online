/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CServerPassword.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include "CGUI.h"
#include "gui_impl/CGUI_Impl.h"
#include "gui_impl/CGUIElement_Impl.h"
#include "gui_impl/CGUIWindow_Impl.h"

typedef void	( * SubmitHandler_t )( String strPassword, void * pUserData );

class CServerPassword
{

private:

	std::shared_ptr<CGUIWindow_Impl>		m_pWindow;
	std::shared_ptr<CGUILabel_Impl>			m_pLabel;
	std::shared_ptr<CGUIEdit_Impl>			m_pEdit;
	std::shared_ptr<CGUIButton_Impl>		m_pSubmit;
	std::shared_ptr<CGUIButton_Impl>		m_pCancel;

	SubmitHandler_t							m_pfnSubmitHandler;
	void									* m_pSubmitHandlerUserData;

	bool									Event_OnSubmitClick				( CGUIElement_Impl * pElement );
	bool									Event_OnCancelClick				( CGUIElement_Impl * pElement );

public:

											CServerPassword					( CGUI_Impl * pGUI );
											~CServerPassword				( void );

	void									SetVisible						( bool bVisible );
	bool									IsVisible						( void ) { return m_pWindow->IsVisible (); }

	void									SetSubmitHandler				( SubmitHandler_t pfnSubmitHandler, void * pSubmitHandlerUserData ) { m_pfnSubmitHandler = pfnSubmitHandler; m_pSubmitHandlerUserData = pSubmitHandlerUserData; }

	void									OnScreenSizeChange				( float fX, float fY );

};