/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUI.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "CMafia.h"

#include "CGUI.h"

#include "gui_impl/CGUI_Impl.h"
#include "CDirectInput8Hook.h"

#include "CServerBrowser.h"
#include "CMainMenu.h"
#include "CDownloadProgress.h"
#include "CMenuSettings.h"

#include "CM2Camera.h"

#include "CClientScriptingManager.h" // \/
#include "CClientScriptGUIManager.h" // for delete clientscript gui (???)

#include "CLogFile.h"

#include "Strings/Unicode.h"

CGUI::CGUI( IDirect3DDevice9 * pDevice )
	: m_pDevice(pDevice)

	, m_pGUI(nullptr)
	, m_pDownloadProgress(nullptr)
	, m_pMainMenu(nullptr)
	, m_pServerBrowser(nullptr)
	, m_pSettings(nullptr)

	, m_MouseState()
	, m_vecCursorPosition()
	, m_byteButtonClicked()

	, m_bPreviousCameraState(false)
{
	memset( &m_vecCursorPosition, 0, sizeof(m_vecCursorPosition) );
	memset( m_byteButtonClicked, 0, sizeof(m_byteButtonClicked) );

	m_pGUI = new CGUI_Impl( pDevice );
}

CGUI::~CGUI( void )
{
	SAFE_DELETE( m_pSettings );
	SAFE_DELETE( m_pMainMenu );
	SAFE_DELETE( m_pServerBrowser );
	SAFE_DELETE( m_pDownloadProgress );
	SAFE_DELETE( m_pGUI );
}

void CGUI::SetupGUI( void )
{
	m_pServerBrowser = new CServerBrowser( m_pGUI );
	m_pMainMenu = new CMainMenu( m_pGUI );
	m_pDownloadProgress = new CDownloadProgress;
	m_pSettings = new CMenuSettings( m_pGUI );
}

void CGUI::SetCursorVisible(bool bVisible)
{
	CCore *pCore = CCore::Instance();
	CM2Camera *pGameCamera = pCore->GetCamera();
	if (pGameCamera)
	{
		if (bVisible)
		{
			if (IsCursorVisible())
			{
				return;
			}

			m_bPreviousCameraState = pGameCamera->IsLocked();
			pGameCamera->LockControl(true);
			CMafia *pGame = pCore->GetGame();

			if (pGame->IsMapOpen())
				pGame->OpenMap(false);
		}
		else
		{
			pGameCamera->LockControl(m_bPreviousCameraState);
			m_bPreviousCameraState = false;
		}
	}

	m_pGUI->SetCursorEnabled(bVisible);
}

bool CGUI::IsCursorVisible( void ) const
{
	return m_pGUI->IsCursorEnabled();
}

bool CGUI::DeleteAllClientScriptGUI( void )
{
	if ( CCore::Instance()->GetClientScriptingManager() && CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager() )
	{
		CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager()->Hide();
		//CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager()->DeleteAll(); // todo

		DEBUG_LOG("CGUI::HideAllClientScriptGUI > Hide all scriptGUI elements.");
		return true;
	}

	return false;
}

void CGUI::Render( void )
{
	// Render the gui
	m_pGUI->Draw();

	// Render the mouse cursor
	m_pGUI->DrawMouseCursor();

	// Pulse the serverbrowser
	if( m_pServerBrowser )
		m_pServerBrowser->Pulse();
}

