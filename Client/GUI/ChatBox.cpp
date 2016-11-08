// Copyright (C) 2016 by Mafia 2 Online Team

#include "BaseInc.h"

#include "gui_impl/CGUI_Impl.h"
#include "gui_impl/CGUIFont_Impl.h"
#include "CGUI.h"
#include "CMainMenu.h"

#include "CLua.h"

#include "CCore.h"

#include "CGraphics.h"

#include "ChatBox.h"

#include "CLogFile.h"

#include "Types.h"

#include "Network/NetRPCs.h"
#include "CNetworkModule.h"

#include "CLocalPlayer.h" // todo: Remove
#include "CPlayerManager.h" // todo: Remove

#include "CEvents.h"
#include "CCommands.h"
#include "CClientScriptingManager.h"

#include "CM2Camera.h"

#include "Strings/Unicode.h"

const CColor ChatBox::INFO_MESSAGE_COLOR(180, 180, 180);
const CColor ChatBox::ERROR_MESSAGE_COLOR(255, 0, 0);

const float CHATBOX_PADDING = 5.0f;

const float INPUTLINE_HEIGHT = 20.0f;
const float CHATLINE_SPACING = 15.0f;

const Vector2 CHATBOX_POS(10.0f, 30.0f);
const Vector2 CHATBOX_SIZE(400.0f, MAX_CHAT_MESSAGES * CHATLINE_SPACING + CHATBOX_PADDING * 2.0f);

/** The '/q' command handler. */
void QuitHandler(const char *const command, const std::vector<String> &params)
{
	CCore::Instance()->Shutdown();
}

/** The '/disconnect' command handler. */
void DisconnectHandler(const char *const command, const std::vector<String> &params)
{
	CCore *const core = CCore::Instance();
	CNetworkModule *const netModule = core->GetNetworkModule();
	if (netModule && netModule->IsConnected())
	{
		netModule->Disconnect();
		core->StopMultiplayer();
		core->GetGUI()->GetMainMenu()->SetVisible(true);
	}
}

/** The '/lua' command handler. */
void LuaHandler(const char *const command, const std::vector<String> &params)
{
	String script;
	for (size_t i = 0; i < params.size(); ++i) {
		String param;
		param.Format("%s ", params[i]);
		script.Append(param);
	}

	const bool success = CLua::Execute(script);
	ChatBox::Instance()->Output(success ? ChatBox::INFO_MESSAGE_COLOR : ChatBox::ERROR_MESSAGE_COLOR, script);
}

/**
 * Constructor.
 *
 * @param gui The gui object which owns the chat box.
 */
ChatBox::ChatBox(CGUI *const gui)
	: m_isVisible(false)
	, m_isInputActive(false)

	, m_font()

	, m_inputText()

	, m_cursorBlinkTimer(0.0f)

	, m_lastDrawTime (0)

	, m_inputCursorPosition(0)
	, m_inputCursorOffset(0.0f)

	, m_wereControlsLocked(false)
	, m_previousCameraState(true)

	, m_gameInputDisabled(false)

	, m_chatInputHistory()
	, m_currentInputHistoryEntry(Nil())

	, m_currentLanguage()
{
	UpdateCurrentLanguage();

	CGUI_Impl *const guiImpl = gui->GetCEGUI();
	String fontPath;
	fontPath.Format("%s%s", CCore::Instance()->GetModDirectory().Get(), "data/gui/fonts/tahoma-bold.ttf");
	m_font = guiImpl->CreateFnt("Tahoma-Bold", fontPath, 8, 0, false);

	RegisterCommand("q", QuitHandler);
	RegisterCommand("quit", QuitHandler);

	RegisterCommand("disconnect", DisconnectHandler);

#ifdef _DEBUG
	RegisterCommand("lua", LuaHandler);
#endif
}

/** Desturctor. */
ChatBox::~ChatBox()
{
}


/**
 * Draw the chatbox.
 */
