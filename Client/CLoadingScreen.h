/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CLoadingScreen.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CLoadingScreen
{

private:

	static	CGUIStaticImage_Impl	* m_pLogo;
	static	CGUIStaticImage_Impl	* m_pLogoM2MP;
	static	CGUIStaticImage_Impl	* m_pLogoRakNet;

public:

	static	void					Start				( void );
	static	void					Finish				( void );
	static	void					Render				( void );

};