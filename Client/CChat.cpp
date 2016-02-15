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

bool m_bChatOldCameraState;

CChat::CChat(CGUI_Impl * pGUI, float fX, float fY)
{
	// Reset Chat
	memset(&m_chatLine, 0, sizeof(m_chatLine));
	m_fX = fX;
	m_fY = fY;
	m_bVisible = false;

	// Set the default font
	//SetFont( "tahoma-bold" );

	// Reset the input
	memset(m_szCurrentHistory, 0, sizeof(m_szCurrentHistory));
	memset(m_szHistory, 0, sizeof(m_szHistory));
	m_iTotalHistory = 0;
	m_iTextCursorPosition = 0;
	m_iCountSelectedChars = 0;
	m_iCurrentHistory = -1;
	m_bInputVisible = false;
	m_bOldLockState = false;

	// Set the key handlers
	pGUI->SetCharacterKeyHandler(GUI_CALLBACK_KEY(&CChat::HandleKeyInput, this));
	pGUI->SetKeyDownHandler(GUI_CALLBACK_KEY(&CChat::HandleKeyDown, this));
}

CChat::~CChat(void)
{
}

void CChat::PushUp(void)
{
	// Loop over all chatlines
	for (int i = (MAX_CHAT_LINES - 1); i; i--)
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

void CChat::Render(void)
{
	// Is the chat not visible?
	if (!m_bVisible)
		return;

	// Get the default font
	CGraphics * pGraphics = CCore::Instance()->GetGraphics();

	// Calculate the render Y position
	float fCurrentY = m_fY;
	int iCurrentMessage = (MAX_CHAT_LINES - 1);

	// Loop over each chat line
	for (int i = 0; i < MAX_CHAT_LINES; i++)
	{
		// Do we have a name?
		if (m_chatLine[iCurrentMessage - i].fNameExtent > 0.0f)
		{
			// Draw the player name
			pGraphics->DrawText(m_fX, fCurrentY, m_chatLine[iCurrentMessage - i].ulNameColour, 1.0f, "tahoma-bold", true, m_chatLine[iCurrentMessage - i].szName);

			// Draw the chat text
			pGraphics->DrawText((m_fX + m_chatLine[iCurrentMessage - i].fNameExtent), fCurrentY, m_chatLine[iCurrentMessage - i].ulMsgColour, 1.0f, "tahoma-bold", true, m_chatLine[iCurrentMessage - i].szMessage);
		}
		else
		{
			// Draw the text
			pGraphics->DrawText(m_fX, fCurrentY, m_chatLine[iCurrentMessage - i].ulMsgColour, 1.0f, "tahoma-bold", true, m_chatLine[iCurrentMessage - i].szMessage);
		}

		// Increase the current offset
		fCurrentY += 16.0f;
	}

	// Is the input visible?
	if (m_bInputVisible)
	{
		// Calculate the input render position
		float fInputY = (m_fY + (16.0f * MAX_CHAT_LINES) + 10.0f);

		// Offset text cursor
		int iOffsetCursor = strlen("Say: ");

		// String for printing
		String strSay = String("%s%s", "Say: ", m_strInput.Get());

		//Is text select?
		if (m_iCountSelectedChars != 0)
		{
			// Start select position
			int iStartPos = iOffsetCursor + m_iTextCursorPosition + (m_iCountSelectedChars > 0 ? 0 : m_iCountSelectedChars);
			// End select position
			int iEndPos = iOffsetCursor + m_iTextCursorPosition + (m_iCountSelectedChars > 0 ? m_iCountSelectedChars : 0);

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
			strSay.Insert(m_iTextCursorPosition + iOffsetCursor, '|');
			pGraphics->DrawText(m_fX, fInputY, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, "tahoma-bold", true, strSay.Get());
		}
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
	if ((keyArgs.codepoint == 116 || keyArgs.codepoint == 96 || keyArgs.codepoint == 84 || keyArgs.codepoint == 229) && !IsInputVisible())
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
		// Disable the input
		SetInputVisible(false);

		// Restore game controls
		LockGameControls(false);

		// Process the input
		ProcessInput();

		// Clear the input text
		ClearInputText();
		break;
	}

	case VK_ESCAPE:
	{
		// Disable the input
		SetInputVisible(false);

		// Restore game controls
		LockGameControls(false);
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
			// Text cursor doesn't last 
			if (m_iTextCursorPosition < sInputLen)
			{
				//char cInputedChar = static_cast< char >(keyArgs.codepoint);
				m_strInput.Insert(m_iTextCursorPosition, static_cast< char >(keyArgs.codepoint));
				++m_iTextCursorPosition;
			}
			else
			{
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

		/*case CGUIKeys::C:
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
		// Clear the current input text
		ClearInputText ();
		m_iTextCursorPosition = start;
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
		//Clear the select text
		ClearSelectText();

		// Update the input
		m_strInput.Insert(m_iTextCursorPosition, strlen( szClipboard ));
		m_iTextCursorPosition = m_iTextCursorPosition + strlen (szClipboard) - 1;
		}
		}
		break;
		}

		case CGUIKeys::A:
		{
			// Is the control key down and the input is visible?
			if ((keyArgs.sysKeys & CEGUI::SystemKey::Control) && IsInputVisible())
			{
				m_iTextCursorPosition = m_strInput.GetLength();
				m_iCountSelectedChars = -1 * m_strInput.GetLength();
			}
			break;
		}*/
	}
	return true;
}

void CChat::ProcessInput(void)
{
	// Are we not connected?
	if (!CCore::Instance()->GetNetworkModule() || !CCore::Instance()->GetNetworkModule()->IsConnected())
		return;

	// Was anything entered?
	if (m_strInput.GetLength() > 0)
	{
		// Is the input a command?
		bool bIsCommand = (m_strInput.GetChar(0) == CHAT_CMD_CHAR);

		// Internal cmds
		bool bHasUsedCmd = false;

		// Process internal commands
		if (bIsCommand)
		{
			//
			std::string sInput = m_strInput.Get();

			// Get the end of the command
			size_t sCommandEnd = sInput.find(" ");

			// If we don't have a valid end use the end of the string
			if (sCommandEnd == std::string::npos)
				sCommandEnd = sInput.length();

			// Get the command name
			std::string strCommand = sInput.substr(1, (sCommandEnd - 1));

			// Get the command parameters
			std::string strParams;

			// Do we have any parameters?
			if (sCommandEnd < sInput.length())
				strParams = sInput.substr((sCommandEnd + 1), sInput.length());

			if (strCommand == "q" || strCommand == "quit" || strCommand == "exit")
			{
				// Shutdown
				CCore::Instance()->Shutdown();
				return;
			}
			else if (strCommand == "disconnect")
			{
				// Are we connected?
				if (CCore::Instance()->GetNetworkModule() && CCore::Instance()->GetNetworkModule()->IsConnected())
				{
					// Disconnect from the network
					CCore::Instance()->GetNetworkModule()->Disconnect();

					// Stop multiplayer
					CCore::Instance()->StopMultiplayer();

					// Go back to main menu
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

				// Should we send this message?
				if (CCore::Instance()->GetClientScriptingManager()->GetEvents()->Call("onClientChat", &pArguments).GetInteger() == 1)
				{
					// Send it to the server
					CCore::Instance()->GetNetworkModule()->Call(RPC_PLAYER_CHAT, &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, true);

					// Add this message to the history
					AddToHistory();

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

/*
// TEMP
char * szWeather[87] =
{
"",
"DTFreeRideDay",
"DTFreeRideDayRain",
"DTFreeRideDaySnow",
"DTFreeRideDayWinter",
"DTFreeRideNightSnow",
"DTFreerideNight",

"DT_RTRclear_day_afternoon",
"DT_RTRclear_day_early_morn1",
"DT_RTRclear_day_early_morn2",
"DT_RTRclear_day_evening",
"DT_RTRclear_day_late_afternoon",
"DT_RTRclear_day_late_even",
"DT_RTRclear_day_morning",
"DT_RTRclear_day_night",
"DT_RTRclear_day_noon",
"DT_RTRfoggy_day_afternoon",
"DT_RTRfoggy_day_early_morn1",
"DT_RTRfoggy_day_evening",
"DT_RTRfoggy_day_late_afternoon",
"DT_RTRfoggy_day_late_even",
"DT_RTRfoggy_day_morning",
"DT_RTRfoggy_day_night",
"DT_RTRfoggy_day_noon",
"DT_RTRrainy_day_afternoon",
"DT_RTRrainy_day_early_morn",
"DT_RTRrainy_day_evening",
"DT_RTRrainy_day_late_afternoon",
"DT_RTRrainy_day_late_even",
"DT_RTRrainy_day_morning",
"DT_RTRrainy_day_night",
"DT_RTRrainy_day_noon",

"DT01part01sicily_svit",
"DT01part02sicily",
"DT02part01Railwaystation",
"DT02part02JoesFlat",
"DT02part03Charlie",
"DT02part04Giuseppe",
"DT02part05Derek",
"DT02NewStart1",
"DT02NewStart2",
"DT03part01JoesFlat",
"DT03part02FreddysBar",
"DT03part03MariaAgnelo",
"DT03part04PriceOffice",
"DT04part01JoesFlat",
"DT04part02",
"DT05Distillery_inside",
"DT05part01JoesFlat",
"DT05part02FreddysBar",
"DT05part03HarrysGunshop",
"DT05part04Distillery",
"DT05part05ElGreco",
"DT05part06Francesca",
"DT06part01",
"DT06part02",
"DT06part03",
"DT07part01fromprison",
"DT07part02dereksubquest",
"DT07part03prepadrestaurcie",
"DT07part04night_bordel",
"DT08part01cigarettesriver",
"DT08part02cigarettesmill",
"DT08part03crazyhorse",
"DT08part04subquestwarning",
"DT09part1VitosFlat",
"DT09part2MalteseFalcone",
"DT09part3SlaughterHouseAfter",
"DT09part4MalteseFalcone2",
"DT13part01death",
"DT10part02Roof",
"DT10part02bSUNOFF",
"DT10part03Evening",
"DT10part03Subquest",
"DT11part01",
"DT11part02",
"DT11part03",
"DT11part04",
"DT11part05",
"DT12_part_all",
"DT13part02",
"DT14part1_6",
"DT14part7_10",
"DT14part11",
"DT15",
"DT15_interier",
"DT15end",
};
int iCurrentIndex = 0;*/