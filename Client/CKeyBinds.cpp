/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CKeyBinds.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "StdInc.h"

extern CCore			* pCore;

CKeyBind::CKeyBind( const char * szKey, const char * szState, SQVM * pVM, SQObjectPtr pFunction )
{
	// Initialise
	m_strKey.Set( szKey );
	m_strState.Set( szState );
	m_pVM = pVM;
	m_pFunction = pFunction;
	m_bActive = false;
}

void CKeyBind::Trigger( void )
{
	// Get the script from the VM
	CSquirrel * pScript = pCore->GetClientScriptingManager()->GetScriptingManager()->Get( m_pVM );

	// Did we fail to find the script?
	if( !pScript )
		return;

	// Call the function
	pScript->Call( m_pFunction );
}

CKeyBinds::~CKeyBinds( void )
{
	// Unbind all keybinds
	UnbindAll();

	// Clear all pressed keys
	m_pressedKeys.clear();
}

bool CKeyBinds::BindKey( const char * szKey, const char * szState, SQVM * pVM, SQObjectPtr pFunction )
{
	// Is this key already bound?
	if( IsKeyBound( szKey, szState ) )
		return false;

	// Create a new keybind instance
	CKeyBind * pKeyBind = new CKeyBind( szKey, szState, pVM, pFunction );

	// Did the keybind fail to create?
	if( !pKeyBind )
		return false;

	// Set the keybind active
	pKeyBind->SetActive( true );

	// Add the key bind to the list
	m_keyBinds.push_back( pKeyBind );

	return true;
}

bool CKeyBinds::UnbindKey( const char * szKey, const char * szState )
{
	// Get the keybind
	CKeyBind * pKeyBind = GetKeyBind( szKey, szState );

	// Did we fail to find the keybind?
	if( !pKeyBind )
		return false;

	// Remove the keybind from the list
	m_keyBinds.remove( pKeyBind );

	// Delete the keybind
	SAFE_DELETE( pKeyBind );

	return true;
}

void CKeyBinds::UnbindAll( void )
{
	// Loop over all keybinds
	for( std::list< CKeyBind* >::iterator iter = m_keyBinds.begin(); iter != m_keyBinds.end(); iter++ )
	{
		// Unbind this key
		UnbindKey( (*iter)->GetKey(), (*iter)->GetState() );
	}

	// Clear the keybinds list
	m_keyBinds.clear();
}

CKeyBind * CKeyBinds::GetKeyBind( const char * szKey, const char * szState )
{
	// Loop over all key binds
	for( std::list< CKeyBind* >::iterator iter = m_keyBinds.begin(); iter != m_keyBinds.end(); iter++ )
	{
		// Is the current bind what we're looking for?
		if( !(*iter)->GetKey().Compare( szKey ) && !(*iter)->GetState().Compare( szState ) )
			return *iter;
	}

	return NULL;
}

bool CKeyBinds::IsKeyBound( const char * szKey, const char * szState )
{
	return (GetKeyBind( szKey, szState ) != NULL);
}

String GetKeyNameByCode( DWORD dwCode )
{
	String strCode;

	if( dwCode >= 0x30 && dwCode <= 0x39 )
	{
		strCode = (unsigned char)dwCode;
	}
	else if( dwCode >= 0x41 && dwCode <= 0x5A )
	{
		strCode = (unsigned char)( dwCode + 0x20 );
	}
	else if( dwCode >= VK_NUMPAD0 && dwCode <= VK_NUMPAD9 )
	{
		strCode.Format( "num_%d", dwCode - VK_NUMPAD0 );
	}
	else if( dwCode >= VK_F1 && dwCode <= VK_F12 )
	{
		strCode.Format( "f%d", dwCode - VK_F1 + 1 );
	}
	else
	{
		switch( dwCode )
		{
		case VK_TAB:
			strCode = "tab"; break;
		case VK_RETURN:
			strCode = "enter"; break;
		case VK_CONTROL:
			strCode = "ctrl"; break;
		case VK_SHIFT:
			strCode = "shift"; break;
		case VK_MENU:
			strCode = "alt"; break;
		case VK_ADD:
			strCode = "num_add"; break;
		case VK_SUBTRACT:
			strCode = "num_sub"; break;
		case VK_DIVIDE:
			strCode = "num_div"; break;
		case VK_MULTIPLY:
			strCode = "num_mul"; break;
		case VK_SPACE:
			strCode = "space"; break;
		case VK_LEFT:
			strCode = "arrow_left"; break;
		case VK_RIGHT:
			strCode = "arrow_right"; break;
		case VK_UP:
			strCode = "arrow_up"; break;
		case VK_DOWN:
			strCode = "arrow_down"; break;
		case VK_PRIOR:
			strCode = "page_up"; break;
		case VK_NEXT:
			strCode = "page_down"; break;
		case VK_END:
			strCode = "end"; break;
		case VK_HOME:
			strCode = "home"; break;
		case VK_INSERT:
			strCode = "insert"; break;
		case VK_DELETE:
			strCode = "delete"; break;
		case 0x1E:
			strCode = "backspace"; break;
		case 0x01:
			strCode = "esc"; break;
		case VK_CAPITAL:
			strCode = "caps"; break;
		case VK_BACK:
			strCode = "back"; break;
		case 0xC4:
			strCode = "ä"; break;
		case 0xE4:
			strCode = "ä"; break;
		case 0xD4:
			strCode = "ö"; break;
		case 0xF4:
			strCode = "ö"; break;
		case 0xDC:
			strCode = "ü"; break;
		case 0xFC:
			strCode = "ü"; break;
		default:
			break;
		}
	}

	return strCode;
}

void CKeyBinds::ProcessInput( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// Is the chat window input active?
	if( pCore->GetChat() && pCore->GetChat()->IsInputVisible() )
		return;

	// Do we not have any keybinds?
	if( !m_keyBinds.size() )
		return;

	//
	String strKey = GetKeyNameByCode( wParam );
	String strState( "unknown" );

	// Don't we have a key?
	if( strKey.IsEmpty() )
		return;

	// Was this a key up/down event?
	if( uMsg == WM_KEYDOWN )
	{
		// Set the state string
		strState = "down";

		// Has the key not already been pressed?
		if( std::find( m_pressedKeys.begin(), m_pressedKeys.end(), strKey ) == m_pressedKeys.end() )
		{
			// Remove this key
			m_pressedKeys.push_back( strKey );
		}
		else
			return;
	}
	else if( uMsg == WM_KEYUP )
	{
		// Set the state string
		strState = "up";

		// Has the key already been pressed?
		if( std::find( m_pressedKeys.begin(), m_pressedKeys.end(), strKey ) != m_pressedKeys.end() )
		{
			// Remove this key
			m_pressedKeys.remove( strKey );
		}
	}

	// Get the keybind for this combination
	CKeyBind * pKeyBind = GetKeyBind( strKey.Get(), strState.Get() );

	// Is the keybind invalid?
	if( !pKeyBind )
		return;

	// Call the keybind handler
	pKeyBind->Trigger();
}