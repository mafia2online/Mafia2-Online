/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CChat.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"BaseInc.h"
#include	<math.h>

#include "CCore.h"

#include "CChat.h"

#include "CGUICallback.h"
#include "CGUIEvent.h"
#include "CGUI.h"
#include "gui_impl\CGUI_Impl.h"

#include "CGraphics.h"
#include "CMainMenu.h"

#include "CString.h"
#include "CColor.h"
#include "Math/CVector3.h"

#include "CLocalPlayer.h"
#include "CNetworkPlayer.h"
#include "CPlayerManager.h"
#include "CNetworkModule.h"

#include "BitStream.h"
#include "../Libraries/RakNet/Source/PacketPriority.h"
#include "../Shared/CNetworkRPC.h"

#include "SharedUtility.h"

#include "CClientScriptingManager.h"
#include "CEvents.h"
#include "Scripting\CSquirrelArguments.h"

#include "CM2Camera.h"

#include "CSettings.h"

#include "CLua.h"

bool m_bChatOldCameraState;

CChat::CChat(CGUI_Impl * pGUI, float fX, float fY)
{
	// Reset Chat
	memset(&m_chatLine, 0, sizeof(m_chatLine));
	m_fX = fX;
	m_fY = fY;
	m_bVisible = false;

	// Reset the input
	memset(m_szCurrentHistory, 0, sizeof(m_szCurrentHistory));
	memset(m_szHistory, 0, sizeof(m_szHistory));
	m_iTotalHistory = 0;
	m_iTextCursorPosition = 0;
	m_iCountSelectedChars = 0;
	m_iCurrentHistory = -1;
	m_iChatScrollOffset = 0;
	m_bInputVisible = false;
	m_bOldLockState = false;
	m_bLastFixUnnec = false;

	// Get timestamp param
	m_bTimeStamp = CVAR_GET_BOOL("timestamp");

	// Set the key handlers
	pGUI->SetCharacterKeyHandler(GUI_CALLBACK_KEY(&CChat::HandleKeyInput, this));
	pGUI->SetKeyDownHandler(GUI_CALLBACK_KEY(&CChat::HandleKeyDown, this));

	// Set the mouse handlers
	pGUI->SetMouseWheelHandler( GUI_CALLBACK_MOUSE( &CChat::HandleMouseWheel, this) );
}

CChat::~CChat(void)
{
}

void CChat::SetInputVisible( bool bVisible )
{
	// Set input visible
	m_bInputVisible = bVisible;

	// Should we input visible?
	if ( bVisible )
	{
		// Save the old cursor state
		m_bOldCursorState = CCore::Instance()->GetGUI()->IsCursorVisible();

		// Enable cursor
		CCore::Instance()->GetGUI()->SetCursorVisible( true );
		//CCore::Instance()->GetChat()->AddDebugMessage("Old state cursor: %s", m_bOldCursorState ? "true" : "false");
	}
	else
	{
		// Restore cursor
		CCore::Instance()->GetGUI()->SetCursorVisible( m_bOldCursorState );
	}
}

void CChat::PushUp(void)
{
	// Loop over all chatlines
	for (int i = (MAX_CHAT_LINES_HISTORY - 1); i; i--)
	{
		// Copy the current chat line to the next one
		memcpy(&m_chatLine[i], &m_chatLine[i - 1], sizeof(ChatLine));
	}
}

void CChat::AddToHistory(void)
{
	// Move all history items up
	PushUpHistory();

	// Set the last history message
	strcpy_s(m_szHistory[0], m_strInput.Get());
	memset(m_szCurrentHistory, 0, sizeof(m_szCurrentHistory));

	// Reset the current history
	m_iCurrentHistory = -1;

	// Increase the total history items
	if (m_iTotalHistory < MAX_CHAT_HISTORY)
		m_iTotalHistory++;
}

void CChat::PushUpHistory(void)
{
	// Loop over all history items
	for (int i = (MAX_CHAT_HISTORY - 1); i; i--)
		strcpy_s(m_szHistory[i], m_szHistory[i - 1]);
}

void CChat::HistoryUp(void)
{
	// Is there any history to move?
	if (m_iCurrentHistory < MAX_CHAT_HISTORY && ((m_iTotalHistory - 1) > m_iCurrentHistory))
	{
		// Are we at the start?
		if (m_iCurrentHistory == -1)
			strcpy_s(m_szCurrentHistory, m_strInput.Get());

		// Increase the current history count
		m_iCurrentHistory++;

		// Decrease the current history count if there's nothing here
		if (strlen(m_szHistory[m_iCurrentHistory]) == 0)
			m_iCurrentHistory--;

		// Set the input text
		SetInputText(m_szHistory[m_iCurrentHistory]);
	}
}

