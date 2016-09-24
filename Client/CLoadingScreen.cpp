/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CLoadingScreen.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "CGUI.h"

#include "CGraphics.h"

#include "gui_impl/CGUI_Impl.h"
#include "gui_impl/CGUIElement_Impl.h"
#include "gui_impl/CGUIStaticImage_Impl.h"

#include "SharedUtility.h"
#include "Math/CVector3.h"

#include "CLoadingScreen.h"

std::shared_ptr<CGUIStaticImage_Impl> CLoadingScreen::m_pLogo;
std::shared_ptr<CGUIStaticImage_Impl> CLoadingScreen::m_pLogotypes;

void CLoadingScreen::Start( void )
{
	float fX = CCore::Instance()->GetGUI()->GetCEGUI()->GetResolution().fX;
	float fY = CCore::Instance()->GetGUI()->GetCEGUI()->GetResolution().fY;
	float fXhalf = (fX / 2);
	float fYhalf = (fY / 2);
	float fPadding[2] = { 25.0, 25.0 };

	// Create the gui image
	m_pLogo = CCore::Instance()->GetGUI()->GetCEGUI()->CreateStaticImage();
	m_pLogotypes = CCore::Instance()->GetGUI()->GetCEGUI()->CreateStaticImage();

	// Logo Mafia2-Online
	m_pLogo->LoadFromFile("loadingscreen.png", SharedUtility::GetAbsolutePath("data\\gui\\images"));
	m_pLogo->SetPosition(Vector2((fXhalf - 512), (fYhalf - 256)));
	m_pLogo->SetSize(Vector2(1024, 512));
	m_pLogo->SetAlwaysOnTop(true);

	// Other logo
	m_pLogotypes->LoadFromFile("loadingscreen_logos.png", SharedUtility::GetAbsolutePath("data\\gui\\images"));
	m_pLogotypes->SetPosition(Vector2((fX - 493 - fPadding[0]), (fY - 85 - fPadding[1])));
	m_pLogotypes->SetSize(Vector2(493, 85));
	m_pLogotypes->SetAlwaysOnTop(true);
}

void CLoadingScreen::Finish( void )
{
	m_pLogo.reset();
	m_pLogotypes.reset();
}

void CLoadingScreen::Render ( void )
{
	if (m_pLogo && m_pLogotypes)
	{
		CCore::Instance()->GetGraphics()->DrawBox(0, 0, CCore::Instance()->GetGUI()->GetCEGUI()->GetResolution().fX, CCore::Instance()->GetGUI()->GetCEGUI()->GetResolution().fY, 0xFF000000);
	}
}