// Copyright (C) 2016 by Mafia 2 Online Team

#pragma once

#include <memory>
#include <unordered_map>

#include "Types.h"
#include "ExplicitSingleton.h"

#include "CColor.h"
#include "CString.h"

#include "CEGUIVector.h"

class CGUI;
class CGUIFont_Impl;
class CNetworkPlayer;

const size_t MAX_CHAT_MESSAGES = 15;

typedef void (*CommandHandler_t)(const char *const commandName, const std::vector<String> &params);

/**
 * The chat box class.
 *
 * Class implements the chat box functionallity.
 */
class ChatBox : public ExplicitSingleton<ChatBox>
{
private:

	/** Variable indicating if the chat box is visible or not. */
	bool								m_isVisible;

	/** Is the chat input active? */
	bool								m_isInputActive;

	/** The font used to render the chat. */
	std::shared_ptr<CGUIFont_Impl>		m_font;

	/** The UTF-8 input text. */
	String								m_inputText;

	/** The single chat line data. */
	struct ChatLine
	{
		/** The chat message content. */
		String		message;

		/** The chat message base color. */
		CColor		color;

		/** Does this line use color codding? */
		bool		colorCodding;
	};

	/** The chat messages */
	ChatLine							m_chatLines[MAX_CHAT_MESSAGES];

	/** The storage for client-local chat commands. */
	std::unordered_map<std::string, CommandHandler_t> m_commands;

	/** The timer used to display blinking type cursor. */
	float								m_cursorBlinkTimer;

	/** The last miliseconds time when the chat was drawn. */
	uint64								m_lastDrawTime;

	/** The input cursor position. */
	size_t								m_inputCursorPosition;

	/** The visual offset of the cursor. */
	float								m_inputCursorOffset;

	/** The variables storing the previous game input state. */
	//@{
	bool								m_wereControlsLocked;  //< Were the controls already locked when client locked it?
	bool								m_previousCameraState; //< The previous camera state.
	//@}

	/** Internal chat game input disable state used to check if any lock must be made. */
	bool								m_gameInputDisabled;

	/** Chat input history. */
	std::vector<String>					m_chatInputHistory;

	/** Current chat input history entry. If Nil the user is typing message. */
	size_t								m_currentInputHistoryEntry;

	/** The current input locale name. */
	char								m_currentLanguage[LOCALE_NAME_MAX_LENGTH];

	void			DrawInputLine		(void);

	void			SendChatMessage		(void);

	bool			ProcessLocalCommands(const char *const command);

	void			HandleCharacterInput(const unsigned character);
	bool			HandleKeyInput		(const unsigned virtualKeyCode, const bool down);

	void			MoveInputCursorRight(void);
	void			MoveInputCursorLeft (void);
	void			MoveInputCursorBegin(void);
	void			MoveInputCursorEnd (void);

	void			UpdateInputCursorOffset(void);

	void			ActivateInput		(void);
	void			DeactivateInput		(void);

	void			DisableGameInput	(const bool disable);

	void			InputHistoryNext	(void);
	void			InputHistoryPrevious(void);

	void			SetInputAsHistoryEntry(const size_t index);

	void			AddLine				(const CColor &color, const String &message, const bool colorCodding);
	void			OutputInternal		(const CColor &color, const String &message, const bool colorCodding);

	void			UpdateCurrentLanguage(void);

public:
					ChatBox				(CGUI *const gui);
					~ChatBox			();

	void			Draw				(void);

	bool			HandleMessage		(const UINT uMsg, const DWORD wParam, const DWORD lParam);

	void			Output				(const CColor &color, const String &message);
	void			OutputF				(const CColor &color, const char *const message, ...);

	void			OutputColored		(const CColor &color, const String &message);
	void			OutputColoredF		(const CColor &color, const char *const message, ...);

	void			OutputPlayerMessage	(CNetworkPlayer *const sender, const String &message);

	void			SetVisible			(const bool visible);
	bool			IsVisible			(void) const;

	bool			IsInputActive		(void) const;

	void			RegisterCommand		(const char *const command, CommandHandler_t handler);

	void			Clear				(void);

	static const CColor	INFO_MESSAGE_COLOR;
	static const CColor	ERROR_MESSAGE_COLOR;
};


/* eof */