void ChatBox::Draw(void)
{
	if (!m_isVisible)
		return;

	CGraphics *const graphics = CCore::Instance()->GetGraphics();

	Vector2 position(CHATBOX_POS);

	graphics->DrawBox(position.fX, position.fY, CHATBOX_SIZE.fX, CHATBOX_SIZE.fY, 0xA0000000);

	position.fX += CHATBOX_PADDING;
	position.fY += CHATBOX_SIZE.fY - CHATLINE_SPACING - CHATBOX_PADDING;
	for (size_t i = 0; i < MAX_CHAT_MESSAGES; ++i) {
		const ChatLine &line = m_chatLines[i];
		const String &msg = line.message;
		if (! msg.IsEmpty()) {
			if (line.colorCodding) {
				m_font->DrawColoredString(msg, position, CEGUI::TextFormatting::LeftAligned, line.color.ToUint(), 1.0f);
			}
			else {
				m_font->DrawString(msg, position, CEGUI::TextFormatting::LeftAligned, line.color.ToUint(), 1.0f);
			}
		}

		position.fY -= CHATLINE_SPACING;
	}

	if (m_isInputActive) {
		DrawInputLine();
	}
}

/**
 * Draw the chat box input line.
 */
void ChatBox::DrawInputLine(void)
{
	CGraphics *const graphics = CCore::Instance()->GetGraphics();

	Vector2 position(CHATBOX_POS.fX, CHATBOX_POS.fY + CHATBOX_SIZE.fY);

	// Draw input background.

	graphics->DrawBox(position.fX, position.fY, CHATBOX_SIZE.fX, INPUTLINE_HEIGHT, 0x80000000);

	position.fX += CHATBOX_PADDING;

	// Draw current input language id.

	const float currentLanguageWidth = 30.0f;
	const float inputAreaWidth = CHATBOX_SIZE.fX - CHATBOX_PADDING * 2.0f - currentLanguageWidth;

	const CEGUI::Rect rct(position.fX + inputAreaWidth + CHATBOX_PADDING, position.fY, position.fX + inputAreaWidth + currentLanguageWidth, position.fY + INPUTLINE_HEIGHT);
	graphics->DrawBox(position.fX + inputAreaWidth + CHATBOX_PADDING, position.fY, currentLanguageWidth, INPUTLINE_HEIGHT, 0x80FFFFFF);

	const CEGUI::Size scale(1.0f, 1.0f);
	m_font->DrawString(m_currentLanguage, rct, 0.0f, rct, CEGUI::TextFormatting::Centred, 0xFFFFFFFF, scale);

	// Draw the input line text.

	position.fY += INPUTLINE_HEIGHT * 0.5f - m_font->GetFontHeight() * 0.5f;

	IDirect3DDevice9 *const d3ddevice = graphics->GetDevice();

	const RECT scissors = {
		static_cast<LONG>(position.fX), static_cast<LONG>(position.fY),
		static_cast<LONG>(position.fX + inputAreaWidth), static_cast<LONG>(position.fY + INPUTLINE_HEIGHT)
	};
	d3ddevice->SetScissorRect(&scissors);
	d3ddevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);

	const float visualInputOffset = fmax(m_inputCursorOffset - inputAreaWidth, 0.0f);

	position.fX -= visualInputOffset;

	m_font->DrawString(m_inputText.Get(), position, CEGUI::TextFormatting::LeftAligned, 0xFFFFFFFF, 1.0f);

	position.fX += visualInputOffset;

	d3ddevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	// Draw blinking cursor.

	position.fX += m_inputCursorOffset - visualInputOffset;

	float alpha = 0.0f;
	uint64 now = GetTickCount64();
	if (m_lastDrawTime > 0) {
		const float delta = (now - m_lastDrawTime) / 1000.0f;

		m_cursorBlinkTimer += delta;
		if (m_cursorBlinkTimer >= 2.0f) m_cursorBlinkTimer = 0.0f;

		alpha = m_cursorBlinkTimer;
		if (alpha >= 1.0f) {
			alpha = 2.0f - alpha;
		}
	}

	m_lastDrawTime = now;

	graphics->DrawBox(position.fX, position.fY, 2.0f, m_font->GetFontHeight(), CColor(255, 255, 255, static_cast<unsigned char>(alpha * 255)).ToUint());
}