void CChat::HistoryDown(void)
{
	// Can we move down?
	if (m_iCurrentHistory > -1)
	{
		// Decrease the current history
		m_iCurrentHistory--;

		// Are we at the end?
		if (m_iCurrentHistory == -1)
			SetInputText(m_szCurrentHistory);
		else
			SetInputText(m_szHistory[m_iCurrentHistory]);
	}
}

void CChat::ScrollUpChatHistory( void )
{
	if ( m_iChatScrollOffset < (MAX_CHAT_LINES_HISTORY - MAX_CHAT_LINES) )
	{
		// Get message len
		String szChattext = m_chatLine[MAX_CHAT_LINES + m_iChatScrollOffset].szMessage;
		size_t sLenMsg = szChattext.GetLength();

		// Do not scroll, if the messages have ended on top
		if ( (int)sLenMsg > 0 )
			m_iChatScrollOffset++;
	}
}

void CChat::ScrollDwnChatHistory( void )
{
	if ( m_iChatScrollOffset > 0 )
		m_iChatScrollOffset--;
}

void CChat::Render(void)
{
	// Is the chat not visible?
	if (!m_bVisible)
		return;

	// Get the default font
	CGraphics * pGraphics = CCore::Instance()->GetGraphics();

	// Calculate the render Y position
	float fCurrentY = m_fY;
	int iCurrentMessage = ((MAX_CHAT_LINES - 1) + m_iChatScrollOffset);

	// Loop over each chat line
	for (int i = 0; i < MAX_CHAT_LINES; i++)
	{
		float fTSOffset = 0.0;

		// Do we have a name?
		if (m_chatLine[iCurrentMessage - i].fNameExtent > 0.0f )
		{
			if ( IsTimeStampVisible() )
			{
				// Draw the timestamp
				pGraphics->DrawText(m_fX, fCurrentY, m_chatLine[iCurrentMessage - i].ulMsgColour, 1.0f, "tahoma-bold", true, m_chatLine[iCurrentMessage - i].szTimeSend);

				// Offset x
				fTSOffset = m_chatLine[iCurrentMessage - i].fTimeSendExtent;
			}

			// Draw the player name
			pGraphics->DrawText( m_fX + fTSOffset, fCurrentY, m_chatLine[iCurrentMessage - i].ulNameColour, 1.0f, "tahoma-bold", true, m_chatLine[iCurrentMessage - i].szName );

			// Draw the chat text
			pGraphics->DrawText( (m_fX + m_chatLine[iCurrentMessage - i].fNameExtent + fTSOffset), fCurrentY, m_chatLine[iCurrentMessage - i].ulMsgColour, 1.0f, "tahoma-bold", true, m_chatLine[iCurrentMessage - i].szMessage );
		}
		else
		{
			// Get the timestamp settings
			if ( IsTimeStampVisible() )
			{
				// Draw the text
				pGraphics->DrawText( m_fX, fCurrentY, m_chatLine[iCurrentMessage - i].ulMsgColour, 1.0f, "tahoma-bold", true, m_chatLine[iCurrentMessage - i].szTimeSend );

				// Offset x
				fTSOffset = m_chatLine[iCurrentMessage - i].fTimeSendExtent;
			}

			// Draw the chat text
			pGraphics->DrawText( (m_fX + fTSOffset), fCurrentY, m_chatLine[iCurrentMessage - i].ulMsgColour, 1.0f, "tahoma-bold", true, m_chatLine[iCurrentMessage - i].szMessage );
		}

		// Increase the current offset
		fCurrentY += 16.0f;
	}

	// Is the input visible?
	if (m_bInputVisible)
	{
		// Calculate the input render position
		float fInputY = (m_fY + (16.0f * MAX_CHAT_LINES) + 10.0f);

		// String for printing
		String strSay = m_strInput.Get();


		// Get intput text width
		float fStrInputWidth = pGraphics->GetTextWidth(m_strInput.Get(), 1.0, "tahoma-bold");
		
		// Initial size
		float fWidthInputSubstrate = 500.0f;

		// Width to size of the text
		if (fStrInputWidth > fWidthInputSubstrate - 22.0)
			fWidthInputSubstrate = (fWidthInputSubstrate + (fStrInputWidth - fWidthInputSubstrate) + 22.0);

		// Draw the input substrate
		pGraphics->DrawBox(m_fX - 5.0f, fInputY - 3.0f, fWidthInputSubstrate, 22.0, D3DCOLOR_ARGB(140, 0, 0, 0));

#ifdef _DEBUG
		pGraphics->DrawText(30.0f, 400.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, "tahoma-bold", true, "m_iCountSelectedChars: %d, m_iTextCursorPosition: %d", m_iCountSelectedChars, m_iTextCursorPosition );
#endif

		//Is text select?
		if (m_iCountSelectedChars != 0)
		{
			// Start select position
			int iStartPos = m_iTextCursorPosition + (m_iCountSelectedChars > 0 ? 0 : m_iCountSelectedChars);
			// End select position
			int iEndPos = m_iTextCursorPosition + (m_iCountSelectedChars > 0 ? m_iCountSelectedChars : 0);

			// Split string
			String strFirst = strSay.substr(0, iStartPos);
			String strMiddle = strSay.substr(iStartPos, abs(m_iCountSelectedChars));
			String strLast = strSay.substr(iEndPos, strSay.GetLength() - iEndPos);

			// Substrings width
			float fStrFirstWidth = pGraphics->GetTextWidth(strFirst.Get(), 1.0f, "tahoma-bold");
			float fStrMiddleWidth = pGraphics->GetTextWidth(strMiddle.Get(), 1.0f, "tahoma-bold");

			// Draw first substring
			pGraphics->DrawText(m_fX, fInputY, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, "tahoma-bold", true, strFirst.Get());

			// Draw select substring
			pGraphics->DrawBox(m_fX + fStrFirstWidth, fInputY, fStrMiddleWidth, 16.0f, D3DCOLOR_ARGB(64, 0, 0, 255));
			pGraphics->DrawText(m_fX + fStrFirstWidth, fInputY, D3DCOLOR_ARGB(255, 128, 128, 128), 1.0f, "tahoma-bold", true, strMiddle.Get());

			// Draw last substring
			float strLastStart_x = m_fX + fStrFirstWidth + fStrMiddleWidth;
			pGraphics->DrawText(strLastStart_x, fInputY, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, "tahoma-bold", true, strLast.Get());
		}
		else
		{
			// Draw the input message
			strSay.Insert(m_iTextCursorPosition, '|');
			pGraphics->DrawText(m_fX, fInputY, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, "tahoma-bold", true, strSay.Get());
		}




		// Calculate scroll box position
		int iMaxChatHistMinRender = (MAX_CHAT_LINES_HISTORY - MAX_CHAT_LINES);
		int	iMessagesIsset = 0;
		float fXScrlBx = m_fX - 20.0f;
		float fYScrlBx = m_fY + 3.0f;
		float fHScrlBx = fInputY - 39.0f;

		for( int i = MAX_CHAT_LINES; i < MAX_CHAT_LINES_HISTORY; i++ )
		{
			String szChattext = m_chatLine[i].szMessage;
			size_t sLenMsg = szChattext.GetLength();

			// We add if there is a message beyond the borders of the render
			if ( (int)sLenMsg > 0 )
				iMessagesIsset++;
		}
	
		// Max height
		float fScrMaxHeight = (fHScrlBx - 2.0f);
		// Take the 4 percent of the maximum length
		float fScrollingOneStrPr = (fScrMaxHeight * 4 / 100);
		// Reduce the length when more than 12 posts
		float fScrHeight = ( fScrMaxHeight - (iMessagesIsset * fScrollingOneStrPr) );
		// Drop down the scroll bar and lift as you scroll through
		float fScrlYPos = ((fYScrlBx + 1.0f) + (fScrMaxHeight - fScrHeight) - (m_iChatScrollOffset * fScrollingOneStrPr));		

		// Just in case
		if ( fScrlYPos > (fYScrlBx + 1.0f) && iMessagesIsset == m_iChatScrollOffset && iMessagesIsset != 0  )
			fScrlYPos = (fYScrlBx + 1.0f);

		// Draw scrollbox
		pGraphics->DrawBox(fXScrlBx, fYScrlBx, 10.0, fHScrlBx, D3DCOLOR_ARGB(140, 0, 0, 0));
		pGraphics->DrawBox(fXScrlBx + 1.0f, fScrlYPos, 8.0, fScrHeight, D3DCOLOR_ARGB(128, 200, 200, 200));


#ifdef _DEBUG
		pGraphics->DrawText(25.0f, 450.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, "tahoma-bold", true, "fScrollingProportion: %f, fScrHeight: %f, fScrlYPos: %f, iMessagesIsset: %d, m_iChatScrollOffset: %d\n kastil: %f", fScrollingOneStrPr, fScrHeight, fScrlYPos, iMessagesIsset, m_iChatScrollOffset, (fYScrlBx + 1.0f));
#endif

	}
}

