/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CChat.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"ExplicitSingleton.h"

#include	"CGUI.h"
#include	"gui_impl\CGUI_Impl.h"
#include	"gui_impl\CGUIElement_Impl.h"

#include	"CColor.h"

#define		MAX_MESSAGE_LEN					128
#define		MAX_CHAT_LINES					12
#define		MAX_CHAT_LINES_HISTORY			30
#define		MAX_CHAT_HISTORY				12
#define		CHAT_CMD_CHAR					'/'

struct ChatLine
{
	char szMessage[MAX_MESSAGE_LEN + 1], szName[MAX_PLAYER_NAME + 3];
	unsigned long ulMsgColour, ulNameColour;
	float fNameExtent, fTimeSendExtent;
	String szTimeSend;
};

class CNetworkPlayer;
class String;

class CChat : public ExplicitSingleton<CChat>
{

private:

	// Chat
	ChatLine				m_chatLine[MAX_CHAT_LINES_HISTORY];
	bool					m_bVisible;
	String					m_strFont;
	float					m_fX;
	float					m_fY;
	bool					m_bTimeStamp;

	// Input
	String					m_strInput;
	bool					m_bInputVisible;
	bool					m_bOldLockState;
	int						m_iTextCursorPosition;
	int						m_iCountSelectedChars;

	// Scroll chat lines
	int						m_iChatScrollOffset;
	bool					m_bLastFixUnnec;

	// History
	int						m_iTotalHistory;
	int						m_iCurrentHistory;
	char					m_szCurrentHistory[MAX_MESSAGE_LEN];
	char					m_szHistory[MAX_CHAT_HISTORY][MAX_MESSAGE_LEN + 1];

	// Chat functions
	void					PushUp				( void );

	// Chat history scroll functions
	void					ScrollUpChatHistory	( void );
	void					ScrollDwnChatHistory( void );

	// History functions
	void					AddToHistory		( void );
	void					PushUpHistory		( void );
	void					HistoryUp			( void );
	void					HistoryDown			( void );

public:

	CChat(CGUI_Impl * pGUI, float fX, float fY);
	~CChat(void);

	void					Render				( void );

	void					SetVisible			( bool bVisible) { m_bVisible = bVisible; }
	bool					IsVisible			( void ) { return m_bVisible; }

	void					SetInputVisible		( bool bVisible ) { m_bInputVisible = bVisible; }
	bool					IsInputVisible		( void ) { return m_bInputVisible; }

	void					SetInputText		( const char * szText );
	const char				* GetInputText		( void ) { return m_strInput.Get(); }

	void					Clear				( void );
	void					ClearHistory		( void );

	void					ClearInputText		( void );

	bool					ClearSelectText		( void );

	void					SetFont				( const char * szFont ) { m_strFont.Set(szFont); }
	const char				* GetFont			( void ) { return m_strFont.Get(); }

	float					GetFontHeight		( void );
	float					GetCharacterWidth	( int iChar );

	void					AddChatMessage		( CNetworkPlayer * pNetworkPlayer, const char * szMessage );
	void					AddInfoMessage		( const char * szInfo, ... );
	void					AddInfoMessage		( CColor colour, const char * szInfo, ... );
	void					AddDebugMessage		( const char * szDebug, ... );

	bool					HandleKeyInput		( CGUIKeyEventArgs keyArgs );
	bool					HandleKeyDown		( CGUIKeyEventArgs keyArgs );

	bool					HandleMouseWheel	( CGUIMouseEventArgs mouseArgs );

	void					ProcessInput		( void );

	void					LockGameControls	( bool bLock );

	void					SetTimeStampVisible	( bool bTimeStamp ) { m_bTimeStamp = bTimeStamp; }
	bool					IsTimeStampVisible	( void ) { return m_bTimeStamp; }

};