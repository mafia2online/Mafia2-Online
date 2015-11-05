/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUI_Impl.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

// Message box types
enum eMessageBoxFlags
{
	MB_NONE = 0,
	MB_ICON_INFO = 1,
	MB_ICON_QUESTION = 2,
	MB_ICON_WARNING = 4,
	MB_ICON_ERROR = 8
};

class CGUIElement_Impl;
class CGUIFont_Impl;
class CGUIWindow_Impl;
class CGUIProgressBar_Impl;
class CGUIStaticImage_Impl;
class CGUIButton_Impl;
class CGUICheckBox_Impl;
class CGUIComboBox_Impl;
class CGUIEdit_Impl;
class CGUILabel_Impl;
class CGUIRadioButton_Impl;
class CGUITabPanel_Impl;
class CGUIGridList_Impl;
class CGUIScrollBar_Impl;
class CGUIScrollPane_Impl;
class CGUIMessageBox_Impl;

class CGUI_Impl
{

protected:

	IDirect3DDevice9										* m_pDevice;
	CEGUI::Renderer											* m_pRenderer;
	CEGUI::System											* m_pSystem;
	CEGUI::FontManager										* m_pFontManager;
	CEGUI::ImagesetManager									* m_pImageSetManager;
	CEGUI::SchemeManager									* m_pSchemeManager;
	CEGUI::WindowManager									* m_pWindowManager;

	CEGUI::DefaultWindow									* m_pDefaultWindow;
	CEGUI::MouseCursor										* m_pCursor;

	CGUIFont_Impl											* m_pDefaultFont;
	CGUIFont_Impl											* m_pDefaultFontBold;

	unsigned int											m_uiUnique;
	std::list< CGUIElement_Impl* >							m_redrawQueue;

	// Key callbacks
	GUI_CALLBACK_KEY										m_CharacterKeyHandler;
	GUI_CALLBACK_KEY										m_KeyDownHandler;

	// Mouse callbacks
	GUI_CALLBACK_MOUSE										m_MouseClickHandler;
	GUI_CALLBACK_MOUSE										m_MouseDoubleClickHandler;
	GUI_CALLBACK_MOUSE										m_MouseButtonDownHandler;
	GUI_CALLBACK_MOUSE										m_MouseButtonUpHandler;
	GUI_CALLBACK_MOUSE										m_MouseMoveHandler;
	GUI_CALLBACK_MOUSE										m_MouseEnterHandler;
	GUI_CALLBACK_MOUSE										m_MouseLeaveHandler;
	GUI_CALLBACK_MOUSE										m_MouseWheelHandler;

	GUI_CALLBACK_FOCUS										m_FocusGainedHandler;
	GUI_CALLBACK_FOCUS										m_FocusLostHandler;

	bool						Event_RedrawRequest			( const CEGUI::EventArgs &e );

	// Key events
	bool						Event_OnKeyDown				( const CEGUI::EventArgs &e );
	bool						Event_CharacterKey			( const CEGUI::EventArgs &e );

	// Mouse events
	bool						Event_MouseClick			( const CEGUI::EventArgs &e );
	bool						Event_MouseDoubleClick		( const CEGUI::EventArgs &e );
	bool						Event_MouseButtonDown		( const CEGUI::EventArgs &e );
	bool						Event_MouseButtonUp			( const CEGUI::EventArgs &e );
	bool						Event_MouseWheel			( const CEGUI::EventArgs &e );
	bool						Event_MouseMove				( const CEGUI::EventArgs &e );
	bool						Event_MouseEnter			( const CEGUI::EventArgs &e );
	bool						Event_MouseLeave			( const CEGUI::EventArgs &e );

	//
	bool						Event_FocusGained			( const CEGUI::EventArgs &e );
	bool						Event_FocusLost				( const CEGUI::EventArgs &e );

public:

								CGUI_Impl					( IDirect3DDevice9 * pDevice );
								~CGUI_Impl					( void );

	void						SetResolution				( float fWidth, float fHeight );
	Vector2						GetResolution				( void );

	void						Draw						( void );
	void						DrawMouseCursor				( void );

	void						OnDeviceLost				( void );
	void						OnDeviceRestore				( void );

	void						ProcessMouseInput			( eMouseType type, float fX, float fY, int iButton = 0 );
	bool						ProcessKeyboardInput		( unsigned long ulKey, bool bIsDown );
	bool						ProcessCharacter			( unsigned long ulCharacter );

	void						CleanDeadPool				( void );

	void						SetCursorEnabled			( bool bCursorEnabled );
	bool						IsCursorEnabled				( void );

	bool						IsInputEnabled				( void );

	String						GetUniqueName				( void );

	void						AddChild					( CGUIElement_Impl * pChild );

	void						AddToRedrawQueue			( CGUIElement_Impl * pElement );
	void						RemoveFromRedrawQueue		( CGUIElement_Impl * pElement );

	static CEGUI::String		GetUTFString				( const char * szInput );
    static CEGUI::String		GetUTFString                ( const String &strInput );

	CGUIFont_Impl				* GetDefaultFont			( bool bBold = false ) { return (bBold ? m_pDefaultFontBold : m_pDefaultFont); }

	CGUIWindow_Impl				* CreateWnd					( String strCaption, CGUIElement_Impl * pParent = NULL );
	CGUIFont_Impl				* CreateFnt					( String strName, String strFile, unsigned int uiSize = 8, unsigned int uFlags = 0, bool bAutoScale = false );