/**
 * Execute an local command.
 *
 * @param command The full input text user type without '/'.
 * @return @c true if any command was found and was executed @c false otherwise.
 */
bool ChatBox::ProcessLocalCommands(const char *const command)
{
	String cmd(command);
	std::vector<String> components = cmd.split(' ');
	assert(components.size() >= 1);

	const char *const commandName(components[0]);

	std::unordered_map<std::string, CommandHandler_t>::iterator it = m_commands.find(commandName);
	if (it != m_commands.end()) {
		std::vector<String> params;
		params.insert(params.begin(), components.begin() + 1, components.end());

		it->second(commandName, params);
		return true;
	}
	return false;
}

/**
 * Process script registered commands.
 *
 * @param input The command text to process without initial '/'.
 */
static bool ProcessScriptCommands(const char *const input)
{
	CClientScriptingManager *const clientScriptingManager = CCore::Instance()->GetClientScriptingManager();
	if (!clientScriptingManager) {
		return false;
	}

	CCommands *const commands = clientScriptingManager->GetCommands();
	if (!commands) {
		return false;
	}

	CPlayerManager *const playerManager = CPlayerManager::Instance();
	if (!playerManager) {
		return false;
	}

	CLocalPlayer *const localPlayer = playerManager->GetLocalPlayer();
	if (!localPlayer) {
		return false;
	}
	return commands->HandleCommand(localPlayer->GetId(), input);
}

/**
 * Calls onClientChat event.
 *
 * @param input     The input typed by user. If the text is command it is a command without / prefix.
 * @param isCommand Is the typed text a command?
 * @return @c true if the called event did not cancel the message to be sent @c false otherwise.
 */
static bool CallOnClientChatEvent(const char *const input, const bool isCommand)
{
	CClientScriptingManager *const clientScriptingManager = CCore::Instance()->GetClientScriptingManager();
	if (!clientScriptingManager) {
		return true;
	}

	CEvents *const events = clientScriptingManager->GetEvents();
	if (!events) {
		return true;
	}

	CSquirrelArguments pArguments;
	pArguments.push(input);
	pArguments.push(isCommand);
	return events->Call("onClientChat", &pArguments).GetInteger() == 1;
}

/**
 * Method called when user confirms typed message.
 */
void ChatBox::SendChatMessage(void)
{
	const char *input = m_inputText.Get();

	const bool isCommand = m_inputText[0] == '/';
	if (isCommand) {
		input += 1;

		if (ProcessLocalCommands(input)) {
			return;
		}
	}

	// The rest of the handling requires client to be connected to network.

	CNetworkModule *const netModule = CCore::Instance()->GetNetworkModule();
	if (!netModule->IsConnected()) {
		return;
	}

	if (isCommand) {
		if (ProcessScriptCommands(input)) {
			return;
		}
	}

	if (!CallOnClientChatEvent(input, isCommand)) {
		return;
	}

	RakNet::BitStream bsData;
	RakNet::RakString data(input);

	if (isCommand) {
		bsData.Write1();
	}
	else {
		bsData.Write0();
	}
	bsData.Write(data);

	netModule->Call(RPC_PLAYER_CHAT, &bsData, HIGH_PRIORITY, RELIABLE_ORDERED, true);
}

/**
 * Handle input of single UTF-32 character.
 */
