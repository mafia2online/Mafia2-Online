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
CGUIStaticImage_Impl	* CLoadingScreen::m_pImage = NULL;

void CLoadingScreen::Start( void )
{
	// Create the gui image
	m_pImage = pCore->GetGUI()->GetCEGUI()->CreateStaticImage();
	m_pImage->LoadFromFile( "loadingscreen.png", SharedUtility::GetAbsolutePath( "data\\gui\\images" ) );
	m_pImage->SetPosition( Vector2( ((pCore->GetGUI()->GetCEGUI()->GetResolution().fX / 2) - 512), ((pCore->GetGUI()->GetCEGUI()->GetResolution().fY / 2) - 256) ) );
	m_pImage->SetSize( Vector2( 1024, 512 ) );
	m_pImage->SetAlwaysOnTop( true );
}

void CLoadingScreen::Finish( void )
{
	// Destroy the gui image instance
	SAFE_DELETE( m_pImage );
}

void CLoadingScreen::Render ( void )
{
	// Is the image valid?
	if ( m_pImage )
	{
		// Draw a black box over the screen
		pCore->GetGraphics()->DrawBox ( 0, 0, pCore->GetGUI()->GetCEGUI()->GetResolution().fX, pCore->GetGUI()->GetCEGUI()->GetResolution().fY, 0xFF000000 );
	}
}