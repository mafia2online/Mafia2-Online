/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUI_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "CLogFile.h"

#include "CGUI.h"
#include "CString.h"
#include "Math/CVector3.h"

#include "CGUICallback.h"
#include "CGUIEvent.h"

#include "CGUIElement_Impl.h"
#include "CGUIFont_Impl.h"
#include "CGUIButton_Impl.h"
#include "CGUICheckBox_Impl.h"
#include "CGUIComboBox_Impl.h"
#include "CGUIEdit_Impl.h"
#include "CGUIGridList_Impl.h"
#include "CGUILabel_Impl.h"
#include "CGUIListItem_Impl.h"
#include "CGUIMessageBox_Impl.h"
#include "CGUIProgressBar_Impl.h"
#include "CGUIRadioButton_Impl.h"
#include "CGUIScrollBar_Impl.h"
#include "CGUIScrollPane_Impl.h"
#include "CGUIStaticImage_Impl.h"
#include "CGUITab_Impl.h"
#include "CGUITabPanel_Impl.h"
#include "CGUITexture_Impl.h"
#include "CGUIWindow_Impl.h"

#include "SharedUtility.h"

#include "CGUI_Impl.h"

/**
 * Helper used to set current app dir as gui.
 *
 * @fixme This is SUPER UNSAFE as different thread may try to read some file and expect
 *        the current directory to be game working directory.
 *        The correct way will be to set CEGUI Resource paths to this directory.
 */
void SetCurrentDirAsGui(void)
{
	String guiDir;
	guiDir.Format ( "%s\\data\\gui\\", CCore::Instance()->GetModDirectory().Get() );
	SetCurrentDirectory( guiDir );
}