void ChatBox::HandleCharacterInput(const unsigned character)
{
	// Do not add any character that is not prinatable one.
	if (character < 0x20) {
		return;
	}

	if ((character == 'T' || character == 't') && !m_isInputActive) {
		ActivateInput();
		return;
	}

	if (!m_isInputActive) {
		return;
	}

	char utf8Input[5] = { 0 };
	const unsigned advance = Unicode::ConvertUTF32toUTF8(character, utf8Input);
	if (advance == 0) {
		return;
	}

	m_inputText.Insert(m_inputCursorPosition, utf8Input);
	m_inputCursorPosition += static_cast<size_t>(advance);
	UpdateInputCursorOffset();
}

/**
 * Handle the key input.
 *
 * @param virtualKeyCode Virtual key code of pressed/released button.
 * @param down           Was the button pressed @c true or released @c false?
 * @return @c true if the key was handled and should not be distributed
 *         to more input layers @c false otherwise.
 */
bool ChatBox::HandleKeyInput(const unsigned virtualKeyCode, const bool down)
{
	if (!m_isVisible)
		return false;

	switch (virtualKeyCode) {
	case VK_DOWN:
		if (down && m_isInputActive) {
			InputHistoryPrevious();
			return true;
		}
		break;

	case VK_UP:
		if (down && m_isInputActive) {
			InputHistoryNext();
			return true;
		}
		break;

	case VK_LEFT:
		if (down && m_isInputActive) {
			MoveInputCursorLeft();
			return true;
		}
		break;

	case VK_RIGHT:
		if (down && m_isInputActive) {
			MoveInputCursorRight();
			return true;
		}
		break;

	case VK_HOME:
		if (down && m_isInputActive) {
			MoveInputCursorBegin();
			return true;
		}
		break;

	case VK_END:
		if (down && m_isInputActive) {
			MoveInputCursorEnd();
			return true;
		}
		break;

	case VK_BACK:
		if (down && m_isInputActive) {
			size_t previousCharacterStart = 0;

			unsigned advance = 0;
			unsigned previousCharacterAdvance = 0;
			for (size_t i = 0; i < m_inputCursorPosition; i += advance) {
				const unsigned cp = Unicode::GetUTF8CodePoint(m_inputText.Get(), advance, i);
				if (cp == 0 || IsNil(cp))
					break;

				previousCharacterStart = i;
				previousCharacterAdvance = advance;
			}

			m_inputText.Erase(previousCharacterStart, m_inputCursorPosition - previousCharacterStart);
			m_inputCursorPosition -= static_cast<size_t>(previousCharacterAdvance);
			UpdateInputCursorOffset();
			return true;
		}
		break;

	case VK_DELETE:
		if (down && m_isInputActive) {
			size_t nextCharacterStart = 0;

			unsigned advance = 0;
			unsigned nextCharacterAdvance = 0;
			for (size_t i = m_inputCursorPosition; ; i += advance) {
				const unsigned cp = Unicode::GetUTF8CodePoint(m_inputText.Get(), advance, i);
				if (cp == 0 || IsNil(cp))
					break;

				nextCharacterStart = i;
				nextCharacterAdvance = advance;
				break;
			}

			m_inputText.Erase(nextCharacterStart, nextCharacterAdvance);
			return true;
		}
		break;

	case VK_RETURN:
		if (down) {
			if (m_isInputActive) {
				if (! m_inputText.IsEmpty()) {
					SendChatMessage();

					m_chatInputHistory.push_back(m_inputText);
					m_currentInputHistoryEntry = Nil();
				}

				DeactivateInput();
			}
			else {
				ActivateInput();
			}
			return true;
		}
		break;

	case VK_ESCAPE:
		if (down && m_isInputActive) {
			DeactivateInput();
			return true;
		}
		break;
	}
	return false;
}

/**
 * Move the input cursor to the left.
 */
void ChatBox::MoveInputCursorLeft(void)
{
	const size_t inputTextLen = m_inputText.GetLength();
	m_inputCursorPosition = Unicode::FindPreviousUTF8CharacterStart(m_inputText.Get(), inputTextLen, m_inputCursorPosition);
	UpdateInputCursorOffset();
}