void CChat::SetInputText(const char * szText)
{
	m_strInput.Set(szText);
	m_iTextCursorPosition = m_strInput.GetLength();
	m_iCountSelectedChars = 0;
}

void CChat::Clear(void)
{
	// Clear all current chat lines
	memset(&m_chatLine, 0, sizeof(m_chatLine));
}

void CChat::ClearHistory(void)
{
	// Clear the last stored history
	memset(m_szCurrentHistory, 0, sizeof(m_szCurrentHistory));

	// Clear all stored history
	memset(m_szHistory, 0, sizeof(m_szHistory));

	// Reset
	m_iTotalHistory = 0;
	m_iCurrentHistory = -1;
}

void CChat::ClearInputText(void)
{
	m_strInput.clear();
	m_iTextCursorPosition = 0;
	m_iCountSelectedChars = 0;
}

bool CChat::ClearSelectText(void)
{
	// Is text select?
	if (m_iCountSelectedChars != 0)
	{
		int start = m_iTextCursorPosition + (m_iCountSelectedChars > 0 ? 0 : m_iCountSelectedChars);

		m_strInput.Erase(start, abs(m_iCountSelectedChars));
		m_iTextCursorPosition = start;
		m_iCountSelectedChars = 0;
		return true;
	}
	return false;
}