	CGUIProgressBar_Impl		* CreateProgressBar			( CGUIElement_Impl * pParent = NULL );
	CGUIStaticImage_Impl		* CreateStaticImage			( CGUIElement_Impl * pParent = NULL );
	CGUIButton_Impl				* CreateButton				( String strCaption, CGUIElement_Impl * pParent = NULL );
	CGUICheckBox_Impl			* CreateCheckBox			( String strCaption, CGUIElement_Impl * pParent = NULL );
	CGUIComboBox_Impl			* CreateComboBox			( const char * szCaption, CGUIElement_Impl * pParent = NULL );
	CGUIEdit_Impl				* CreateEdit				( String strText, CGUIElement_Impl * pParent = NULL );
	CGUILabel_Impl				* CreateLabel				( String strText, CGUIFont_Impl * pFont = NULL, CGUIElement_Impl * pParent = NULL );
	CGUIRadioButton_Impl		* CreateRadioButton			( String strCaption, CGUIElement_Impl * pParent = NULL );
	CGUITabPanel_Impl			* CreateTabPanel			( CGUIElement_Impl * pParent = NULL );
	CGUIGridList_Impl			* CreateGridList			( CGUIElement_Impl * pParent = NULL );
	CGUIScrollBar_Impl			* CreateScrollBar			( bool bHorizonatal, CGUIElement_Impl * pParent = NULL );
	CGUIScrollPane_Impl			* CreateScrollPane			( CGUIElement_Impl * pParent = NULL );
	CGUIMessageBox_Impl			* CreateMessageBox			( const char * szTitle, const char * szCaption, const char * szButton1 = "", const char * szButton2 = "" );

	CEGUI::Renderer				* GetRenderer				( void ) { return m_pRenderer; }
	CEGUI::FontManager			* GetFontManager			( void ) { return m_pFontManager; }
	CEGUI::ImagesetManager		* GetImageSetManager		( void ) { return m_pImageSetManager; }
	CEGUI::SchemeManager		* GetSchemeManager			( void ) { return m_pSchemeManager; }
	CEGUI::WindowManager		* GetWindowManager			( void ) { return m_pWindowManager; }

	// Key handlers
	void						SetCharacterKeyHandler		( const GUI_CALLBACK_KEY &callback ) { m_CharacterKeyHandler = callback; }
	const GUI_CALLBACK_KEY		GetCharacterKeyHandler		( void ) { return m_CharacterKeyHandler; }
	void						SetKeyDownHandler			( const GUI_CALLBACK_KEY &callback ) { m_KeyDownHandler = callback; }
	const GUI_CALLBACK_KEY		GetKeyDownHandler			( void ) { return m_KeyDownHandler; }

	// Mouse handlers
	void						SetMouseClickHandler		( const GUI_CALLBACK_MOUSE &callback ) { m_MouseClickHandler = callback; }
	const GUI_CALLBACK_MOUSE	GetMouseClickHandler		( void ) { return m_MouseClickHandler; }
	void						SetMouseDoubleClickHandler	( const GUI_CALLBACK_MOUSE &callback ) { m_MouseDoubleClickHandler = callback; }
	const GUI_CALLBACK_MOUSE	GetMouseDoubleClickHandler	( void ) { return m_MouseDoubleClickHandler; }
	void						SetMouseButtonDownHandler	( const GUI_CALLBACK_MOUSE &callback ) { m_MouseButtonDownHandler = callback; }
	const GUI_CALLBACK_MOUSE	GetMouseButtonDownHandler	( void ) { return m_MouseButtonDownHandler; }
	void						SetMouseButtonUpHandler		( const GUI_CALLBACK_MOUSE &callback ) { m_MouseButtonUpHandler = callback; }
	const GUI_CALLBACK_MOUSE	GetMouseButtonUpHandler		( void ) { return m_MouseButtonUpHandler; }
	void						SetMouseMoveHandler			( const GUI_CALLBACK_MOUSE &callback ) { m_MouseMoveHandler = callback; }
	const GUI_CALLBACK_MOUSE	GetMouseMoveHandler			( void ) { return m_MouseMoveHandler; }
	void						SetMouseEnterHandler		( const GUI_CALLBACK_MOUSE &callback ) { m_MouseEnterHandler = callback; }
	const GUI_CALLBACK_MOUSE	GetMouseEnterHandler		( void ) { return m_MouseEnterHandler; }
	void						SetMouseLeaveHandler		( const GUI_CALLBACK_MOUSE &callback ) { m_MouseLeaveHandler = callback; }
	const GUI_CALLBACK_MOUSE	GetMouseLeaveHandler		( void ) { return m_MouseLeaveHandler; }
	void						SetMouseWheelHandler		( const GUI_CALLBACK_MOUSE &callback ) { m_MouseWheelHandler = callback; }
	const GUI_CALLBACK_MOUSE	GetMouseWheelHandler		( void ) { return m_MouseWheelHandler; }

	//
	void						SetFocusGainedHandler		( const GUI_CALLBACK_FOCUS &callback ) { m_FocusGainedHandler = callback; }
	const GUI_CALLBACK_FOCUS	GetFocusGainedHandler		( void ) { return m_FocusGainedHandler; }
	void						SetFocusLostHandler			( const GUI_CALLBACK_FOCUS &callback ) { m_FocusLostHandler = callback; }
	const GUI_CALLBACK_FOCUS	GetFocusLostHandler			( void ) { return m_FocusGainedHandler; }

};