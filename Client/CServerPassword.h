/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CServerPassword.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

typedef void	( * SubmitHandler_t )( String strPassword, void * pUserData );

class CServerPassword
{

private:

	CGUIWindow_Impl							* m_pWindow;
	CGUILabel_Impl							* m_pLabel;
	CGUIEdit_Impl							* m_pEdit;
	CGUIButton_Impl							* m_pSubmit;
	CGUIButton_Impl							* m_pCancel;

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