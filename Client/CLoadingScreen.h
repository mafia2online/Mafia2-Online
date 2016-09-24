/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CLoadingScreen.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include "gui_impl/CGUI_Impl.h"
#include "gui_impl/CGUIElement_Impl.h"
#include "gui_impl/CGUIStaticImage_Impl.h"

class CLoadingScreen
{

private:

	static std::shared_ptr<CGUIStaticImage_Impl> m_pLogo;
	static std::shared_ptr<CGUIStaticImage_Impl> m_pLogotypes;

public:

	static	void					Start				( void );
	static	void					Finish				( void );
	static	void					Render				( void );

};