/**
 * Move the input cursor to the right.
 */
void ChatBox::MoveInputCursorRight(void)
{
	const size_t inputTextLen = m_inputText.GetLength();
	m_inputCursorPosition = Unicode::FindNextUTF8CharacterStart(m_inputText.Get(), inputTextLen, m_inputCursorPosition);
	UpdateInputCursorOffset();
}

/**
 * Move the input cursor to the begin of input.
 */
void ChatBox::MoveInputCursorBegin(void)
{
	m_inputCursorPosition = 0;
	UpdateInputCursorOffset();
}

/**
 * Move the input cursor to the end of input
 */
void ChatBox::MoveInputCursorEnd(void)
{
	const size_t inputTextLen = m_inputText.GetLength();
	m_inputCursorPosition = inputTextLen;
	UpdateInputCursorOffset();
}

/**
 * Update input cursor position visual offset based on cursor position.
 */
void ChatBox::UpdateInputCursorOffset(void)
{
	if (m_inputCursorPosition == 0) {
		m_inputCursorOffset = 0.0f;
	}
	else {
		m_inputCursorOffset = m_font->GetTextExtent(m_inputText.substr(0, m_inputCursorPosition));
	}
}

/**
 * Activate the input line.
 */
void ChatBox::ActivateInput(void)
{
	m_isInputActive = true;

	DisableGameInput(true);
}

/**
 * Deactivate the input line.
 */
void ChatBox::DeactivateInput(void)
{
	m_inputText.clear();
	m_isInputActive = false;
	m_inputCursorPosition = 0;

	UpdateInputCursorOffset();
	DisableGameInput(false);
}


/**
 * Disable game input.
 *
 * In case caller wants to disable input when input is already disabled this method does nothing.
 *
 * @param disable @c true if we want to disable game input @c false otherwise.
 */
void ChatBox::DisableGameInput(const bool disable)
{
	if (m_gameInputDisabled == disable) {
		return;
	}

	CPlayerManager *const playerManager = CPlayerManager::Instance();
	if (!playerManager) {
		return;
	}

	CLocalPlayer *const localPlayer = playerManager->GetLocalPlayer();
	if (!localPlayer) {
		return;
	}

	CM2Camera *const camera = CCore::Instance()->GetCamera();
	if (!camera) {
		return;
	}

	if (disable) {
		m_wereControlsLocked = localPlayer->AreControlsLocked();
		localPlayer->LockControls(true);

		m_previousCameraState = camera->IsLocked();
		camera->LockControl(true);
	}
	else {
		localPlayer->LockControls(m_wereControlsLocked);
		camera->LockControl(m_previousCameraState);
	}

	m_gameInputDisabled = disable;
}

/**
 * Select next entry from input history and set it as current input text.
 */
void ChatBox::InputHistoryNext(void)
{
	if (m_chatInputHistory.empty()) {
		return;
	}

	size_t inputHistoryEntry = m_currentInputHistoryEntry;
	if (IsNil(inputHistoryEntry)) {
		inputHistoryEntry = m_chatInputHistory.size() - 1;
	}
	else if (inputHistoryEntry == 0) {
		inputHistoryEntry = Nil();
	}
	else {
		--inputHistoryEntry;
	}

	SetInputAsHistoryEntry(inputHistoryEntry);
}

/**
 * Select previous entry from input history and set it as current input text.
 */
void ChatBox::InputHistoryPrevious(void)
{
	if (m_chatInputHistory.empty()) {
		return;
	}

	size_t inputHistoryEntry = m_currentInputHistoryEntry;
	if (IsNil(inputHistoryEntry)) {
		inputHistoryEntry = 0;
	}
	else if (++inputHistoryEntry == m_chatInputHistory.size()) {
		inputHistoryEntry = Nil();
	}

	SetInputAsHistoryEntry(inputHistoryEntry);
}

