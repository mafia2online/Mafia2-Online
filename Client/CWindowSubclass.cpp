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
	bool bFocus = (GetForegroundWindow() == hWnd);

	if( uMsg == WM_KILLFOCUS || (uMsg == WM_ACTIVATE && LOWORD(wParam) == WA_INACTIVE) )
		return true;

	CCore *pCore = CCore::Instance();
	if( uMsg == WM_QUIT )
	{
		// TODO: Investigation why we are removing the core here..
		delete pCore;

		return CallWindowProc( m_wWndProc, hWnd, uMsg, wParam, lParam );
	}

	if( bFocus && !pCore->GetGame()->Focused() )
	{
		pCore->GetGame()->SetFocus( true );

		ShowCursor( true );

		if( pCore->GetClientScriptingManager() )
		{
			CSquirrelArguments args;
			args.push( false );
			pCore->GetClientScriptingManager()->GetEvents()->Call( "onClientFocusChange", &args );
		}

		pCore->GetAudioManager()->UnmuteAll();
		return true;
	}
	else if( !bFocus && pCore->GetGame()->Focused() )
	{
		pCore->GetGame()->SetFocus( false );

		ShowCursor( false );

		if( pCore->GetClientScriptingManager() )
		{
			CSquirrelArguments args;
			args.push( true );
			pCore->GetClientScriptingManager()->GetEvents()->Call( "onClientFocusChange", &args );
		}

		pCore->GetAudioManager()->MuteAll();
		return true;
	}

	if( bFocus )
	{
		if( pCore->IsGameLoaded() )
		{
			pCore->GetGUI()->ProcessInput( uMsg, wParam, lParam );

			if( pCore->GetNetworkModule()->IsConnected() && !pCore->GetChat()->IsInputVisible() && !pCore->GetGUI()->GetCEGUI()->IsInputEnabled() )
			{
				if( uMsg == WM_KEYDOWN && (DWORD)wParam == VK_ESCAPE )
				{
					pCore->GetGUI()->GetMainMenu()->SetVisible( !pCore->GetGUI()->GetMainMenu()->IsVisible() );
					return true;
				}

				if( CLocalPlayer::Instance()->ProcessControls( uMsg, wParam ) )
					return true;

				pCore->GetKeyBinds()->ProcessInput( uMsg, wParam, lParam );
			}
		}
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