float CChat::GetFontHeight(void)
{
	return CCore::Instance()->GetGraphics()->GetFontHeight(1.0f, GetFont());
}

float CChat::GetCharacterWidth(int iChar)
{
	return CCore::Instance()->GetGraphics()->GetCharacterWidth(iChar, 1.0f, GetFont());
}

void CChat::AddChatMessage(CNetworkPlayer * pNetworkPlayer, const char * szMessage)
{
	// Is the player invalid?
	if (!pNetworkPlayer)
		return;

	// Check for the % character
	char * szChar = (char *)memchr(szMessage, '%', strlen(szMessage));
	char szMessageToSet[129];

	// Did we find the character?
	if (szChar)
	{
		//
		char szMessageA[129];
		int offset1 = 0;
		int offset2 = 0;
		strcpy(szMessageA, szMessage);

		while (true)
		{
			if (szMessageA[offset1] == '%')
			{
				szMessageToSet[offset2] = '%';
				szMessageToSet[offset2 + 1] = '%';
				offset2 += 2;
				offset1++;
				continue;
			}
			else if (szMessageA[offset1] == '\0' || offset1 == 128 || offset2 == 128)
			{
				szMessageToSet[offset2] = '\0';
				break;
			}
			else
			{
				szMessageToSet[offset2] = szMessageA[offset1];
				offset1++;
				offset2++;
			}
		}
	}
	else
		strcpy(szMessageToSet, szMessage);

	// Push all the messages up
	PushUp();

	// Get the player colour
	CColor colour(pNetworkPlayer->GetColour(), true);

	// Set the chat line data
	strcpy_s(m_chatLine[0].szMessage, (MAX_MESSAGE_LEN + 1), szMessageToSet);
	sprintf(m_chatLine[0].szName, "%s: ", pNetworkPlayer->GetNick().Get());
	m_chatLine[0].ulMsgColour = D3DCOLOR_ARGB(255, 255, 255, 255);
	m_chatLine[0].ulNameColour = colour.dwHexColor;
	m_chatLine[0].fNameExtent = CCore::Instance()->GetGraphics()->GetTextWidth(m_chatLine[0].szName, 1.0f, "tahoma-bold");
}

void CChat::AddInfoMessage(const char * szInfo, ...)
{
	// Get the arguments
	char szBuffer[MAX_MESSAGE_LEN];
	va_list args;
	va_start(args, szInfo);
	vsnprintf(szBuffer, MAX_MESSAGE_LEN, szInfo, args);
	va_end(args);

	// Get the arguments
	AddInfoMessage(CColor(24, 116, 205, 255), szBuffer);
}

