/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CWindowSubclass.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CWindowSubclass
{

private:

	static	bool					m_bSubclassed;
	static	WNDPROC					m_wWndProc;

	static	LRESULT APIENTRY		WndProc_Hook					( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

public:

	static	void					Subclass						( HWND hWnd );

};