/**
 * Set input text as history entry by index.
 *
 * In case @a index is Nil then the input text is cleared.
 *
 * @param[in] index The index of the history entry to set as input.
 */
void ChatBox::SetInputAsHistoryEntry(const size_t index)
{
	if (NotNil(index)) {
		m_inputText = m_chatInputHistory[index];
	}
	else {
		m_inputText.clear();
	}

	m_inputCursorPosition = m_inputText.GetLength();
	m_currentInputHistoryEntry = index;
	UpdateInputCursorOffset();
}

/**
 * Handle WinAPI message.
 *
 * @param[in] uMsg   The message to process.
 * @param[in] wParam Additional message information. The contents of this parameter
 *                   depend on the value of the uMsg parameter.
 * @param[in] lParam Additional message information. The contents of this parameter
 *                   depend on the value of the uMsg parameter.
 * @return @c true if message was handled and should not be send to next input layers @c false otherwise.
 */
bool ChatBox::HandleMessage(const UINT uMsg, const DWORD wParam, const DWORD lParam)
{
	switch (uMsg) {
	case WM_INPUTLANGCHANGE:
		UpdateCurrentLanguage();
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
		return HandleKeyInput(wParam, uMsg == WM_KEYDOWN);

	case WM_CHAR:
		HandleCharacterInput(wParam);
		break;
	}

	return false;
}

/**
 * Internal helper adding message to the chat.
 *
 * @param[in] color        The chat line base color.
 * @param[in] message      The chat line message contents.
 * @param[in] colorCodding Is this message using color codding?
 */
void ChatBox::AddLine(const CColor &color, const String &message, const bool colorCodding)
{
	for (size_t i = MAX_CHAT_MESSAGES; i > 1; --i) {
		m_chatLines[i - 1] = m_chatLines[i - 2];
	}

	m_chatLines[0].message = message;
	m_chatLines[0].color = color;
	m_chatLines[0].colorCodding = colorCodding;
}

/**
 * Internal helper doing the word wrapping of the messages.
 *
 * @param[in] color        The chat line base color.
 * @param[in] message      The chat line message contents.
 * @param[in] colorCodding Is this message using color codding?
 */
void ChatBox::OutputInternal(const CColor &color, const String &message, const bool colorCodding)
{
	unsigned advance = 0;

	const float maxMessageWidth = (CHATBOX_SIZE.fX - CHATBOX_PADDING * 2.0f);

	size_t lastCharacterPosition = 0;

	size_t lastNiceCut = Nil();
	size_t currentStringLength = 0;
	String currentString;
	CColor currentColor(color);
	CColor lastLineColor(color);

	for (size_t i = 0; ; i += advance) {
		const unsigned cp = Unicode::GetUTF8CodePoint(message.Get(), advance, i);
		if (IsNil(cp) || cp == 0) {
			break;
		}

		char character[5] = { 0 };
		Unicode::ConvertUTF32toUTF8(cp, character);
		currentString.Append(character);
		currentStringLength += static_cast<size_t>(advance);

		const size_t currentCharacterPosition = currentStringLength - static_cast<size_t>(advance);

		if (m_font->GetTextExtent(currentString, 1.0f, colorCodding) > maxMessageWidth) {
			const size_t cutPosition = IsNil(lastNiceCut) ? currentCharacterPosition : lastNiceCut;

			AddLine(currentColor, currentString.substr(0, cutPosition), colorCodding);

			currentString = currentString.substr(cutPosition, currentStringLength);
			lastNiceCut = Nil();
			currentStringLength = currentStringLength - cutPosition;

			currentColor = lastLineColor;
		}

		if (cp == ' ' || (colorCodding && cp == '#')) {
			lastNiceCut = currentCharacterPosition;
		}

		if ((i - lastNiceCut) > 10) {
			// In case last nice cut is far from the current character make it obsolete
			// to not cut text at beginning of the message for example.
			lastNiceCut = Nil();
		}

		if (colorCodding) {
			CColor clr;
			if (ParseColor(clr, message.Get() + i)) {
				lastLineColor = clr;
			}
		}
	}

	AddLine(currentColor, currentString, colorCodding);
}