void CChat::AddInfoMessage(CColor colour, const char * szInfo, ...)
{
	// Push all the messages up
	PushUp();

	// Get the arguments
	va_list args;
	va_start(args, szInfo);
	vsnprintf(m_chatLine[0].szMessage, 128, szInfo, args);
	va_end(args);

	// Set the chat line data
	m_chatLine[0].szMessage[MAX_MESSAGE_LEN] = '\0';
	m_chatLine[0].ulMsgColour = colour.dwHexColor;
	m_chatLine[0].fNameExtent = 0.0f;
	m_chatLine[0].szTimeSend.Format("[%s] ", SharedUtility::GetTimeString());
	m_chatLine[0].fTimeSendExtent = CCore::Instance()->GetGraphics()->GetTextWidth(m_chatLine[0].szTimeSend, 1.0f, "tahoma-bold");
}

void CChat::AddDebugMessage(const char * szDebug, ...)
{
	// Push all the messages up
	PushUp();

	// Get the arguments
	va_list args;
	va_start(args, szDebug);
	vsnprintf(m_chatLine[0].szMessage, 128, szDebug, args);
	va_end(args);

	// Set the chat line data
	m_chatLine[0].szMessage[MAX_MESSAGE_LEN] = '\0';
	m_chatLine[0].ulMsgColour = D3DCOLOR_ARGB(255, 179, 179, 179);
	m_chatLine[0].fNameExtent = 0.0f;
	m_chatLine[0].szTimeSend.Format("[%s] ", SharedUtility::GetTimeString());
	m_chatLine[0].fTimeSendExtent = CCore::Instance()->GetGraphics()->GetTextWidth(m_chatLine[0].szTimeSend, 1.0f, "tahoma-bold");
}

bool CChat::HandleMouseWheel( CGUIMouseEventArgs mouseArgs )
{
	// Are we not connected?
	if (!CCore::Instance()->GetNetworkModule() || !CCore::Instance()->GetNetworkModule()->IsConnected())
		return false;

	// Open main menu?
	if ( CCore::Instance()->GetGUI()->GetMainMenu()->IsVisible() )
		return false;

	// Is the input focused on the GUI?
	if ( CCore::Instance()->GetGUI()->GetCEGUI()->IsInputEnabled() )
		return false;

	// Is the input not visible?
	if ( !IsInputVisible() )
		return false;

	// If you then show the cursor - unnecessary scrolling
	if ( m_bLastFixUnnec )
	{
		if ( mouseArgs.wheelChange > 0 )
			ScrollUpChatHistory();
		else
			ScrollDwnChatHistory();

#ifdef _DEBUG
		CCore::Instance()->GetGraphics()->DrawText(30, 300, D3DCOLOR_ARGB(200, 200, 200, 0), 1.0f, "tahoma-bold", true, "MOUSE WHEEL BRO: %f, m_iChatScrollOffset: %d", mouseArgs.wheelChange, m_iChatScrollOffset);
#endif
	}

	// If you then show the cursor - unnecessary scrolling
	m_bLastFixUnnec = true;
	
	return true;
}

