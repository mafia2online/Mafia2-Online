/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CLoadingScreen.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;
CGUIStaticImage_Impl	* CLoadingScreen::m_pLogo = NULL;
CGUIStaticImage_Impl	* CLoadingScreen::m_pLogoM2MP = NULL;
CGUIStaticImage_Impl	* CLoadingScreen::m_pLogoRakNet = NULL;

void CLoadingScreen::Start( void )
{
	// Create the gui image
	m_pLogo = pCore->GetGUI()->GetCEGUI()->CreateStaticImage();
	m_pLogoM2MP = pCore->GetGUI()->GetCEGUI()->CreateStaticImage();
	m_pLogoRakNet = pCore->GetGUI()->GetCEGUI()->CreateStaticImage();

	// Logo Mafia2-Online
	m_pLogo->LoadFromFile("loadingscreen.png", SharedUtility::GetAbsolutePath("data\\gui\\images"));
	m_pLogo->SetPosition(Vector2(((pCore->GetGUI()->GetCEGUI()->GetResolution().fX / 2) - 512), ((pCore->GetGUI()->GetCEGUI()->GetResolution().fY / 2) - 256)));
	m_pLogo->SetSize(Vector2(1024, 512));
	m_pLogo->SetAlwaysOnTop(true);
}

void CLoadingScreen::Finish( void )
{
	// Destroy the gui image instance
	SAFE_DELETE(m_pLogo);
	SAFE_DELETE(m_pLogoM2MP);
	SAFE_DELETE(m_pLogoRakNet);
}

void CLoadingScreen::Render ( void )
{
	// Is the image valid?
	if (m_pLogo)
	{
		// Draw a black box over the screen
		pCore->GetGraphics()->DrawBox ( 0, 0, pCore->GetGUI()->GetCEGUI()->GetResolution().fX, pCore->GetGUI()->GetCEGUI()->GetResolution().fY, 0xFF000000 );
	}
}