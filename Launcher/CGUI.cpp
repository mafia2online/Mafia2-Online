/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Launcher
* File       : CGUI.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "StdInc.h"

#include <CommCtrl.h>
#pragma comment(lib,"comctl32.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

extern CGUI * pGUI;

CGUI::CGUI ( void )
{
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC = (DWORD)ICC_STANDARD_CLASSES;
	InitCommonControlsEx ( &icc );

	icc.dwICC = (DWORD)ICC_PROGRESS_CLASS;
	InitCommonControlsEx ( &icc );

	// Reset window class data
	ZeroMemory(&m_wndClass, sizeof(WNDCLASSEX));
	m_wndClass.cbSize = sizeof(WNDCLASSEX);
	m_wndClass.style = CS_HREDRAW | CS_VREDRAW;
	m_wndClass.lpfnWndProc = (WNDPROC)WndProc; 
	m_wndClass.hInstance = m_hInstance;
	m_wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	m_wndClass.lpszClassName = "m2mpUpdate";
	//m_wndClass.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(ICO_101));

	// Register the window class
	RegisterClassEx ( &m_wndClass );

	// Calculate the main window position
	int ScreenX = GetSystemMetrics ( SM_CXSCREEN );
	int ScreenY = GetSystemMetrics ( SM_CYSCREEN );
	int iWidth = 416, iHeight = 116;

	// Create the main window
	m_hwWindow = CreateWindowEx ( NULL, "m2mpUpdate", "Mafia 2 Multiplayer", WS_MINIMIZEBOX | WS_CAPTION | WS_SYSMENU, ((ScreenX / 2) - (iWidth / 2)), ((ScreenY / 2) - (iHeight / 2)), iWidth, iHeight, NULL, NULL, m_hInstance, NULL );
	
	// Create the widgets
	CreateWidget ( m_hwWindow, "download_label", "Static", "", 11, 15, 250, 16, "Arial", 16, ES_LEFT );
	CreateWidget ( m_hwWindow, "total_label", "Static", "", 308, 18, 94, 14, "Arial", 14, ES_RIGHT );
	HWND hProgress = CreateWidget ( m_hwWindow, "progress_bar", PROGRESS_CLASS, "", 12, 43, 389, 30 );

	// Set the progress bar range
	SendMessage ( hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 100) );

	// Hide the window
	SetVisible ( false );
}

CGUI::~CGUI ( void )
{

}

HWND CGUI::CreateWidget ( HWND hParent, const char * szName, const char * szType, const char * szTitle, float fX, float fY, float fWidth, float fHeight, const char * szFontName, int iFontSize, DWORD dwFlags )
{
	HWND hWidget = CreateWindowEx ( NULL, szType, szTitle, WS_CHILD | WS_VISIBLE | dwFlags, fX, fY, fWidth, fHeight, hParent, NULL, m_hInstance, NULL );
	HFONT hFont = CreateFont ( iFontSize, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, szFontName );

	SendMessage ( hWidget, WM_SETFONT, (WPARAM)hFont, TRUE );
	
	return m_widgets [ szName ] = hWidget;
}

HWND CGUI::GetWidget ( const char * szName )
{
	// Loop over each widget
	for ( std::map < const char *, HWND >::iterator iter = m_widgets.begin (); iter != m_widgets.end (); iter ++ )
	{
		// Is this the widget we're looking for?
		if ( !strcmp ( (*iter).first, szName ) )
			return (*iter).second;
	}

	return NULL;
}

void CGUI::SetWidgetText ( HWND hWidget, const char * szText )
{
	// Set the widget text
	SendMessage ( hWidget, WM_SETTEXT, 0, (LPARAM)szText );
}

void CGUI::SetProgressValue ( int iValue )
{
	// Get the progress bar widget
	HWND hProgressBar = GetWidget ( "progress_bar" );

	// Did we fail to find the widget?
	if ( hProgressBar == NULL )
		return;

	// Set the progress bar value
	SendMessage ( hProgressBar, PBM_SETPOS, iValue, 0 );
}

void CGUI::SetVisible ( bool bVisible )
{
	// Toggle the window
	ShowWindow ( m_hwWindow, (bVisible ? SW_SHOWNORMAL : SW_HIDE) );
}

MSG CGUI::Loop ( void )
{
	MSG message;
	while ( GetMessage ( &message, NULL, 0, 0 ) )
	{
		TranslateMessage ( &message );
		DispatchMessage ( &message );
	}

	return message;
}

LRESULT CALLBACK CGUI::WndProc ( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch ( message )
	{
		// Should we destroy the window?
	case WM_DESTROY:
		{
			PostQuitMessage ( 0 );
			TerminateProcess ( GetCurrentProcess(), 0 );
			break;
		}

	/*	// Recolour labels
	case WM_CTLCOLORSTATIC:
		{
			if ( pGUI->GetWidget ( "download_label") == (HWND)lParam || pGUI->GetWidget ( "total_label" ) == (HWND)lParam )
			{
				SetBkMode ( (HDC)wParam, TRANSPARENT );
				RedrawWindow ( (HWND)lParam, NULL, NULL, RDW_ERASE );
				return (LRESULT) GetStockObject ( HOLLOW_BRUSH );
			}
			break;
		}*/
	}

	return DefWindowProc ( hWnd, message, wParam, lParam );
}