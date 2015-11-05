/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIMessageBox_Impl.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

typedef void	( * ButtonClickHandler_t )( void * pUserData );

class CGUIMessageBox_Impl
{

private:

	CGUIWindow_Impl									* m_pWindow;
	CGUIStaticImage_Impl							* m_pIcon;
	CGUILabel_Impl									* m_pCaption;
	CGUIButton_Impl									* m_pButton[2];

	ButtonClickHandler_t							m_pfnButtonClickHandler[2];
	void											* m_pButtonClickUserData[2];

	bool				Event_OnButton1Click		( CGUIElement_Impl * pElement );
	bool				Event_OnButton2Click		( CGUIElement_Impl * pElement );

public:

						CGUIMessageBox_Impl			( class CGUI_Impl * pGUI, const char * szTitle, const char * szCaption, const char * szButton1 = "", const char * szButton2 = "" );
						~CGUIMessageBox_Impl		( void );

	void				SetVisible					( bool bVisible ) { m_pWindow->SetVisible ( bVisible ); }
	bool				IsVisible					( void ) { return m_pWindow->IsVisible(); }

	void				SetTitle					( const char * szTitle ) { m_pWindow->SetText ( szTitle ); }
	const char			* GetTitle					( void ) { return m_pWindow->GetText().Get(); }

	void				SetCaption					( const char * szCaption ) { m_pCaption->SetText ( szCaption ); }
	const char			* GetCaption				( void ) { return m_pCaption->GetText().Get(); }

	CGUIWindow_Impl		* GetWindow					( void ) { return m_pWindow; }

	void				SetButtonClickHandler		( int id, ButtonClickHandler_t pfnButtonClickHandler, void * pButtonClickUserData ) { m_pfnButtonClickHandler[ id ] = pfnButtonClickHandler; m_pButtonClickUserData[ id ] = pButtonClickUserData; }

};