CGUI_Impl::CGUI_Impl( IDirect3DDevice9 * pDevice )
	: m_pDevice(nullptr)
	, m_pRenderer()
	, m_pSystem()
	, m_pFontManager(nullptr)
	, m_pImageSetManager(nullptr)
	, m_pSchemeManager(nullptr)
	, m_pWindowManager(nullptr)

	, m_pDefaultWindow(nullptr)
	, m_pCursor(nullptr)

	, m_pDefaultFont(nullptr)
	, m_pDefaultFontBold(nullptr)

	, m_uiUnique(0)
	, m_redrawQueue()
	, m_redrawQueueMutex()

	, m_CharacterKeyHandler()
	, m_KeyDownHandler()

	, m_MouseClickHandler()
	, m_MouseDoubleClickHandler()
	, m_MouseButtonDownHandler()
	, m_MouseButtonUpHandler()
	, m_MouseMoveHandler()
	, m_MouseEnterHandler()
	, m_MouseLeaveHandler()
	, m_MouseWheelHandler()

	, m_FocusGainedHandler()
	, m_FocusLostHandler()
{
	// Set the current directory to the mod folder
	SetCurrentDirectory( CCore::Instance()->GetModDirectory().Get() );

	// Store the d3d9 device
	m_pDevice = pDevice;

	// Create the gui renderer and system
	m_pRenderer = std::make_unique<CEGUI::DirectX9Renderer>( pDevice, 0 );
	m_pSystem = std::make_unique<CEGUI::System>( m_pRenderer.get() );

	// Get pointers to the cegui singletons
	m_pFontManager = CEGUI::FontManager::getSingletonPtr();
	m_pImageSetManager = CEGUI::ImagesetManager::getSingletonPtr();
	m_pSchemeManager = CEGUI::SchemeManager::getSingletonPtr();
	m_pWindowManager = CEGUI::WindowManager::getSingletonPtr();

	// Enable CEGUI logger
#ifdef _DEBUG
	CEGUI::Logger::getSingleton().setLoggingLevel( CEGUI::Informative );
#else
	CEGUI::Logger::getSingleton().setLoggingLevel( CEGUI::Standard );
#endif
	CEGUI::Logger::getSingleton().setLogFilename( "logs\\gui.log" );

	// Set the current directory to the gui folder
	SetCurrentDirAsGui();

	try
	{
		// Load the default fonts
		m_pDefaultFont = CreateFnt( "tahoma", "fonts\\tahoma.ttf" );
		m_pDefaultFontBold = CreateFnt( "tahoma-bold", "fonts\\tahoma-bold.ttf" );
	}
	catch ( CEGUI::InvalidRequestException e )
	{
		CLogFile::Printf ( "Failed to load CEGUI fonts!" );
		TerminateProcess ( GetCurrentProcess(), 0 );
	}

	// Load the default skin
	CEGUI::SchemeManager::getSingleton().loadScheme( "skins\\default.xml" );
	CEGUI::System::getSingleton().setDefaultMouseCursor( "CGUI-Images", "MouseArrow" );

	// Create the gui root
	m_pDefaultWindow = (CEGUI::DefaultWindow *)m_pWindowManager->createWindow( "DefaultWindow", "guiroot" );

	// Set the available chars (english and cyrillic support)
	CEGUI::String tmp;
	for (CEGUI::utf32 cp = 0x20; cp <= 0x7E; ++cp)
		tmp += cp;
	for (CEGUI::utf32 cp = 0x400; cp <= 0x4FF; ++cp)
		tmp += cp;
	m_pDefaultFont->GetFont()->defineFontGlyphs(tmp);

	// Set the default gui sheet
	m_pSystem->setGUISheet( m_pDefaultWindow );

	// Disable single click timeouts
	m_pSystem->setSingleClickTimeout( 100000000.0f );

	// Set the default font
	m_pSystem->setDefaultFont( m_pDefaultFont->GetFont() );

	// Get the default cursor
	m_pCursor = CEGUI::MouseCursor::getSingletonPtr();

	// Hide the mouse cursor
	m_pCursor->hide();

	// Global Events
	CEGUI::GlobalEventSet * pEvents = CEGUI::GlobalEventSet::getSingletonPtr();
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventRedrawRequested, CEGUI::Event::Subscriber( &CGUI_Impl::Event_RedrawRequest, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventActivated, CEGUI::Event::Subscriber( &CGUI_Impl::Event_FocusGained, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventDeactivated, CEGUI::Event::Subscriber( &CGUI_Impl::Event_FocusLost, this ) );

	// Global key events
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventKeyDown, CEGUI::Event::Subscriber( &CGUI_Impl::Event_OnKeyDown, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventCharacterKey, CEGUI::Event::Subscriber( &CGUI_Impl::Event_CharacterKey, this ) );

	// Global Mouse events
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber( &CGUI_Impl::Event_MouseClick, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventMouseDoubleClick, CEGUI::Event::Subscriber( &CGUI_Impl::Event_MouseDoubleClick, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber( &CGUI_Impl::Event_MouseButtonDown, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber( &CGUI_Impl::Event_MouseButtonUp, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventMouseWheel, CEGUI::Event::Subscriber( &CGUI_Impl::Event_MouseWheel, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventMouseMove, CEGUI::Event::Subscriber( &CGUI_Impl::Event_MouseMove, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber( &CGUI_Impl::Event_MouseEnter, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventMouseLeaves, CEGUI::Event::Subscriber( &CGUI_Impl::Event_MouseLeave, this ) );

	// Reset the current directory
	SetCurrentDirectory( CCore::Instance()->GetModDirectory().Get() );
}

CGUI_Impl::~CGUI_Impl( void )
{
	// In case it asserts here you have some gui element leak. Check the content of list
	// to track down what is the problem and where.
	assert(m_elementsList.empty());
}

void CGUI_Impl::NotifyElementCreate( CGUIElement_Impl *element )
{
	m_elementsList.push_back(element);
}

void CGUI_Impl::NotifyElementDestroy( CGUIElement_Impl *element )
{
	m_elementsList.remove(element);
}

void CGUI_Impl::SetResolution( float fWidth, float fHeight )
{
	m_pRenderer->setDisplaySize( CEGUI::Size( fWidth, fHeight ) );
}

Vector2 CGUI_Impl::GetResolution( void )
{
	return Vector2( m_pRenderer->getWidth(), m_pRenderer->getHeight() );
}

void CGUI_Impl::Draw( void )
{
	{
		std::lock_guard<std::mutex> lock(m_redrawQueueMutex);
		// Is the redraw queue not empty?
		if( !m_redrawQueue.empty() )
		{
			// Loop over all elements in the redraw queue
			std::list< CGUIElement_Impl* >::const_iterator iter = m_redrawQueue.begin();
			for( ; iter != m_redrawQueue.end(); iter++ )
			{
				// Redraw the current element
				(*iter)->ForceRedraw();
			}

			// Clear the redraw queue
			m_redrawQueue.clear();
		}
	}

	// Render the gui
	m_pSystem->renderGUI();
}

void CGUI_Impl::DrawMouseCursor( void )
{
	// Draw the mouse cursor
	m_pCursor->draw();
}

void CGUI_Impl::OnDeviceLost( void )
{
	try
	{
		m_pRenderer->preD3DReset();
	}
	catch ( CEGUI::RendererException &e )
	{
		MessageBox ( NULL, e.getMessage().c_str(), MOD_NAME " - CEGUI Render Exception", MB_ICONERROR|MB_OK );
		TerminateProcess ( GetCurrentProcess(), 0 );
	}
}

void CGUI_Impl::OnDeviceRestore( void )
{
	SetCurrentDirAsGui();

	CLogFile::Printf( "[GFX] Restoring CEGUI render device..." );

	try
	{
		m_pRenderer->postD3DReset();
	}
	catch ( CEGUI::RendererException &e )
	{
		MessageBox ( NULL, e.getMessage().c_str(), MOD_NAME " - CEGUI Render Exception", MB_ICONERROR|MB_OK );
		TerminateProcess ( GetCurrentProcess(), 0 );
	}

	CLogFile::Printf( "[GFX] Done!" );

	// Reset the current directory
	SetCurrentDirectory( CCore::Instance()->GetModDirectory().Get() );
}

void CGUI_Impl::ProcessMouseInput( eMouseType type, float fX, float fY, int iButton )
{
	// Switch the mouse event type
	switch( type )
	{
		// Inject the mouse movement into cegui
	case eMouseType::MOVE:
		m_pSystem->injectMouseMove( fX, fY );
		break;

		// Inject the mouse position into cegui
	case eMouseType::POSITION:
		m_pSystem->injectMousePosition( fX, fY );
		break;

		// Inject the mouse scroll into cegui
	case eMouseType::SCROLL:
		m_pSystem->injectMouseWheelChange( (int)fX );
		break;

		// Inject the button down
	case eMouseType::BUTTON_DOWN:
		m_pSystem->injectMouseButtonDown( (CEGUI::MouseButton)iButton );
		break;

		// Inject the button up
	case eMouseType::BUTTON_UP:
		m_pSystem->injectMouseButtonUp( (CEGUI::MouseButton)iButton );
		break;
	}
}

bool CGUI_Impl::ProcessKeyboardInput( unsigned long ulKey, bool bIsDown )
{
	bool bReturn = false;

	// Inject the key into cegui
	if( bIsDown )
		bReturn = m_pSystem->injectKeyDown( ulKey );
	else
		bReturn = m_pSystem->injectKeyUp( ulKey );

	return bReturn;
}

bool CGUI_Impl::ProcessCharacter( unsigned long ulCharacter )
{
	// Inject the character input into cegui
	return m_pSystem->injectChar( ulCharacter );
}

void CGUI_Impl::CleanDeadPool( void )
{
	// Clean the dead pool
	if( m_pWindowManager )
		m_pWindowManager->cleanDeadPool();
}

void CGUI_Impl::SetCursorEnabled( bool bCursorEnabled )
{
	// Toggle the cursor
	if( bCursorEnabled )
		m_pCursor->show();
	else
		m_pCursor->hide();
}

bool CGUI_Impl::IsCursorEnabled( void )
{
	return m_pCursor->isVisible();
}

bool CGUI_Impl::IsInputEnabled( void )
{
	// Get the active window
	CEGUI::Window * pWindow = m_pDefaultWindow->getActiveChild();

	// Don't we have a valid active child?
	if( !pWindow || pWindow == m_pDefaultWindow || !pWindow->isVisible() )
		return false;

	// Is the active window an edit box?
	if( pWindow->getType() == "CGUI/Editbox" )
	{
		// Cast the window to an editbox
		CEGUI::Editbox * pEditBox = (CEGUI::Editbox *)pWindow;

		//
		return ( !pEditBox->isReadOnly() && pEditBox->hasInputFocus() );
	}
	// Is the active window a multi-line edit box?
	else if( pWindow->getType() == "CGUI/MultiLineEditbox" )
	{
		// Cast the window to a multi-line editbox
		CEGUI::MultiLineEditbox * pMultiLineEditBox = (CEGUI::MultiLineEditbox *)pWindow;

		//
		return ( !pMultiLineEditBox->isReadOnly() && pMultiLineEditBox->hasInputFocus() );
	}

	return false;
}

String CGUI_Impl::GetUniqueName( void )
{
	String strName;
	strName.Format ("window_%d", m_uiUnique );

	m_uiUnique++;
	return strName;
}

void CGUI_Impl::AddChild( CGUIElement_Impl * pChild )
{
	m_pDefaultWindow->addChildWindow( pChild->GetWindow() );
}

void CGUI_Impl::AddToRedrawQueue( CGUIElement_Impl * pElement )
{
	std::lock_guard<std::mutex> lock(m_redrawQueueMutex);
	if (m_redrawQueue.size() != 0)
	{
		std::list< CGUIElement_Impl* >::const_iterator iter = m_redrawQueue.begin();
		for( ; iter != m_redrawQueue.end(); ++iter )
		{
			if (m_redrawQueue.empty()) break;

			if( pElement->GetParent() == *iter )
				return;
			else if( (*iter)->GetParent() == pElement )
			{
				m_redrawQueue.remove( *iter );
				if( m_redrawQueue.empty() )
					return;
				iter = m_redrawQueue.begin();
			}
			else if( *iter == pElement )
				return;
		}
	}
	m_redrawQueue.push_back( pElement );
}

void CGUI_Impl::RemoveFromRedrawQueue( CGUIElement_Impl * pElement )
{
	std::lock_guard<std::mutex> lock(m_redrawQueueMutex);
	if (m_redrawQueue.size() <= 0)
		return;
	m_redrawQueue.remove( pElement );
}

CEGUI::String CGUI_Impl::GetUTFString( const char * szInput )
{
	CEGUI::String strUTF = (CEGUI::utf8 *)szInput;
	return strUTF;
}

CEGUI::String CGUI_Impl::GetUTFString( const String &strInput )
{
	return GetUTFString( strInput.Get() );
}

bool CGUI_Impl::Event_RedrawRequest( const CEGUI::EventArgs &e )
{
	const CEGUI::WindowEventArgs &arg = (CEGUI::WindowEventArgs&)e;

	// Get the element
	CGUIElement_Impl * pElement = (CGUIElement_Impl *)arg.window->getUserData();

	// If the element is valid add it to the redraw queue, if not force a window redraw
	if( pElement )
		AddToRedrawQueue( pElement );
	else
		arg.window->forceRedraw();

	return true;
}

bool CGUI_Impl::Event_OnKeyDown( const CEGUI::EventArgs &e )
{
	// Cast the argument to a key event
	const CEGUI::KeyEventArgs &keyArgs = (const CEGUI::KeyEventArgs &)e;

	//
	switch( keyArgs.scancode )
	{
		// Copy/cut keys
	case CEGUI::Key::Scan::C:
	case CEGUI::Key::Scan::X:
		{
			// Is control being pressed?
			if( keyArgs.sysKeys & CEGUI::Control )
			{
				// Temp string
				CEGUI::String strTemp;

				// Get the cegui window
				CEGUI::Window * pWindow = (CEGUI::Window *)keyArgs.window;

				// Is this an editbox?
				if( pWindow->getType() == "CGUI/Editbox" )
				{
					// Cast the window to a cegui editbox
					CEGUI::Editbox * pEditBox = (CEGUI::Editbox *)pWindow;

					// Get the text from the editbox
					size_t sSelectionStart = pEditBox->getSelectionStartIndex();
					size_t sSelectionEnd = pEditBox->getSelectionLength();
					strTemp = pEditBox->getText().substr( sSelectionStart, sSelectionEnd );

					// Are we cutting the text?
					if( keyArgs.scancode == CEGUI::Key::Scan::X )
					{
						// Is the editbox not read only?
						if( !pEditBox->isReadOnly() )
						{
							// Get the edit box text
							CEGUI::String strTemp2 = pEditBox->getText();

							// Cut the selection from the edit box text
							strTemp2.replace( sSelectionStart, sSelectionEnd, "", 0 );

							// Update the edit box text
							pEditBox->setText( strTemp2 );
						}
					}
				}
				else if( pWindow->getType() == "CGUI/MultiLineEditBox" )
				{
					// Cast the window to a cegui multiline editbox
					CEGUI::MultiLineEditbox * pEditBox = (CEGUI::MultiLineEditbox *)pWindow;

					// Get the text from the editbox
					size_t sSelectionStart = pEditBox->getSelectionStartIndex();
					size_t sSelectionEnd = pEditBox->getSelectionLength();
					strTemp = pEditBox->getText().substr( sSelectionStart, sSelectionEnd );

					// Are we cutting the text?
					if( keyArgs.scancode == CEGUI::Key::Scan::X )
					{
						// Is the editbox not read only?
						if( !pEditBox->isReadOnly() )
						{
							// Get the edit box text
							CEGUI::String strTemp2 = pEditBox->getText();

							// Cut the selection from the edit box text
							strTemp2.replace( sSelectionStart, sSelectionEnd, "", 0 );

							// Update the edit box text
							pEditBox->setText( strTemp2 );
						}
					}
				}

				// Do we have any data to copy?
				if( strTemp.length() > 0 )
				{
					// Copy the string to the clipboard
					SharedUtility::SetClipboardText( strTemp.c_str(), strTemp.length() );
				}
			}
			break;
		}

		// Paste
	case CEGUI::Key::Scan::V:
		{
			// Is control being pressed?
			if( keyArgs.sysKeys & CEGUI::Control )
			{
				// Get the cegui window
				CEGUI::Window * pWindow = (CEGUI::Window *)keyArgs.window;

				// Is this an editbox?
				if( pWindow->getType() == "CGUI/Editbox" )
				{
					// Cast the window to a cegui editbox
					CEGUI::Editbox * pEditBox = (CEGUI::Editbox *)pWindow;

					// Get the clipboard text
					const char * szClipboard = SharedUtility::GetClipboardText();

					// Do we have anything in the clipboard?
					if( szClipboard )
					{
						//
						std::string strClipboard( szClipboard );
						size_t sNewlineIndex;

						// Replace the newlines and insert spaces insted
						do
						{
							// Find any newline charactes
							sNewlineIndex = strClipboard.find( '\n' );

							// Did we find the newline?
							if( sNewlineIndex != std::string::npos )
							{
								// Valid?
								if( sNewlineIndex > 0 && strClipboard[ sNewlineIndex - 1 ] == '\r' )
								{
									//
									strClipboard[ sNewlineIndex - 1 ] = ' ';
									strClipboard.replace( sNewlineIndex, (strClipboard.length() - sNewlineIndex), strClipboard, (sNewlineIndex + 1), (strClipboard.length() - sNewlineIndex - 1) );
								}
								else
								{
									strClipboard[ sNewlineIndex ] = ' ';
								}
							}
						}
						while( sNewlineIndex != std::string::npos );

						// Get the edit box text
						CEGUI::String strTemp = pEditBox->getText();

						//
						if( (strClipboard.length() + strTemp.length() ) < pEditBox->getMaxTextLength() )
						{
							size_t sCaratIndex = 0;

							// Have we got anything selected?
							if( pEditBox->getSelectionLength() > 0 )
							{
								// Replace the text
								strTemp.replace( pEditBox->getSelectionStartIndex(), pEditBox->getSelectionLength(), strClipboard.c_str(), strClipboard.length() );

								//
								sCaratIndex = pEditBox->getSelectionStartIndex() + strClipboard.length();
							}
							else
							{
								// Insert the text
								strTemp.insert( pEditBox->getSelectionStartIndex(), strClipboard.c_str(), strClipboard.length() );

								//
								sCaratIndex = pEditBox->getCaratIndex() + strClipboard.length();
							}

							// Update the text
							pEditBox->setText( strTemp );

							// Set the carat index
							pEditBox->setCaratIndex( sCaratIndex );
						}
						else
						{
							// Fire the edit box full event
							pEditBox->fireEvent( CEGUI::Editbox::EventEditboxFull, CEGUI::WindowEventArgs( pEditBox ) );
						}
					}
				}
			}
			break;
		}

		// Select all
	case CEGUI::Key::Scan::A:
		{
			// Is control being pressed?
			if( keyArgs.sysKeys & CEGUI::Control )
			{
				// Get the cegui window
				CEGUI::Window * pWindow = (CEGUI::Window *)keyArgs.window;

				// Is this an editbox?
				if( pWindow->getType() == "CGUI/Editbox" )
				{
					// Cast to an edit box
					CEGUI::Editbox * pEditBox = (CEGUI::Editbox *)pWindow;

					// Set the edit box selection
					pEditBox->setSelection( 0, pEditBox->getText().size() );
				}
				else if( pWindow->getType() == "CGUI/MultiLineEditBox" )
				{
					// Cast to an multiline edit box
					CEGUI::MultiLineEditbox * pMultiLineEditBox = (CEGUI::MultiLineEditbox *)pWindow;

					// Set the multiline edit box selection
					pMultiLineEditBox->setSelection( 0, pMultiLineEditBox->getText().size() );
				}
			}
			break;
		}
	}

	// Do we have a key down handler?
	if( m_KeyDownHandler )
	{
		// Copy the variables
		CGUIKeyEventArgs args;
		args.codepoint = keyArgs.codepoint;
		args.scancode = (CGUIKeys::Scan)keyArgs.scancode;
		args.sysKeys = keyArgs.sysKeys;
		args.pWindow = (CGUIElement_Impl *)keyArgs.window->getUserData();

		// Call the key down handler
		m_KeyDownHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_CharacterKey( const CEGUI::EventArgs &e )
{
	// Do we have a character key handler?
	if( m_CharacterKeyHandler )
	{
		// Cast the event to a key event
		const CEGUI::KeyEventArgs &keyArgs = (const CEGUI::KeyEventArgs &)e;

		// Copy the variables
		CGUIKeyEventArgs args;
		args.codepoint = keyArgs.codepoint;
		args.scancode = (CGUIKeys::Scan)keyArgs.scancode;
		args.sysKeys = keyArgs.sysKeys;
		args.pWindow = (CGUIElement_Impl *)keyArgs.window->getUserData();

		// Call the character key handler
		m_CharacterKeyHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_MouseClick( const CEGUI::EventArgs &e )
{
	// Do we have a mouse click handler?
	if( m_MouseClickHandler )
	{
		// Cast the event to a mouse event
		const CEGUI::MouseEventArgs &mouseArgs = (const CEGUI::MouseEventArgs &)e;

		// Get the cegui window
		CEGUI::Window * pWindow = mouseArgs.window;

		// Get the appropriate parent
		if( pWindow->testClassName( CEGUI::Titlebar::EventNamespace ) || pWindow->testClassName( CEGUI::Scrollbar::EventNamespace ) )
			pWindow = pWindow->getParent();

		// Copy the variables
		CGUIMouseEventArgs args;
		args.button = (CGUIMouse::MouseButton)mouseArgs.button;
		args.position = Vector2( mouseArgs.position.d_x, mouseArgs.position.d_y );
		args.moveDelta = Vector2( mouseArgs.moveDelta.d_x, mouseArgs.moveDelta.d_y );
		args.sysKeys = mouseArgs.sysKeys;
		args.wheelChange = mouseArgs.wheelChange;
		args.pWindow = (CGUIElement_Impl *)pWindow->getUserData();

		// Call the mouse click handler
		m_MouseClickHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_MouseDoubleClick( const CEGUI::EventArgs &e )
{
	// Do we have a mouse double click handler?
	if( m_MouseDoubleClickHandler )
	{
		// Cast the event to a mouse event
		const CEGUI::MouseEventArgs &mouseArgs = (const CEGUI::MouseEventArgs &)e;

		// Get the cegui window
		CEGUI::Window * pWindow = mouseArgs.window;

		// Get the appropriate parent
		if( pWindow->testClassName( CEGUI::Titlebar::EventNamespace ) || pWindow->testClassName( CEGUI::Scrollbar::EventNamespace ) )
			pWindow = pWindow->getParent();

		// Copy the variables
		CGUIMouseEventArgs args;
		args.button = (CGUIMouse::MouseButton)mouseArgs.button;
		args.position = Vector2( mouseArgs.position.d_x, mouseArgs.position.d_y );
		args.moveDelta = Vector2( mouseArgs.moveDelta.d_x, mouseArgs.moveDelta.d_y );
		args.sysKeys = mouseArgs.sysKeys;
		args.wheelChange = mouseArgs.wheelChange;
		args.pWindow = (CGUIElement_Impl *)pWindow->getUserData();

		// Call the mouse click handler
		m_MouseDoubleClickHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_MouseButtonDown( const CEGUI::EventArgs &e )
{
	// Do we have a mouse button down handler?
	if( m_MouseButtonDownHandler )
	{
		// Cast the event to a mouse event
		const CEGUI::MouseEventArgs &mouseArgs = (const CEGUI::MouseEventArgs &)e;

		// Get the cegui window
		CEGUI::Window * pWindow = mouseArgs.window;

		// Get the appropriate parent
		if( pWindow->testClassName( CEGUI::Titlebar::EventNamespace ) || pWindow->testClassName( CEGUI::Scrollbar::EventNamespace ) )
			pWindow = pWindow->getParent();

		// Copy the variables
		CGUIMouseEventArgs args;
		args.button = (CGUIMouse::MouseButton)mouseArgs.button;
		args.position = Vector2( mouseArgs.position.d_x, mouseArgs.position.d_y );
		args.moveDelta = Vector2( mouseArgs.moveDelta.d_x, mouseArgs.moveDelta.d_y );
		args.sysKeys = mouseArgs.sysKeys;
		args.wheelChange = mouseArgs.wheelChange;
		args.pWindow = (CGUIElement_Impl *)pWindow->getUserData();

		// Call the mouse click handler
		m_MouseButtonDownHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_MouseButtonUp( const CEGUI::EventArgs &e )
{
	// Do we have a mouse button up handler?
	if( m_MouseButtonUpHandler )
	{
		// Cast the event to a mouse event
		const CEGUI::MouseEventArgs &mouseArgs = (const CEGUI::MouseEventArgs &)e;

		// Get the cegui window
		CEGUI::Window * pWindow = mouseArgs.window;

		// Get the appropriate parent
		if( pWindow->testClassName( CEGUI::Titlebar::EventNamespace ) || pWindow->testClassName( CEGUI::Scrollbar::EventNamespace ) )
			pWindow = pWindow->getParent();

		// Copy the variables
		CGUIMouseEventArgs args;
		args.button = (CGUIMouse::MouseButton)mouseArgs.button;
		args.position = Vector2( mouseArgs.position.d_x, mouseArgs.position.d_y );
		args.moveDelta = Vector2( mouseArgs.moveDelta.d_x, mouseArgs.moveDelta.d_y );
		args.sysKeys = mouseArgs.sysKeys;
		args.wheelChange = mouseArgs.wheelChange;
		args.pWindow = (CGUIElement_Impl *)pWindow->getUserData();

		// Call the mouse click handler
		m_MouseButtonUpHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_MouseWheel( const CEGUI::EventArgs &e )
{
	// Do we have a mouse wheel handler?
	if( m_MouseWheelHandler )
	{
		// Cast the event to a mouse event
		const CEGUI::MouseEventArgs &mouseArgs = (const CEGUI::MouseEventArgs &)e;

		// Get the cegui window
		CEGUI::Window * pWindow = mouseArgs.window;

		// Get the appropriate parent
		if( pWindow->testClassName( CEGUI::Titlebar::EventNamespace ) || pWindow->testClassName( CEGUI::Scrollbar::EventNamespace ) )
			pWindow = pWindow->getParent();

		// Copy the variables
		CGUIMouseEventArgs args;
		args.button = (CGUIMouse::MouseButton)mouseArgs.button;
		args.position = Vector2( mouseArgs.position.d_x, mouseArgs.position.d_y );
		args.moveDelta = Vector2( mouseArgs.moveDelta.d_x, mouseArgs.moveDelta.d_y );
		args.sysKeys = mouseArgs.sysKeys;
		args.wheelChange = mouseArgs.wheelChange;
		args.pWindow = (CGUIElement_Impl *)pWindow->getUserData();

		// Call the mouse click handler
		m_MouseWheelHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_MouseMove( const CEGUI::EventArgs &e )
{
	// Do we have a mouse move handler?
	if( m_MouseMoveHandler )
	{
		// Cast the event to a mouse event
		const CEGUI::MouseEventArgs &mouseArgs = (const CEGUI::MouseEventArgs &)e;

		// Get the cegui window
		CEGUI::Window * pWindow = mouseArgs.window;

		// Get the appropriate parent
		if( pWindow->testClassName( CEGUI::Titlebar::EventNamespace ) || pWindow->testClassName( CEGUI::Scrollbar::EventNamespace ) )
			pWindow = pWindow->getParent();

		// Copy the variables
		CGUIMouseEventArgs args;
		args.button = (CGUIMouse::MouseButton)mouseArgs.button;
		args.position = Vector2( mouseArgs.position.d_x, mouseArgs.position.d_y );
		args.moveDelta = Vector2( mouseArgs.moveDelta.d_x, mouseArgs.moveDelta.d_y );
		args.sysKeys = mouseArgs.sysKeys;
		args.wheelChange = mouseArgs.wheelChange;
		args.pWindow = (CGUIElement_Impl *)pWindow->getUserData();

		// Call the mouse click handler
		m_MouseMoveHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_MouseEnter( const CEGUI::EventArgs &e )
{
	// Do we have a mouse enter handler?
	if( m_MouseEnterHandler )
	{
		// Cast the event to a mouse event
		const CEGUI::MouseEventArgs &mouseArgs = (const CEGUI::MouseEventArgs &)e;

		// Get the cegui window
		CEGUI::Window * pWindow = mouseArgs.window;

		// Get the appropriate parent
		if( pWindow->testClassName( CEGUI::Titlebar::EventNamespace ) || pWindow->testClassName( CEGUI::Scrollbar::EventNamespace ) )
			pWindow = pWindow->getParent();

		// Copy the variables
		CGUIMouseEventArgs args;
		args.button = (CGUIMouse::MouseButton)mouseArgs.button;
		args.position = Vector2( mouseArgs.position.d_x, mouseArgs.position.d_y );
		args.moveDelta = Vector2( mouseArgs.moveDelta.d_x, mouseArgs.moveDelta.d_y );
		args.sysKeys = mouseArgs.sysKeys;
		args.wheelChange = mouseArgs.wheelChange;
		args.pWindow = (CGUIElement_Impl *)pWindow->getUserData();

		// Call the mouse click handler
		m_MouseEnterHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_MouseLeave( const CEGUI::EventArgs &e )
{
	// Do we have a mouse leave handler?
	if( m_MouseLeaveHandler )
	{
		// Cast the event to a mouse event
		const CEGUI::MouseEventArgs &mouseArgs = (const CEGUI::MouseEventArgs &)e;

		// Get the cegui window
		CEGUI::Window * pWindow = mouseArgs.window;

		// Get the appropriate parent
		if( pWindow->testClassName( CEGUI::Titlebar::EventNamespace ) || pWindow->testClassName( CEGUI::Scrollbar::EventNamespace ) )
			pWindow = pWindow->getParent();

		// Copy the variables
		CGUIMouseEventArgs args;
		args.button = (CGUIMouse::MouseButton)mouseArgs.button;
		args.position = Vector2( mouseArgs.position.d_x, mouseArgs.position.d_y );
		args.moveDelta = Vector2( mouseArgs.moveDelta.d_x, mouseArgs.moveDelta.d_y );
		args.sysKeys = mouseArgs.sysKeys;
		args.wheelChange = mouseArgs.wheelChange;
		args.pWindow = (CGUIElement_Impl *)pWindow->getUserData();

		// Call the mouse click handler
		m_MouseLeaveHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_FocusGained( const CEGUI::EventArgs &e )
{
	// Do we have a focus gained handler?
	if( m_FocusGainedHandler )
	{
		// Cast the event to a focus event
		const CEGUI::ActivationEventArgs &activationArgs = (const CEGUI::ActivationEventArgs &)e;

		// Copy the variables
		CGUIFocusEventArgs args;
		args.pDeactivatedWindow = (CGUIElement_Impl *)activationArgs.window->getUserData();
		args.pActivatedWindow = NULL;

		// Get the activated window
		if( activationArgs.otherWindow )
			args.pActivatedWindow = (CGUIElement_Impl *)activationArgs.otherWindow->getUserData();

		// Call the focus gained handler
		m_FocusGainedHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_FocusLost( const CEGUI::EventArgs &e )
{
	// Do we have a focus lost handler?
	if( m_FocusGainedHandler )
	{
		// Cast the event to a focus event
		const CEGUI::ActivationEventArgs &activationArgs = (const CEGUI::ActivationEventArgs &)e;

		// Copy the variables
		CGUIFocusEventArgs args;
		args.pDeactivatedWindow = (CGUIElement_Impl *)activationArgs.window->getUserData();
		args.pActivatedWindow = NULL;

		// Get the activated window
		if( activationArgs.otherWindow )
			args.pActivatedWindow = (CGUIElement_Impl *)activationArgs.otherWindow->getUserData();

		// Call the focus lost handler
		m_FocusLostHandler( args );
	}

	return true;
}

std::shared_ptr<CGUIWindow_Impl> CGUI_Impl::CreateWnd( String strCaption, CGUIElement_Impl * pParent )
{
	return std::make_shared<CGUIWindow_Impl>( this, strCaption, pParent );
}

std::shared_ptr<CGUIFont_Impl> CGUI_Impl::CreateFnt( String strName, String strFile, unsigned int uiSize, unsigned int uFlags, bool bAutoScale )
{
	return std::make_shared<CGUIFont_Impl>( this, strName, strFile, uiSize, uFlags, bAutoScale );
}

std::shared_ptr<CGUIProgressBar_Impl> CGUI_Impl::CreateProgressBar( CGUIElement_Impl * pParent )
{
	return std::make_shared<CGUIProgressBar_Impl>( this, pParent );
}

std::shared_ptr<CGUIStaticImage_Impl> CGUI_Impl::CreateStaticImage( CGUIElement_Impl * pParent )
{
	return std::make_shared<CGUIStaticImage_Impl>( this, pParent );
}

std::shared_ptr<CGUIButton_Impl> CGUI_Impl::CreateButton( String strCaption, CGUIElement_Impl * pParent )
{
	return std::make_shared<CGUIButton_Impl>( this, strCaption, pParent );
}

std::shared_ptr<CGUICheckBox_Impl> CGUI_Impl::CreateCheckBox( String strCaption, CGUIElement_Impl * pParent )
{
	return std::make_shared<CGUICheckBox_Impl>( this, strCaption, pParent );
}

std::shared_ptr<CGUIComboBox_Impl> CGUI_Impl::CreateComboBox( const char * szCaption, CGUIElement_Impl * pParent )
{
	return std::make_shared<CGUIComboBox_Impl>( this, szCaption, pParent );
}

std::shared_ptr<CGUIEdit_Impl> CGUI_Impl::CreateEdit( String strText, CGUIElement_Impl * pParent )
{
	return std::make_shared<CGUIEdit_Impl>( this, strText, pParent );
}

std::shared_ptr<CGUILabel_Impl> CGUI_Impl::CreateLabel( String strText, CGUIFont_Impl * pFont, CGUIElement_Impl * pParent )
{
	return std::make_shared<CGUILabel_Impl>( this, strText, pFont, pParent );
}

std::shared_ptr<CGUIRadioButton_Impl> CGUI_Impl::CreateRadioButton( String strCaption, CGUIElement_Impl * pParent )
{
	return std::make_shared<CGUIRadioButton_Impl>( this, strCaption, pParent );
}

std::shared_ptr<CGUITabPanel_Impl> CGUI_Impl::CreateTabPanel( CGUIElement_Impl * pParent )
{
	return std::make_shared<CGUITabPanel_Impl>( this, pParent );
}

std::shared_ptr<CGUIGridList_Impl> CGUI_Impl::CreateGridList( CGUIElement_Impl * pParent )
{
	return std::make_shared<CGUIGridList_Impl>( this, pParent );
}

std::shared_ptr<CGUIScrollBar_Impl> CGUI_Impl::CreateScrollBar( bool bHorizontal, CGUIElement_Impl * pParent )
{
	return std::make_shared<CGUIScrollBar_Impl>( this, bHorizontal, pParent );
}

std::shared_ptr<CGUIScrollPane_Impl> CGUI_Impl::CreateScrollPane( CGUIElement_Impl * pParent )
{
	return std::make_shared<CGUIScrollPane_Impl>( this, pParent );
}

std::shared_ptr<CGUIMessageBox_Impl> CGUI_Impl::CreateMessageBox( const char * szTitle, const char * szCaption, const char * szButton1, const char * szButton2 )
{
	return std::make_shared<CGUIMessageBox_Impl>( this, szTitle, szCaption, szButton1, szButton2 );
}
