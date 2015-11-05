/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Launcher
* File       : CGUI.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CGUI
{

private:

	HINSTANCE									m_hInstance;
	HWND										m_hwWindow;
	WNDCLASSEX									m_wndClass;

	std::map < const char *, HWND >				m_widgets;

public:

								CGUI							( void );
								~CGUI							( void );

	HWND						CreateWidget					( HWND hParent, const char * szName, const char * szType, const char * szTitle, float fX, float fY, float fWidth, float fHeight, const char * szFontName = "Arial", int iFontSize = 12, DWORD dwFlags = 0 );
	HWND						GetWidget						( const char * szName );

	void						SetWidgetText					( HWND hWidget, const char * szText );
	void						SetProgressValue				( int iValue );

	void						SetVisible						( bool bVisible );

	MSG							Loop							( void );
	static LRESULT CALLBACK		WndProc							( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

};