/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CLoadingScreen.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"BaseInc.h"

#include	"CCore.h"

#include	"CGUI.h"

#include	"CGraphics.h"

#include	"gui_impl\CGUI_Impl.h"
#include	"gui_impl\CGUIElement_Impl.h"
#include	"gui_impl\CGUIStaticImage_Impl.h"

#include	"SharedUtility.h"
#include	"Math\CVector3.h"

#include	"CLoadingScreen.h"

CGUIStaticImage_Impl	* CLoadingScreen::m_pLogo = NULL;
CGUIStaticImage_Impl	* CLoadingScreen::m_pLogoM2MP = NULL;
CGUIStaticImage_Impl	* CLoadingScreen::m_pLogoRakNet = NULL;

void CLoadingScreen::Start( void )
{
	// Create the gui image
	m_pLogo = CCore::Instance()->GetGUI()->GetCEGUI()->CreateStaticImage();
	m_pLogoM2MP = CCore::Instance()->GetGUI()->GetCEGUI()->CreateStaticImage();
	m_pLogoRakNet = CCore::Instance()->GetGUI()->GetCEGUI()->CreateStaticImage();

	// Logo Mafia2-Online
	m_pLogo->LoadFromFile("loadingscreen.png", SharedUtility::GetAbsolutePath("data\\gui\\images"));
	m_pLogo->SetPosition(Vector2(((CCore::Instance()->GetGUI()->GetCEGUI()->GetResolution().fX / 2) - 512), ((CCore::Instance()->GetGUI()->GetCEGUI()->GetResolution().fY / 2) - 256)));
	m_pLogo->SetSize(Vector2(1024, 512));
	m_pLogo->SetAlwaysOnTop(true);

	// Logo RakNet
	m_pLogoRakNet->LoadFromFile("logo_raknet.jpg", SharedUtility::GetAbsolutePath("data\\gui\\images"));
	m_pLogoRakNet->SetPosition(Vector2(((CCore::Instance()->GetGUI()->GetCEGUI()->GetResolution().fX / 2) + 700), ((CCore::Instance()->GetGUI()->GetCEGUI()->GetResolution().fY / 2) + 320)));
	m_pLogoRakNet->SetSize(Vector2(152, 152));
	m_pLogoRakNet->SetAlwaysOnTop(true);

	// Logo M2MP
	m_pLogoM2MP->LoadFromFile("logo_m2mp.png", SharedUtility::GetAbsolutePath("data\\gui\\images"));
	m_pLogoM2MP->SetPosition(Vector2(((CCore::Instance()->GetGUI()->GetCEGUI()->GetResolution().fX / 2) - 950), ((CCore::Instance()->GetGUI()->GetCEGUI()->GetResolution().fY / 2) + 270)));
	m_pLogoM2MP->SetSize(Vector2(500, 250));
	m_pLogoM2MP->SetAlwaysOnTop(true);
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
	if (m_pLogo && m_pLogoRakNet && m_pLogoM2MP)
	{
		// Draw a black box over the screen
		CCore::Instance()->GetGraphics()->DrawBox(0, 0, CCore::Instance()->GetGUI()->GetCEGUI()->GetResolution().fX, CCore::Instance()->GetGUI()->GetCEGUI()->GetResolution().fY, 0xFF000000);
	}
}