/**
 * Output message to chat without parsing of the color codes.
 *
 * @param[in] color        The chat line base color.
 * @param[in] message      The chat line message contents.
 */
void ChatBox::Output(const CColor &color, const String &message)
{
	OutputInternal(color, message, false);
}

/**
 * Output formatted message to chat without parsing of the color codes.
 *
 * @param[in] color        The chat line base color.
 * @param[in] message      The chat line message contents.
 */
void ChatBox::OutputF(const CColor &color, const char *const message, ...)
{
	String msg;

	va_list args;
	va_start(args, message);
	msg.VAFormat(message, args);
	va_end(args);

	OutputInternal(color, msg, false);
}

/**
 * Output message to chat with parsing of the color codes.
 *
 * @param[in] color        The chat line base color.
 * @param[in] message      The chat line message contents. (can use #RRGGBB color codes)
 */
void ChatBox::OutputColored(const CColor &color, const String &message)
{
	OutputInternal(color, message, true);
}

/**
 * Output formatted message to chat with parsing of the color codes.
 *
 * @param[in] color        The chat line base color.
 * @param[in] message      The chat line message contents. (can use #RRGGBB color codes)
 */
void ChatBox::OutputColoredF(const CColor &color, const char *const message, ...)
{
	String msg;

	va_list args;
	va_start(args, message);
	msg.VAFormat(message, args);
	va_end(args);

	OutputInternal(color, msg, true);
}

/**
 * Format and output chat message send by player.
 *
 * @param[in] sender  The player that send message.
 * @param[in] message The chat message. (If will contain color codes those will be displayed!)
 */
void ChatBox::OutputPlayerMessage(CNetworkPlayer *const sender, const String &message)
{
	String msg;
	msg.Format("#%06X%s: #FFFFFF", sender->GetColour() & 0xFFFFFF, sender->GetNick().Get());
	msg.Append(message);

	OutputColoredF(INFO_MESSAGE_COLOR, msg);
}

/**
 * Set chat visibility.
 *
 * In case user was typing something and this method is called the input will be deactivated.
 *
 * @param[in] visible Should the chat be visible?
 */
void ChatBox::SetVisible(const bool visible)
{
	m_isVisible = visible;

	if (m_isInputActive) {
		DeactivateInput();
	}
}

/** Is chat box visible? */
bool ChatBox::IsVisible(void) const
{
	return m_isVisible;
}

/** Is the input of the chat active? */
bool ChatBox::IsInputActive(void) const
{
	return m_isInputActive;
}

/**
 * Register the local system command.
 *
 * @param command The command name without /.
 * @param handler The handler called when command is executed.
 */
void ChatBox::RegisterCommand(const char *const command, CommandHandler_t handler)
{
	assert(m_commands.find(command) == m_commands.end());
	m_commands.insert(std::pair<std::string, CommandHandler_t>(command, handler));
}

/**
 * Clear chat box contents.
 */
void ChatBox::Clear(void)
{
	for (size_t i = 0; i < MAX_CHAT_MESSAGES; ++i) {
		m_chatLines[i].message.clear();
	}
	DeactivateInput();
}

/**
 * Update current language name.
 */
void ChatBox::UpdateCurrentLanguage(void)
{
	HKL kbLayout = GetKeyboardLayout(0);
	WORD langId = static_cast<WORD>((unsigned)(kbLayout) & 0xffff);

	if (!GetLocaleInfo(MAKELCID(langId, 0), LOCALE_SISO639LANGNAME, m_currentLanguage, LOCALE_NAME_MAX_LENGTH)) {
		strcpy(m_currentLanguage, "en");
	}
}

/* eof */
