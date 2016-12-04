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
	SetCurrentDirectory( CCore::Instance()->GetModDirectory().Get() );

	m_pDevice = pDevice;
	m_pRenderer = std::make_unique<CEGUI::DirectX9Renderer>( pDevice, 0 );
	m_pSystem = std::make_unique<CEGUI::System>( m_pRenderer.get() );
	m_pFontManager = CEGUI::FontManager::getSingletonPtr();
	m_pImageSetManager = CEGUI::ImagesetManager::getSingletonPtr();
	m_pSchemeManager = CEGUI::SchemeManager::getSingletonPtr();
	m_pWindowManager = CEGUI::WindowManager::getSingletonPtr();

#ifdef _DEBUG
	CEGUI::Logger::getSingleton().setLoggingLevel( CEGUI::Informative );
#else
	CEGUI::Logger::getSingleton().setLoggingLevel( CEGUI::Standard );
#endif
	CEGUI::Logger::getSingleton().setLogFilename( "logs\\gui.log" );

	SetCurrentDirAsGui();

	try
	{
		m_pDefaultFont = CreateFnt( "tahoma", "fonts\\tahoma.ttf" );
		m_pDefaultFontBold = CreateFnt( "tahoma-bold", "fonts\\tahoma-bold.ttf" );
	}
	catch ( CEGUI::InvalidRequestException e )
	{
		CLogFile::Printf ( "Failed to load CEGUI fonts!" );
		TerminateProcess ( GetCurrentProcess(), 0 );
	}

	CEGUI::SchemeManager::getSingleton().loadScheme( "skins\\default.xml" );
	CEGUI::System::getSingleton().setDefaultMouseCursor( "CGUI-Images", "MouseArrow" );

	m_pDefaultWindow = (CEGUI::DefaultWindow *)m_pWindowManager->createWindow( "DefaultWindow", "guiroot" );

	// Set the available chars (english and cyrillic support)
	CEGUI::String tmp;
	for (CEGUI::utf32 cp = 0x20; cp <= 0x7E; ++cp)
		tmp += cp;
	for (CEGUI::utf32 cp = 0x400; cp <= 0x4FF; ++cp)
		tmp += cp;
	m_pDefaultFont->GetFont()->defineFontGlyphs(tmp);

	m_pSystem->setGUISheet( m_pDefaultWindow );
	m_pSystem->setSingleClickTimeout( 100000000.0f );
	m_pSystem->setDefaultFont( m_pDefaultFont->GetFont() );
	m_pCursor = CEGUI::MouseCursor::getSingletonPtr();
	m_pCursor->hide();

	CEGUI::GlobalEventSet * pEvents = CEGUI::GlobalEventSet::getSingletonPtr();
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventRedrawRequested, CEGUI::Event::Subscriber( &CGUI_Impl::Event_RedrawRequest, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventActivated, CEGUI::Event::Subscriber( &CGUI_Impl::Event_FocusGained, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventDeactivated, CEGUI::Event::Subscriber( &CGUI_Impl::Event_FocusLost, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventKeyDown, CEGUI::Event::Subscriber( &CGUI_Impl::Event_OnKeyDown, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventCharacterKey, CEGUI::Event::Subscriber( &CGUI_Impl::Event_CharacterKey, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber( &CGUI_Impl::Event_MouseClick, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventMouseDoubleClick, CEGUI::Event::Subscriber( &CGUI_Impl::Event_MouseDoubleClick, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber( &CGUI_Impl::Event_MouseButtonDown, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber( &CGUI_Impl::Event_MouseButtonUp, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventMouseWheel, CEGUI::Event::Subscriber( &CGUI_Impl::Event_MouseWheel, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventMouseMove, CEGUI::Event::Subscriber( &CGUI_Impl::Event_MouseMove, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber( &CGUI_Impl::Event_MouseEnter, this ) );
	pEvents->subscribeEvent( "Window/" + CEGUI::Window::EventMouseLeaves, CEGUI::Event::Subscriber( &CGUI_Impl::Event_MouseLeave, this ) );
	SetCurrentDirectory( CCore::Instance()->GetModDirectory().Get() );
}

CGUI_Impl::~CGUI_Impl( void )
{
#ifdef _DEBUG
	assert(m_elementsList.empty());
#endif
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
	std::lock_guard<std::mutex> lock(m_redrawQueueMutex);
	if( !m_redrawQueue.empty() )
	{
		std::list< CGUIElement_Impl* >::const_iterator iter = m_redrawQueue.begin();
		for( ; iter != m_redrawQueue.end(); iter++ )
		{
			(*iter)->ForceRedraw();
		}

		m_redrawQueue.clear();
	}

	m_pSystem->renderGUI();
}

void CGUI_Impl::DrawMouseCursor( void )
{
	CEGUI::MouseCursor::getSingleton().draw();
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

	SetCurrentDirectory( CCore::Instance()->GetModDirectory().Get() );
}

void CGUI_Impl::ProcessMouseInput( eMouseType type, float fX, float fY, int iButton )
{
	switch( type )
	{
	case eMouseType::MOVE:
		m_pSystem->injectMouseMove( fX, fY );
		break;

	case eMouseType::POSITION:
		m_pSystem->injectMousePosition( fX, fY );
		break;

	case eMouseType::SCROLL:
		m_pSystem->injectMouseWheelChange( (int)fX );
		break;

	case eMouseType::BUTTON_DOWN:
		m_pSystem->injectMouseButtonDown( (CEGUI::MouseButton)iButton );
		break;

	case eMouseType::BUTTON_UP:
		m_pSystem->injectMouseButtonUp( (CEGUI::MouseButton)iButton );
		break;
	}
}

bool CGUI_Impl::ProcessKeyboardInput( unsigned long ulKey, bool bIsDown )
{
	bool bReturn = false;

	if( bIsDown )
		bReturn = m_pSystem->injectKeyDown( ulKey );
	else
		bReturn = m_pSystem->injectKeyUp( ulKey );

	return bReturn;
}

bool CGUI_Impl::ProcessCharacter( unsigned long ulCharacter )
{
	return m_pSystem->injectChar( ulCharacter );
}

void CGUI_Impl::CleanDeadPool( void )
{
	if( m_pWindowManager )
		m_pWindowManager->cleanDeadPool();
}

void CGUI_Impl::SetCursorEnabled( bool bCursorEnabled )
{
	if( bCursorEnabled )
		CEGUI::MouseCursor::getSingleton().show();
	else
		CEGUI::MouseCursor::getSingleton().hide();
}

bool CGUI_Impl::IsCursorEnabled( void )
{
	return CEGUI::MouseCursor::getSingleton().isVisible();
}

bool CGUI_Impl::IsInputEnabled( void )
{
	CEGUI::Window * pWindow = m_pDefaultWindow->getActiveChild();

	if( !pWindow || pWindow == m_pDefaultWindow || !pWindow->isVisible() )
		return false;

	if( pWindow->getType() == "CGUI/Editbox" )
	{
		CEGUI::Editbox * pEditBox = reinterpret_cast<CEGUI::Editbox *>(pWindow);
		return ( !pEditBox->isReadOnly() && pEditBox->hasInputFocus() );
	}
	else if( pWindow->getType() == "CGUI/MultiLineEditbox" )
	{
		CEGUI::MultiLineEditbox * pMultiLineEditBox = reinterpret_cast<CEGUI::MultiLineEditbox *>(pWindow);
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
	CGUIElement_Impl * pElement = reinterpret_cast<CGUIElement_Impl *>(arg.window->getUserData());

	if( pElement )
		AddToRedrawQueue( pElement );
	else
		arg.window->forceRedraw();

	return true;
}

bool CGUI_Impl::Event_OnKeyDown( const CEGUI::EventArgs &e )
{
	const CEGUI::KeyEventArgs &keyArgs = reinterpret_cast<const CEGUI::KeyEventArgs &>(e);
	switch( keyArgs.scancode )
	{
	case CEGUI::Key::Scan::C:
	case CEGUI::Key::Scan::X:
		{
			if( keyArgs.sysKeys & CEGUI::Control )
			{
				CEGUI::String strTemp;
				CEGUI::Window * pWindow = reinterpret_cast<CEGUI::Window *>(keyArgs.window);
				if( pWindow->getType() == "CGUI/Editbox" )
				{
					CEGUI::Editbox * pEditBox = reinterpret_cast<CEGUI::Editbox *>(pWindow);
					if (!pEditBox->isTextMasked())
					{
						size_t sSelectionStart = pEditBox->getSelectionStartIndex();
						size_t sSelectionEnd = pEditBox->getSelectionLength();
						strTemp = pEditBox->getText().substr(sSelectionStart, sSelectionEnd);

						if (keyArgs.scancode == CEGUI::Key::Scan::X)
						{
							if (!pEditBox->isReadOnly())
							{
								CEGUI::String strTemp2 = pEditBox->getText();
								strTemp2.replace(sSelectionStart, sSelectionEnd, "", 0);
								pEditBox->setText(strTemp2);
							}
						}
					}
				}
				else if( pWindow->getType() == "CGUI/MultiLineEditBox" )
				{
					CEGUI::MultiLineEditbox * pEditBox = reinterpret_cast<CEGUI::MultiLineEditbox *>(pWindow);
					size_t sSelectionStart = pEditBox->getSelectionStartIndex();
					size_t sSelectionEnd = pEditBox->getSelectionLength();
					strTemp = pEditBox->getText().substr( sSelectionStart, sSelectionEnd );

					if( keyArgs.scancode == CEGUI::Key::Scan::X )
					{
						if( !pEditBox->isReadOnly() )
						{
							CEGUI::String strTemp2 = pEditBox->getText();
							strTemp2.replace( sSelectionStart, sSelectionEnd, "", 0 );
							pEditBox->setText( strTemp2 );
						}
					}
				}

				if( strTemp.length() > 0 )
				{
					SharedUtility::SetClipboardText( strTemp.c_str(), strTemp.length() );
				}
			}
			break;
		}

	case CEGUI::Key::Scan::V:
		{
			if( keyArgs.sysKeys & CEGUI::Control )
			{
				CEGUI::Window * pWindow = reinterpret_cast<CEGUI::Window *>(keyArgs.window);
				if( pWindow->getType() == "CGUI/Editbox" )
				{
					CEGUI::Editbox * pEditBox = reinterpret_cast<CEGUI::Editbox *>(pWindow);
					const char * szClipboard = SharedUtility::GetClipboardText();

					if( szClipboard )
					{
						std::string strClipboard( szClipboard );
						size_t sNewlineIndex;
						do
						{
							sNewlineIndex = strClipboard.find( '\n' );

							if( sNewlineIndex != std::string::npos )
							{
								if( sNewlineIndex > 0 && strClipboard[ sNewlineIndex - 1 ] == '\r' )
								{
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

						CEGUI::String strTemp = pEditBox->getText();

						if( (strClipboard.length() + strTemp.length() ) < pEditBox->getMaxTextLength() )
						{
							size_t sCaratIndex = 0;
							if( pEditBox->getSelectionLength() > 0 )
							{
								strTemp.replace( pEditBox->getSelectionStartIndex(), pEditBox->getSelectionLength(), strClipboard.c_str(), strClipboard.length() );
								sCaratIndex = pEditBox->getSelectionStartIndex() + strClipboard.length();
							}
							else
							{
								strTemp.insert( pEditBox->getSelectionStartIndex(), strClipboard.c_str(), strClipboard.length() );
								sCaratIndex = pEditBox->getCaratIndex() + strClipboard.length();
							}

							pEditBox->setText( strTemp );
							pEditBox->setCaratIndex( sCaratIndex );
						}
						else
						{
							pEditBox->fireEvent( CEGUI::Editbox::EventEditboxFull, CEGUI::WindowEventArgs( pEditBox ) );
						}
					}
				}
			}
			break;
		}

	case CEGUI::Key::Scan::A:
		{
			if( keyArgs.sysKeys & CEGUI::Control )
			{
				CEGUI::Window * pWindow = reinterpret_cast<CEGUI::Window *>(keyArgs.window);

				if( pWindow->getType() == "CGUI/Editbox" )
				{
					CEGUI::Editbox * pEditBox = reinterpret_cast<CEGUI::Editbox *>(pWindow);
					pEditBox->setSelection( 0, pEditBox->getText().size() );
				}
				else if( pWindow->getType() == "CGUI/MultiLineEditBox" )
				{
					CEGUI::MultiLineEditbox * pMultiLineEditBox = reinterpret_cast<CEGUI::MultiLineEditbox *>(pWindow);
					pMultiLineEditBox->setSelection( 0, pMultiLineEditBox->getText().size() );
				}
			}
			break;
		}
	}
	if( m_KeyDownHandler )
	{
		CGUIKeyEventArgs args;
		args.codepoint = keyArgs.codepoint;
		args.scancode = static_cast<CGUIKeys::Scan>(keyArgs.scancode);
		args.sysKeys = keyArgs.sysKeys;
		args.pWindow = reinterpret_cast<CGUIElement_Impl *>(keyArgs.window->getUserData());

		m_KeyDownHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_CharacterKey( const CEGUI::EventArgs &e )
{
	if( m_CharacterKeyHandler )
	{
		const CEGUI::KeyEventArgs &keyArgs = (const CEGUI::KeyEventArgs &)e;

		CGUIKeyEventArgs args;
		args.codepoint = keyArgs.codepoint;
		args.scancode = static_cast<CGUIKeys::Scan>(keyArgs.scancode);
		args.sysKeys = keyArgs.sysKeys;
		args.pWindow = reinterpret_cast<CGUIElement_Impl *>(keyArgs.window->getUserData());

		m_CharacterKeyHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_MouseClick( const CEGUI::EventArgs &e )
{
	if( m_MouseClickHandler )
	{
		const CEGUI::MouseEventArgs &mouseArgs = reinterpret_cast<const CEGUI::MouseEventArgs &>(e);
		CEGUI::Window * pWindow = mouseArgs.window;

		if( pWindow->testClassName( CEGUI::Titlebar::EventNamespace ) || pWindow->testClassName( CEGUI::Scrollbar::EventNamespace ) )
			pWindow = pWindow->getParent();

		CGUIMouseEventArgs args;
		args.button = static_cast<CGUIMouse::MouseButton>(mouseArgs.button);
		args.position = Vector2( mouseArgs.position.d_x, mouseArgs.position.d_y );
		args.moveDelta = Vector2( mouseArgs.moveDelta.d_x, mouseArgs.moveDelta.d_y );
		args.sysKeys = mouseArgs.sysKeys;
		args.wheelChange = mouseArgs.wheelChange;
		args.pWindow = reinterpret_cast<CGUIElement_Impl *>(pWindow->getUserData());

		m_MouseClickHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_MouseDoubleClick( const CEGUI::EventArgs &e )
{
	if( m_MouseDoubleClickHandler )
	{
		const CEGUI::MouseEventArgs &mouseArgs = reinterpret_cast<const CEGUI::MouseEventArgs &>(e);

		CEGUI::Window * pWindow = mouseArgs.window;

		if( pWindow->testClassName( CEGUI::Titlebar::EventNamespace ) || pWindow->testClassName( CEGUI::Scrollbar::EventNamespace ) )
			pWindow = pWindow->getParent();

		CGUIMouseEventArgs args;
		args.button = static_cast<CGUIMouse::MouseButton>(mouseArgs.button);
		args.position = Vector2( mouseArgs.position.d_x, mouseArgs.position.d_y );
		args.moveDelta = Vector2( mouseArgs.moveDelta.d_x, mouseArgs.moveDelta.d_y );
		args.sysKeys = mouseArgs.sysKeys;
		args.wheelChange = mouseArgs.wheelChange;
		args.pWindow = reinterpret_cast<CGUIElement_Impl *>(pWindow->getUserData());

		m_MouseDoubleClickHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_MouseButtonDown( const CEGUI::EventArgs &e )
{
	if( m_MouseButtonDownHandler )
	{
		const CEGUI::MouseEventArgs &mouseArgs = reinterpret_cast<const CEGUI::MouseEventArgs &>(e);
		CEGUI::Window * pWindow = mouseArgs.window;

		if( pWindow->testClassName( CEGUI::Titlebar::EventNamespace ) || pWindow->testClassName( CEGUI::Scrollbar::EventNamespace ) )
			pWindow = pWindow->getParent();

		CGUIMouseEventArgs args;
		args.button = static_cast<CGUIMouse::MouseButton>(mouseArgs.button);
		args.position = Vector2( mouseArgs.position.d_x, mouseArgs.position.d_y );
		args.moveDelta = Vector2( mouseArgs.moveDelta.d_x, mouseArgs.moveDelta.d_y );
		args.sysKeys = mouseArgs.sysKeys;
		args.wheelChange = mouseArgs.wheelChange;
		args.pWindow = reinterpret_cast<CGUIElement_Impl *>(pWindow->getUserData());

		m_MouseButtonDownHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_MouseButtonUp( const CEGUI::EventArgs &e )
{
	if( m_MouseButtonUpHandler )
	{
		const CEGUI::MouseEventArgs &mouseArgs = reinterpret_cast<const CEGUI::MouseEventArgs &>(e);
		CEGUI::Window * pWindow = mouseArgs.window;

		if( pWindow->testClassName( CEGUI::Titlebar::EventNamespace ) || pWindow->testClassName( CEGUI::Scrollbar::EventNamespace ) )
			pWindow = pWindow->getParent();

		CGUIMouseEventArgs args;
		args.button = static_cast<CGUIMouse::MouseButton>(mouseArgs.button);
		args.position = Vector2( mouseArgs.position.d_x, mouseArgs.position.d_y );
		args.moveDelta = Vector2( mouseArgs.moveDelta.d_x, mouseArgs.moveDelta.d_y );
		args.sysKeys = mouseArgs.sysKeys;
		args.wheelChange = mouseArgs.wheelChange;
		args.pWindow = reinterpret_cast<CGUIElement_Impl *>(pWindow->getUserData());

		m_MouseButtonUpHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_MouseWheel( const CEGUI::EventArgs &e )
{
	if( m_MouseWheelHandler )
	{
		const CEGUI::MouseEventArgs &mouseArgs = reinterpret_cast<const CEGUI::MouseEventArgs &>(e);

		CEGUI::Window * pWindow = mouseArgs.window;

		if( pWindow->testClassName( CEGUI::Titlebar::EventNamespace ) || pWindow->testClassName( CEGUI::Scrollbar::EventNamespace ) )
			pWindow = pWindow->getParent();

		CGUIMouseEventArgs args;
		args.button = static_cast<CGUIMouse::MouseButton>(mouseArgs.button);
		args.position = Vector2( mouseArgs.position.d_x, mouseArgs.position.d_y );
		args.moveDelta = Vector2( mouseArgs.moveDelta.d_x, mouseArgs.moveDelta.d_y );
		args.sysKeys = mouseArgs.sysKeys;
		args.wheelChange = mouseArgs.wheelChange;
		args.pWindow = reinterpret_cast<CGUIElement_Impl *>(pWindow->getUserData());

		m_MouseWheelHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_MouseMove( const CEGUI::EventArgs &e )
{
	if( m_MouseMoveHandler )
	{
		const CEGUI::MouseEventArgs &mouseArgs = reinterpret_cast<const CEGUI::MouseEventArgs &>(e);

		CEGUI::Window * pWindow = mouseArgs.window;

		if( pWindow->testClassName( CEGUI::Titlebar::EventNamespace ) || pWindow->testClassName( CEGUI::Scrollbar::EventNamespace ) )
			pWindow = pWindow->getParent();

		CGUIMouseEventArgs args;
		args.button = static_cast<CGUIMouse::MouseButton>(mouseArgs.button);
		args.position = Vector2( mouseArgs.position.d_x, mouseArgs.position.d_y );
		args.moveDelta = Vector2( mouseArgs.moveDelta.d_x, mouseArgs.moveDelta.d_y );
		args.sysKeys = mouseArgs.sysKeys;
		args.wheelChange = mouseArgs.wheelChange;
		args.pWindow = reinterpret_cast<CGUIElement_Impl *>(pWindow->getUserData());

		m_MouseMoveHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_MouseEnter( const CEGUI::EventArgs &e )
{
	if( m_MouseEnterHandler )
	{
		const CEGUI::MouseEventArgs &mouseArgs = reinterpret_cast<const CEGUI::MouseEventArgs &>(e);
		CEGUI::Window * pWindow = mouseArgs.window;

		if( pWindow->testClassName( CEGUI::Titlebar::EventNamespace ) || pWindow->testClassName( CEGUI::Scrollbar::EventNamespace ) )
			pWindow = pWindow->getParent();

		CGUIMouseEventArgs args;
		args.button = static_cast<CGUIMouse::MouseButton>(mouseArgs.button);
		args.position = Vector2( mouseArgs.position.d_x, mouseArgs.position.d_y );
		args.moveDelta = Vector2( mouseArgs.moveDelta.d_x, mouseArgs.moveDelta.d_y );
		args.sysKeys = mouseArgs.sysKeys;
		args.wheelChange = mouseArgs.wheelChange;
		args.pWindow = reinterpret_cast<CGUIElement_Impl *>(pWindow->getUserData());

		m_MouseEnterHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_MouseLeave( const CEGUI::EventArgs &e )
{
	if( m_MouseLeaveHandler )
	{
		const CEGUI::MouseEventArgs &mouseArgs = reinterpret_cast<const CEGUI::MouseEventArgs &>(e);

		CEGUI::Window * pWindow = mouseArgs.window;

		if( pWindow->testClassName( CEGUI::Titlebar::EventNamespace ) || pWindow->testClassName( CEGUI::Scrollbar::EventNamespace ) )
			pWindow = pWindow->getParent();

		CGUIMouseEventArgs args;
		args.button = static_cast<CGUIMouse::MouseButton>(mouseArgs.button);
		args.position = Vector2( mouseArgs.position.d_x, mouseArgs.position.d_y );
		args.moveDelta = Vector2( mouseArgs.moveDelta.d_x, mouseArgs.moveDelta.d_y );
		args.sysKeys = mouseArgs.sysKeys;
		args.wheelChange = mouseArgs.wheelChange;
		args.pWindow = reinterpret_cast<CGUIElement_Impl *>(pWindow->getUserData());

		m_MouseLeaveHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_FocusGained( const CEGUI::EventArgs &e )
{
	if( m_FocusGainedHandler )
	{
		const CEGUI::ActivationEventArgs &activationArgs = reinterpret_cast<const CEGUI::ActivationEventArgs &>(e);

		CGUIFocusEventArgs args;
		args.pDeactivatedWindow = reinterpret_cast<CGUIElement_Impl *>(activationArgs.window->getUserData());
		args.pActivatedWindow = NULL;

		if( activationArgs.otherWindow )
			args.pActivatedWindow = reinterpret_cast<CGUIElement_Impl *>(activationArgs.otherWindow->getUserData());

		m_FocusGainedHandler( args );
	}

	return true;
}

bool CGUI_Impl::Event_FocusLost( const CEGUI::EventArgs &e )
{
	if( m_FocusGainedHandler )
	{
		const CEGUI::ActivationEventArgs &activationArgs = reinterpret_cast<const CEGUI::ActivationEventArgs &>(e);

		CGUIFocusEventArgs args;
		args.pDeactivatedWindow = reinterpret_cast<CGUIElement_Impl *>(activationArgs.window->getUserData());
		args.pActivatedWindow = NULL;

		if( activationArgs.otherWindow )
			args.pActivatedWindow = reinterpret_cast<CGUIElement_Impl *>(activationArgs.otherWindow->getUserData());

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
