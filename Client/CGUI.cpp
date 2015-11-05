/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUI.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;

CGUI::CGUI( IDirect3DDevice9 * pDevice )
{
	// Store the d3d9 device
	m_pDevice = pDevice;

	// Setup the input device
	SetupInputDevice();

	// Create the gui instance
	m_pGUI = new CGUI_Impl( pDevice );

	// Reset
	m_bPreviousCameraState = false;
}

CGUI::~CGUI( void )
{
	// Delete the settings
	if( m_pSettings )
		SAFE_DELETE( m_pSettings );

	// Delete the main menu
	if( m_pMainMenu )
		SAFE_DELETE( m_pMainMenu );

	// Delete the server browser
	if( m_pServerBrowser )
		SAFE_DELETE( m_pServerBrowser );

	// Destroy the m2mp gui instancse
	if( m_pDownloadProgress )
		SAFE_DELETE( m_pDownloadProgress );

	// Shutdown the input device
	if( m_pInputDevice )
	{
		m_pInputDevice->Unacquire();
		m_pInputDevice->Release();
	}

	// Shutdown the input
	if( m_pInput )
		m_pInput->Release();

	// Destroy the gui instance
	if( m_pGUI )
		SAFE_DELETE( m_pGUI );
}

void CGUI::SetupInputDevice( void )
{
	// Reset mouse variables
	memset( &m_vecCursorPosition, 0, sizeof(RECT) );
	memset( m_byteButtonClicked, 0, sizeof(BYTE) * 8 );

	// Create the input
	DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION, IID_IDirectInput8A, (LPVOID *)&m_pInput, NULL );

	// Create the input device
	m_pInput->CreateDevice( GUID_SysMouse, &m_pInputDevice, NULL );

	// Set the input device as a mouse
	m_pInputDevice->SetDataFormat( &c_dfDIMouse2 );

	// Set the input device to non exclusive
	m_pInputDevice->SetCooperativeLevel( pCore->GetGameHwnd(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE );

	// Acquire the input device
	m_pInputDevice->Acquire();
}

void CGUI::SetupGUI( void )
{
	// Create the serverbrowser gui
	m_pServerBrowser = new CServerBrowser( m_pGUI );

	// Create the main menu gui
	m_pMainMenu = new CMainMenu( m_pGUI );

	// Create the download progress gui
	m_pDownloadProgress = new CDownloadProgress;

	// Create the settings gui
	m_pSettings = new CMenuSettings( m_pGUI );

	// Create the console gui
	// todo
}

void CGUI::SetCursorVisible( bool bVisible )
{
	// Toggle the cursor
	m_pGUI->SetCursorEnabled( bVisible );

	// Is the camera instance valid?
	if( pCore->GetCamera() )
	{
		// Are we enabling the mouse?
		if( bVisible )
		{
			// Store the previous camera state
			m_bPreviousCameraState = pCore->GetCamera()->IsLocked();

			// Lock the camera control
			pCore->GetCamera()->LockControl( true );

			// Hide the map if it's visible
			if ( pCore->GetGame()->IsMapOpen () )
				pCore->GetGame()->OpenMap ( false );
		}
		else
		{
			// Restore the old camera state before we locked it
			pCore->GetCamera()->LockControl( m_bPreviousCameraState );

			// Reset the old camera state
			m_bPreviousCameraState = false;
		}
	}
}

bool CGUI::IsCursorVisible( void )
{
	return m_pGUI->IsCursorEnabled();
}

void CGUI::Render( void )
{
	// Render the gui
	m_pGUI->Draw();

	// Process the mouse
	ProcessMouse();

	// Render the mouse cursor
	m_pGUI->DrawMouseCursor();

	// Pulse the serverbrowser
	if( m_pServerBrowser )
		m_pServerBrowser->Pulse();
}

void CGUI::ProcessMouse( void )
{
	// Process the mouse
	if( pCore->GetGame()->Focused() && m_pGUI->IsCursorEnabled() )
	{
		// Make sure the input device is valid
		if( !m_pInputDevice )
			return;

		// If we've lose the input device, re-aquire it
		if( m_pInputDevice->GetDeviceState( sizeof(m_MouseState), (LPVOID)&m_MouseState ) == DIERR_INPUTLOST )
		{
			// Acquire the input device
			m_pInputDevice->Acquire();
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
	return (pCore->GetGame()->Focused() || !pCore->GetChat()->IsInputVisible());
}

unsigned long CGUI::TranslateFromScanCode( DWORD dwCharacter )
{
	switch( dwCharacter )
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
				if( m_pGUI->ProcessKeyboardInput( dwKey, true ) )
					return true;

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
			// Inject the character input into cegui
			if ( CEGUI::System::getSingleton().injectChar ( (CEGUI::utf32)wParam ) )
				return true;

			break;
		}
	}

	// We didn't handle this
	return false;
}