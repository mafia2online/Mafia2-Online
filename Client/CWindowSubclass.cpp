/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CWindowSubclass.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

bool	CWindowSubclass::m_bSubclassed = false;
WNDPROC CWindowSubclass::m_wWndProc;
CSquirrelArguments pArgs;

extern	CCore			* pCore;

// Original wndproc at 0xAB5FD0
LRESULT APIENTRY CWindowSubclass::WndProc_Hook( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// Are we focused on the game?
	bool bFocus = (GetForegroundWindow() == hWnd);

	// Prevent Mafia 2 knowing we minimized or lost focus
	if( uMsg == WM_KILLFOCUS || (uMsg == WM_ACTIVATE && LOWORD(wParam) == WA_INACTIVE) )
		return true;

	// Was the window closed?
	if( uMsg == WM_QUIT )
	{
		// Delete the core instance
		delete pCore;
	}

	// Have we gained focus?
	if( bFocus && !pCore->GetGame()->Focused() )
	{
		// Mark as focused
		pCore->GetGame()->SetFocus( true );

		// Show the cursor
		ShowCursor( true );

		// Is the client scripting manager active?
		if( pCore->GetClientScriptingManager() )
		{
			// Push true
			pArgs.push( false );

			// Call the client event
			pCore->GetClientScriptingManager()->GetEvents()->Call( "onClientFocusChange", &pArgs );

			// Clear the arguments
			pArgs.clear();
		}

		return true;
	}
	else if( !bFocus && pCore->GetGame()->Focused() )
	{
		// Makr as not focused
		pCore->GetGame()->SetFocus( false );

		// Hide the cursor
		ShowCursor( false );

		// Is the client scripting manager active?
		if( pCore->GetClientScriptingManager() )
		{
			// Push true
			pArgs.push( true );

			// Call the client event
			pCore->GetClientScriptingManager()->GetEvents()->Call( "onClientFocusChange", &pArgs );

			// Clear the arguments
			pArgs.clear();
		}

		return true;
	}

	// Are we focused?
	if( bFocus )
	{
		// Is the game loaded?
		if( pCore->IsGameLoaded() )
		{
			// Process the message with the gui
			pCore->GetGUI()->ProcessInput( uMsg, wParam, lParam );

			// Are we connected to a server and is the chat input not visible?
			if( pCore->GetNetworkModule()->IsConnected() && !pCore->GetChat()->IsInputVisible() && !pCore->GetGUI()->GetCEGUI()->IsInputEnabled() )
			{
				// Has the escape key been pressed?
				if( uMsg == WM_KEYDOWN && (DWORD)wParam == VK_ESCAPE )
				{
					// Toggle the main menu
					pCore->GetGUI()->GetMainMenu()->SetVisible( !pCore->GetGUI()->GetMainMenu()->IsVisible() );

					return true;
				}

				// Did we process this key with the localplayer?
				if( pCore->GetPlayerManager()->GetLocalPlayer()->ProcessControls( uMsg, wParam ) )
					return true;

				// Process this key with the key bind manager
				pCore->GetKeyBinds()->ProcessInput( uMsg, wParam, lParam );
			}
		}
	}

	// Pass this input back to mafia 2's window proc
	return CallWindowProc( m_wWndProc, hWnd, uMsg, wParam, lParam );
}

void CWindowSubclass::Subclass( HWND hWnd )
{
	// Are we not already subclassed?
	if( !m_bSubclassed )
	{
		// Subclass the window
		m_wWndProc = SubclassWindow( hWnd, WndProc_Hook );

		// Mark as subclassed
		m_bSubclassed = true;

		// Set the game hwnd
		pCore->SetGameHwnd( hWnd );
	}
}