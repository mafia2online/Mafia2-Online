/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CWindowSubclass.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CMafia.h"
#include "Scripting/CSquirrel.h"
#include "CClientScriptingManager.h"
#include "CEvents.h"

#include "CAudioManager.h"

#include "CMainMenu.h"
#include "CChat.h"
#include "gui_impl/CGUI_Impl.h"
#include "CGUI.h"

#include "CNetworkModule.h"

#include "CLocalPlayer.h"

#include "CKeyBinds.h"

#include "CCore.h"

#include "CWindowSubclass.h"

bool	CWindowSubclass::m_bSubclassed = false;
WNDPROC CWindowSubclass::m_wWndProc;

// Original wndproc at 0xAB5FD0
LRESULT APIENTRY CWindowSubclass::WndProc_Hook( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( uMsg == WM_KILLFOCUS || (uMsg == WM_ACTIVATE && LOWORD(wParam) == WA_INACTIVE) )
		return true;

	CCore *pCore = CCore::Instance();
	if( uMsg == WM_QUIT ) {
		// TODO: Investigation why we are removing the core here.
		delete pCore;

		return CallWindowProc( m_wWndProc, hWnd, uMsg, wParam, lParam );
	}

	if (pCore && pCore->HandleMessage(uMsg, wParam, lParam)) {
		return 1;
	}
	return CallWindowProc( m_wWndProc, hWnd, uMsg, wParam, lParam );
}

void CWindowSubclass::Subclass( HWND hWnd )
{
	if( !m_bSubclassed )
	{
		m_wWndProc = SubclassWindow( hWnd, WndProc_Hook );
		m_bSubclassed = true;
		CCore::Instance()->SetGameHwnd( hWnd );
	}
}
