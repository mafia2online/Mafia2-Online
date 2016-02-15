/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CDownloadProgress.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"CGUI.h"
#include	"gui_impl\CGUI_Impl.h"
#include	"gui_impl\CGUIElement_Impl.h"
#include	"gui_impl\CGUIWindow_Impl.h"
#include	"gui_impl\CGUIProgressBar_Impl.h"

class CDownloadProgress
{

private:

	CGUIWindow_Impl							* m_pWindow;
	CGUIProgressBar_Impl					* m_pProgressBar;

public:

											CDownloadProgress( void );
											~CDownloadProgress( void );

	void									SetVisible( bool bVisible );
	bool									IsVisible( void );

	void									SetFileName( String strName );
	void									SetProgress( float fProgress );

	void									OnScreenSizeChange ( float fX, float fY );

};