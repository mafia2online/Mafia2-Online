/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CDownloadProgress.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore * pCore;

CDownloadProgress::CDownloadProgress( void )
{
	// Create the gui window
	m_pWindow = pCore->GetGUI()->GetCEGUI()->CreateWnd( "Download Progress" );
	m_pWindow->SetPosition( Vector2( ((pCore->GetGUI()->GetCEGUI()->GetResolution().fX / 2) - 175), (pCore->GetGUI()->GetCEGUI()->GetResolution().fY - 100) ) );
	m_pWindow->SetSize( Vector2( 350, 56 ) );
	m_pWindow->SetMovable( false );
	m_pWindow->SetSizingEnabled( false );
	m_pWindow->SetVisible( false );

	// Create the progress bar
	m_pProgressBar = pCore->GetGUI()->GetCEGUI()->CreateProgressBar( m_pWindow );
	m_pProgressBar->SetPosition( Vector2( 2, 23 ) );
	m_pProgressBar->SetSize( Vector2( 340, 25 ) );
}

CDownloadProgress::~CDownloadProgress( void )
{
	// Delete the gui progress bar instance
	SAFE_DELETE( m_pProgressBar );

	// Delete the gui window instance
	SAFE_DELETE( m_pWindow );
}

void CDownloadProgress::SetVisible( bool bVisible )
{
	// Set the gui window visibility
	m_pWindow->SetVisible( bVisible );
}

bool CDownloadProgress::IsVisible( void )
{
	// Is the gui window visiible?
	return m_pWindow->IsVisible();
}

void CDownloadProgress::SetFileName( String strName )
{
	// Set the gui window title
	m_pWindow->SetText( String( "Download Progress - File: %s", strName.Get() ).Get() );
}

void CDownloadProgress::SetProgress( float fProgress )
{
	// Set the gui progress bar progress
	m_pProgressBar->SetProgress( fProgress );
}

void CDownloadProgress::OnScreenSizeChange ( float fX, float fY )
{
	// Adjust the window position
	m_pWindow->SetPosition( Vector2( ((fX / 2) - 175.0f), (fY - 100.0f) ) );
}