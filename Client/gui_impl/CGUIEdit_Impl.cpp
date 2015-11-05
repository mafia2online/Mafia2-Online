/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIEdit_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"../StdInc.h"

extern	CCore			* pCore;

CGUIEdit_Impl::CGUIEdit_Impl( CGUI_Impl * pGUI, String strText, CGUIElement_Impl * pParent )
{
	// Store the manager instance
	m_pManager = pGUI;

	// Get a unique name for cegui
	String strName = pGUI->GetUniqueName();

	// Create the window and set default settings
	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/Editbox", strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	m_pWindow->setVisible( true );

	// Store the pointer to this element
	m_pWindow->setUserData( (void *)this );

	// Set default variables
	SetSize( Vector2( 128, 24 ) );

	// Set default text
	SetText( strText );

	// Register our events
	m_pWindow->subscribeEvent( CEGUI::Editbox::EventKeyDown, CEGUI::Event::Subscriber( &CGUIEdit_Impl::Event_OnKeyDown, this ) );
	m_pWindow->subscribeEvent( CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber( &CGUIEdit_Impl::Event_OnTextChanged, this ) );
	AddEvents();

	// If a parent is set, add it to it's childs list, if not add it as a child to the gui manager
	if( pParent )
	{
		SetParent( pParent );
		// tab lists
	}
	else
	{
		pGUI->AddChild( this );
		SetParent( NULL );
	}
}

CGUIEdit_Impl::~CGUIEdit_Impl( void )
{
	// Destroy the element
	DestroyElement();
}

void CGUIEdit_Impl::SetReadOnly( bool bReadOnly )
{
	((CEGUI::Editbox *)m_pWindow)->setReadOnly( bReadOnly );
}

bool CGUIEdit_Impl::IsReadOnly( void )
{
	return ((CEGUI::Editbox *)m_pWindow)->isReadOnly();
}

void CGUIEdit_Impl::SetMasked( bool bMasked )
{
	((CEGUI::Editbox *)m_pWindow)->setTextMasked( bMasked );
}

bool CGUIEdit_Impl::IsMasked( void )
{
	return ((CEGUI::Editbox *)m_pWindow)->isTextMasked();
}

void CGUIEdit_Impl::SetMaxLength( unsigned int uiMaxLength )
{
	((CEGUI::Editbox *)m_pWindow)->setMaxTextLength( uiMaxLength );
}

unsigned int CGUIEdit_Impl::GetMaxLength( void )
{
	return ((CEGUI::Editbox *)m_pWindow)->getMaxTextLength();
}

void CGUIEdit_Impl::SetSelection( unsigned int uiStart, unsigned int uiEnd )
{
	((CEGUI::Editbox *)m_pWindow)->setSelection( uiStart, uiEnd );
}

unsigned int CGUIEdit_Impl::GetSelectionStart( void )
{
	return (unsigned int)((CEGUI::Editbox *)m_pWindow)->getSelectionStartIndex();
}

unsigned int CGUIEdit_Impl::GetSelectionEnd( void )
{
	return (unsigned int)((CEGUI::Editbox *)m_pWindow)->getSelectionEndIndex();
}

unsigned int CGUIEdit_Impl::GetSelectionLength( void )
{
	return (unsigned int)((CEGUI::Editbox *)m_pWindow)->getSelectionLength();
}

bool CGUIEdit_Impl::ActivateOnTab( void )
{
	// Only set this active if it's visible and writable
	if( IsVisible() && !IsReadOnly() )
	{
		// Activate the input
		Activate();

		// Set the carat index
		((CEGUI::Editbox *)GetWindow())->setCaratIndex( GetText().GetLength() );

		return true;
	}

	return false;
}

bool CGUIEdit_Impl::Event_OnTextChanged( const CEGUI::EventArgs &e )
{
	// Call the text changed handler
	if( m_pfnOnTextChanged )
		m_pfnOnTextChanged( this );

	// Pass the event to the client gui manager
	if( pCore->GetClientScriptingManager() && pCore->GetClientScriptingManager()->GetScriptGUIManager() )
		pCore->GetClientScriptingManager()->GetScriptGUIManager()->HandleEvent( "onGuiElementTextChange", this );

	return true;
}

bool CGUIEdit_Impl::Event_OnKeyDown( const CEGUI::EventArgs &e )
{
	const CEGUI::KeyEventArgs &keyEvent = (const CEGUI::KeyEventArgs &)e;

	// Was this a tab key?
	if( keyEvent.scancode == CEGUI::Key::Tab )
	{
		// Switch to the next element
		//if( m_pParent == NULL )
		//	m_pManager->SelectNext();
	}
	else if( keyEvent.scancode == CEGUI::Key::Return || keyEvent.scancode == CEGUI::Key::NumpadEnter )
	{
		// Call the text accepted handler
		if( m_pfnOnTextAccepted )
			m_pfnOnTextAccepted( this );

		// Pass the event to the client gui manager
		if( pCore->GetClientScriptingManager() && pCore->GetClientScriptingManager()->GetScriptGUIManager() )
			pCore->GetClientScriptingManager()->GetScriptGUIManager()->HandleEvent( "onGuiElementTextAccept", this );
	}

	return true;
}