bool CChat::HandleKeyInput(CGUIKeyEventArgs keyArgs)
{
	// Are we not connected?
	if (!CCore::Instance()->GetNetworkModule() || !CCore::Instance()->GetNetworkModule()->IsConnected())
		return false;

	// Is the input focused on the GUI?
	if (CCore::Instance()->GetGUI()->GetCEGUI()->IsInputEnabled())
		return false;

	// Are we enabling the chat?
	if (((keyArgs.codepoint == 116 || keyArgs.codepoint == 229 || keyArgs.codepoint == 84) || keyArgs.codepoint == 13) && !IsInputVisible()) // T and Enter
	{
		// Set the input visible
		SetInputVisible(true);

		// Lock game controls
		LockGameControls(true);
		return true;
	}

	// Is the input not visible?
	if (!IsInputVisible())
		return false;

	//
	switch (keyArgs.codepoint)
	{
	case VK_RETURN:
	{
		// Reset scroll chat
		m_iChatScrollOffset = 0;

		// If you then show the cursor - unnecessary scrolling
		m_bLastFixUnnec = false;

		// Restore game controls
		LockGameControls(false);

		// Disable the input
		SetInputVisible(false);

		// Process the input
		ProcessInput();

		// Clear the input text
		ClearInputText();
		break;
	}

	case VK_ESCAPE:
	{
		// Reset scroll chat
		m_iChatScrollOffset = 0;	

		// If you then show the cursor - unnecessary scrolling
		m_bLastFixUnnec = false;

		// Restore game controls
		LockGameControls(false);

		// Disable the input
		SetInputVisible(false);

		break;
	}

	case VK_BACK:
	{
		// Get the input text length
		size_t sLen = m_strInput.GetLength();

		// Do we have select text?
		if (ClearSelectText())
			break;

		// Do we have any input?
		if (sLen > 0 && m_iTextCursorPosition > 0)
		{
			--m_iTextCursorPosition;
			m_strInput.Erase(m_iTextCursorPosition, 1);
		}
		break;
	}

	default:
	{
		// Get the current input len
		size_t sInputLen = m_strInput.GetLength();

		// Clear select text
		ClearSelectText();

		// Have we got any room left?
		if (sInputLen < MAX_MESSAGE_LEN)
		{
			// Do not respond to combination (Ctrl+A, Ctrl+C and etc)
			if ( keyArgs.codepoint != 1 && keyArgs.codepoint != 3 && keyArgs.codepoint != 10 && keyArgs.codepoint != 22 && keyArgs.codepoint != 24 && keyArgs.codepoint != 26 && keyArgs.codepoint != 37) // Ctrl+A, Ctrl+C, Ctrl+Enter, Ctrl+V, Ctrl+X, Ctrl+Z, 37=%
			{
				// Text cursor doesn't last 
				if ( m_iTextCursorPosition < sInputLen )			
					// Add the character to the position cursor
					m_strInput.Insert(m_iTextCursorPosition, static_cast< char >(keyArgs.codepoint));
				else
					// Add the character to the end of the input text
					m_strInput += static_cast< char >(keyArgs.codepoint);
		
				++m_iTextCursorPosition;
			}
		}
		break;
	}
	}

	return false;
}

bool CChat::HandleKeyDown(CGUIKeyEventArgs keyArgs)
{
	// Are we not connected?
	if (!CCore::Instance()->GetNetworkModule() || !CCore::Instance()->GetNetworkModule()->IsConnected())
		return false;

	// Switch the scancode
	switch (keyArgs.scancode)
	{
		case CGUIKeys::ArrowUp:
		{
			// Is the input visible?
			if (IsInputVisible())
			{
				// Move the history up
				HistoryUp();
			}
			break;
		}

		case CGUIKeys::ArrowDown:
		{
			// Is the input visible?
			if (IsInputVisible())
			{
				// Move the history down
				HistoryDown();
			}
			break;
		}

		case CGUIKeys::ArrowLeft:
		{
			// Is the input visible?
			if (IsInputVisible())
			{
				// todo: move cursor position left
				if (m_iTextCursorPosition > 0)
				{
					if (keyArgs.sysKeys & CEGUI::SystemKey::Shift)
						++m_iCountSelectedChars;
					else
						m_iCountSelectedChars = 0;

					--m_iTextCursorPosition;
				}
			}
			break;
		}

		case CGUIKeys::ArrowRight:
		{
			// Is the input visible?
			if (IsInputVisible())
			{
				// todo: move cursor position right
				if (m_iTextCursorPosition < m_strInput.GetLength())
				{
					if (keyArgs.sysKeys & CEGUI::SystemKey::Shift)
						--m_iCountSelectedChars;
					else
						m_iCountSelectedChars = 0;

					++m_iTextCursorPosition;
				}
			}
			break;
		}

		case CGUIKeys::End:
		{
			// Is the input visible?
			if (IsInputVisible())
			{
				if (keyArgs.sysKeys & CEGUI::SystemKey::Shift)
				{
					m_iCountSelectedChars = m_iTextCursorPosition - m_strInput.GetLength();
				}
				else
				{
					m_iCountSelectedChars = 0;
				}
				// Move cursor to end
				m_iTextCursorPosition = m_strInput.GetLength();
			}
			break;
		}

		case CGUIKeys::Home:
		{
			// Is the input visible?
			if (IsInputVisible())
			{
				if (keyArgs.sysKeys & CEGUI::SystemKey::Shift)
				{
					m_iCountSelectedChars = m_iTextCursorPosition;
				}
				else
				{
					m_iCountSelectedChars = 0;
				}
				// Move cursor to start
				m_iTextCursorPosition = 0;
			}
			break;
		}

		case CGUIKeys::C:
		case CGUIKeys::X:
		{
			// Is the control key down and the input is visible?
			if( (keyArgs.sysKeys & CEGUI::SystemKey::Control) && IsInputVisible () )
			{
				// Do we have select text?
				if( m_iCountSelectedChars != 0 )
				{
					int start = m_iTextCursorPosition + ( m_iCountSelectedChars > 0 ? 0 : m_iCountSelectedChars );
					SharedUtility::SetClipboardText( m_strInput.substr(start, abs(m_iCountSelectedChars)), abs(m_iCountSelectedChars) + 1);

					// Are we cutting data?
					if ( keyArgs.scancode == CGUIKeys::X )
					{
						// Get the input text length
						size_t sLen = m_strInput.GetLength();

						// Do we have select text?
						ClearSelectText();

						// Do we have any input?
						if (sLen > 0 && m_iTextCursorPosition > 0)
						{
							--m_iTextCursorPosition;
							m_strInput.Erase(m_iTextCursorPosition, 1);
						}
					}
				}
			}
			break;
		}

		case CGUIKeys::V:
		{
			// Is the control key down and the input is visible?
			if( (keyArgs.sysKeys & CEGUI::SystemKey::Control) && IsInputVisible () )
			{
				// Get data from the clipboard
				const char * szClipboard = SharedUtility::GetClipboardText ();

				// Do we have data in the clipboard?
				if ( strlen ( szClipboard ) )
				{
					// Clear the select text
					ClearSelectText();

					// Get the input text length
					size_t szClipLen = String(szClipboard).GetLength();

					// If message is large
					if (szClipLen + m_strInput.GetLength() < 128)
					{
						// Insert text
						m_strInput.Insert( m_iTextCursorPosition, szClipboard);

						// Place the cursor at the end of the inserted text
						m_iTextCursorPosition = m_iTextCursorPosition + strlen(szClipboard);
					}
					else
						AddInfoMessage(CColor(197, 205, 76, 255), "The total length of the string exceeds 128 characters.");
				}
			}
			break;
		}

		case CGUIKeys::A:
		{
			// Is the control key down and the input is visible?
			if ((keyArgs.sysKeys & CEGUI::SystemKey::Control) && IsInputVisible())
			{
				m_iTextCursorPosition = 0;
				m_iCountSelectedChars = m_strInput.GetLength();				
			}
			break;
		}
	}
	return true;
}