void CGUI::ProcessMouse( IDirectInputDevice8 *pMouseDevice )
{
	// Process the mouse
	if( CCore::Instance()->GetGame()->Focused() && m_pGUI->IsCursorEnabled() )
	{
		// If we've lose the input device, re-aquire it
		if( pMouseDevice->GetDeviceState( sizeof(m_MouseState), (LPVOID)&m_MouseState ) == DIERR_INPUTLOST )
		{
			// Acquire the input device
			pMouseDevice->Acquire();
			return;
		}

		// Handle mouse position changes
		m_vecCursorPosition.left += m_MouseState.lX;

		// Clamp the mouse position on the screen
		if(m_vecCursorPosition.left > (long)m_pGUI->GetResolution().fX)
			m_vecCursorPosition.left = (long)m_pGUI->GetResolution().fX;
		else if(m_vecCursorPosition.left < 0)
			m_vecCursorPosition.left = 0;

		// Handle mouse position changes
		m_vecCursorPosition.top += m_MouseState.lY;

		// Clamp the mouse position on the screen
		if(m_vecCursorPosition.top > (long)m_pGUI->GetResolution().fY)
			m_vecCursorPosition.top = (long)m_pGUI->GetResolution().fY;
		else if(m_vecCursorPosition.top < 0)
			m_vecCursorPosition.top = 0;

		// Process mouse movement
		m_pGUI->ProcessMouseInput( eMouseType::POSITION, (float)m_vecCursorPosition.left, (float)m_vecCursorPosition.top );

		// Sync windows cursor with the games
		SetCursorPos( m_vecCursorPosition.left, m_vecCursorPosition.top );

		// Process mouse wheel
		if( m_MouseState.lZ != 0 )
			m_pGUI->ProcessMouseInput( eMouseType::SCROLL, (float)m_MouseState.lZ, 0.0f );

		// Check the mouse buttons for changes
		for( BYTE i = 0; i < 8; i++ )
		{
			// Has the current button been changed?
			if( m_MouseState.rgbButtons[ i ] & 0x80 )
			{
				// Have we not already processed this change?
				if( !m_byteButtonClicked[ i ] )
				{
					// Process the button down
					if( i < 3 )
						m_pGUI->ProcessMouseInput( eMouseType::BUTTON_DOWN, 0.0f, 0.0f, i );

					// Store the button information
					m_byteButtonClicked[ i ] = 1;
				}
			}
			else
			{
				// Have we already seen this change?
				if( m_byteButtonClicked[ i ] )
				{
					// Process the button up
					if( i < 3 )
						m_pGUI->ProcessMouseInput( eMouseType::BUTTON_UP, 0.0f, 0.0f, i );

					// Store the button information
					m_byteButtonClicked[ i ] = 0;
				}
			}
		}
	}
}

void CGUI::OnDeviceLost( void )
{
	// Pass it to cegui
	m_pGUI->OnDeviceLost();
}

void CGUI::OnDeviceReset( void )
{
	// Pass it to cegui
	m_pGUI->OnDeviceRestore();

	// Get the new resolution
	float fX = m_pGUI->GetResolution().fX;
	float fY = m_pGUI->GetResolution().fY;

	// Adjust the gui elements size and position
	m_pSettings->OnScreenSizeChange ( fX, fY );
	m_pMainMenu->OnScreenSizeChange ( fX, fY );
	m_pDownloadProgress->OnScreenSizeChange ( fX, fY );
}

bool CGUI::InputGoesToGUI( void )
{
	// Is the gui not valid?
	if( !m_pGUI )
		return false;

	// Check if input should be going to gui
	return (CCore::Instance()->GetGame()->Focused() || !ChatBox::Instance()->IsInputActive());
}

unsigned long CGUI::TranslateFromScanCode( DWORD dwKey )
{
	switch( dwKey )
	{
	case VK_HOME:       return DIK_HOME;
    case VK_END:        return DIK_END;
    case VK_RETURN:     return DIK_RETURN;
    case VK_TAB:        return DIK_TAB;
    case VK_BACK:       return DIK_BACK;
    case VK_LEFT:       return DIK_LEFTARROW;
    case VK_RIGHT:      return DIK_RIGHTARROW;
    case VK_UP:         return DIK_UPARROW;
    case VK_DOWN:       return DIK_DOWNARROW;
    case VK_LSHIFT:     return DIK_LSHIFT;
    case VK_RSHIFT:     return DIK_RSHIFT;
    case VK_SHIFT:      return DIK_LSHIFT;
    case VK_CONTROL:    return DIK_LCONTROL;
    case VK_DELETE:     return DIK_DELETE;
    case 0x56:          return DIK_V;
    case 0x43:          return DIK_C;
    case 0x58:          return DIK_X;
    case 0x41:          return DIK_A;
    default:            return 0;
	}
}

bool CGUI::ProcessInput( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// Switch the message
	switch( uMsg )
	{
	case WM_KEYDOWN:
        {
			// Convert the key to a cegui scan code
			DWORD dwKey = TranslateFromScanCode( wParam );

			// Process the key with the gui if it's valid
            if( dwKey > 0 )
				m_pGUI->ProcessKeyboardInput( dwKey, true );

			break;
		}

	case WM_KEYUP:
		{
			// Convert the key to a cegui scan code
			DWORD dwKey = TranslateFromScanCode( wParam );

			// Process the key with the gui if it's valid
			if( dwKey > 0 )
				m_pGUI->ProcessKeyboardInput( dwKey, false );

			break;
		}

	case WM_CHAR:
		{
			CEGUI::System::getSingleton().injectChar ( Unicode::UTF16CharacterToUTF32(static_cast<unsigned>(wParam)) );
			break;
		}
	}

	// We didn't handle this
	return false;
}