void CChat::ProcessInput(void)
{
	// Are we not connected?
	if (!CCore::Instance()->GetNetworkModule() || !CCore::Instance()->GetNetworkModule()->IsConnected())
		return;

	if (m_strInput.GetLength() > 0)
	{
		bool bIsCommand = (m_strInput.GetChar(0) == CHAT_CMD_CHAR);
		bool bHasUsedCmd = false;
		if (bIsCommand)
		{
			std::string sInput = m_strInput.Get();
			size_t sCommandEnd = sInput.find(" ");

			// If we don't have a valid end use the end of the string
			if (sCommandEnd == std::string::npos)
				sCommandEnd = sInput.length();

			// Get the command name
			std::string strCommand = sInput.substr(1, (sCommandEnd - 1));

			std::string strParams;

			if (sCommandEnd < sInput.length())
				strParams = sInput.substr((sCommandEnd + 1), sInput.length());

			if (strCommand == "q" || strCommand == "quit" || strCommand == "exit")
			{
				CCore::Instance()->Shutdown();
				return;
			}
			else if (strCommand == "disconnect")
			{
				if (CCore::Instance()->GetNetworkModule() && CCore::Instance()->GetNetworkModule()->IsConnected())
				{
					CCore::Instance()->GetNetworkModule()->Disconnect();
					CCore::Instance()->StopMultiplayer();
					CCore::Instance()->GetGUI()->GetMainMenu()->SetVisible(true);
				}

				bHasUsedCmd = true;
			}
			else if (strCommand == "savepos")
			{
				bHasUsedCmd = true;

				// Are we spawned?
				if (CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->IsSpawned())
				{
					//
					CVector3 vecPosition;
					CVector3 vecRotation;
					bool bOnFoot = true;

					// Is the player on-foot?
					if (CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetState() == ePlayerState::PLAYERSTATE_ONFOOT)
					{
						// Get the localplayer position
						CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetPosition(&vecPosition);

						// Get the localplayer rotation
						CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetRotation(&vecRotation);
					}
					else if (CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetState() == ePlayerState::PLAYERSTATE_DRIVER || CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetState() == ePlayerState::PLAYERSTATE_PASSENGER)
					{
						// Get the vehicle position
						CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetVehicle()->GetPosition(&vecPosition);

						// Get the vehicle rotation
						CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetVehicle()->GetRotation(&vecRotation);

						//
						bOnFoot = false;
					}

					// Open the saved positions file
					FILE * pFile = fopen(SharedUtility::GetAbsolutePath("data\\savedpositions.txt").Get(), "a");

					// Did the file open?
					if (pFile)
					{
						// Get the localplayer pointer
						CLocalPlayer * pLocalPlayer = CCore::Instance()->GetPlayerManager()->GetLocalPlayer();

						// Save the player position
						fprintf(pFile, "%d, %f, %f, %f, %f, %f, %f // %s\n", (bOnFoot ? pLocalPlayer->GetModel() : pLocalPlayer->GetVehicle()->GetModel()), vecPosition.fX, vecPosition.fY, vecPosition.fZ, vecRotation.fX, vecRotation.fY, vecRotation.fZ, strParams.c_str());

						// Close the saved positions file
						fclose(pFile);

						//
						AddInfoMessage((bOnFoot ? " -> Onfoot position saved!" : " -> Invehicle position saved!"));
					}
					else
					{
						//
						AddInfoMessage(CColor(255, 0, 0, 255), "Failed to open savedpositions.txt");
					}
				}
			}
			else if (strCommand == "timestamp")
			{
				// Get settings timestamp
				bool bStatusTimestamp = CVAR_GET_BOOL("timestamp");

				// Set != old condition timestamp
				CVAR_SET( "timestamp", !bStatusTimestamp );
				SetTimeStampVisible( !bStatusTimestamp );
			}
#ifdef DEBUG
			else if (strCommand == "lua")
			{
				bHasUsedCmd = true;

				if (CLua::Execute(strParams.c_str()))
					AddInfoMessage(CColor(50, 177, 94, 255), strParams.c_str());
				else
					AddInfoMessage(CColor(178, 40, 86, 255), strParams.c_str());
			}
#endif
		}

		// Have we used a command?
		if (bHasUsedCmd)
		{
			// Add this command to the history
			AddToHistory();

			// This is an internal command, don't pass it to the server!
			return;
		}

		// Is the network module instance valid?
		if (CCore::Instance()->GetNetworkModule())
		{
			// Are we connected?
			if (CCore::Instance()->GetNetworkModule()->IsConnected())
			{
				RakNet::BitStream bitStream;
				RakNet::RakString strInput;

				// Is this a command?
				if (bIsCommand)
				{
					// Write 1
					bitStream.Write1();

					// Set the input minus the command character
					strInput = (GetInputText() + 1);
				}
				else
				{
					// Write 0
					bitStream.Write0();

					// Set the input
					strInput = GetInputText();
				}

				// Write the input
				bitStream.Write(strInput);

				// Call the client event
				CSquirrelArguments pArguments;
				pArguments.push(strInput.C_String());
				pArguments.push(bIsCommand);


				// It is here that you can turn off the display of messages to the client to format your posts. Therefore, it is here and not out there \/

				// Send it to the server
				CCore::Instance()->GetNetworkModule()->Call(RPC_PLAYER_CHAT, &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, true);

				// Add this message to the history
				AddToHistory();

				// Should we send this message?
				if (CCore::Instance()->GetClientScriptingManager()->GetEvents()->Call("onClientChat", &pArguments).GetInteger() == 1)
				{
					// Add the chat message for the localplayer if it's not a command
					if (!bIsCommand)
						AddChatMessage(CCore::Instance()->GetPlayerManager()->GetLocalPlayer(), GetInputText());
				}
			}
		}
	}
}

void CChat::LockGameControls(bool bLock)
{
	// Should we lock the controls?
	if (bLock)
	{
		// Get the old control state
		m_bOldLockState = CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->AreControlsLocked();

		// Get the old camera state
		m_bChatOldCameraState = CCore::Instance()->GetCamera()->IsLocked();

		// Lock the player controls
		CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->LockControls(true);

		// Lock the camera control
		CCore::Instance()->GetCamera()->LockControl(true);

		// Are we in a vehicle?
		if (CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->IsInVehicle())
		{
			// Reset some vehicle stuff
			CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetVehicle()->GetVehicle()->SetPower(false);
			CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetVehicle()->GetVehicle()->SetBrake(false);
		}
	}
	else
	{
		// Restore the player controls
		CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->LockControls(m_bOldLockState);

		// Restore the camera control
		CCore::Instance()->GetCamera()->LockControl(m_bChatOldCameraState);
	}
}