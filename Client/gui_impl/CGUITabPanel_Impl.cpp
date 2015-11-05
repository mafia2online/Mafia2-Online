/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUITabPanel_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"../StdInc.h"

extern	CCore				* pCore;

CGUITabPanel_Impl::CGUITabPanel_Impl( CGUI_Impl * pGUI, CGUIElement_Impl * pParent )
{
	// Store the manager instance
	m_pManager = pGUI;

	// Get a unique name for cegui
	String strName = pGUI->GetUniqueName();

	// Create the window and set default settings
	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/TabControl", strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	((CEGUI::TabControl *)m_pWindow)->setAbsoluteTabTextPadding( 10.0f );
	m_pWindow->setVisible( true );

	// Store the pointer to this element
	m_pWindow->setUserData( (void *)this );

	// Register our events
	m_pWindow->subscribeEvent( CEGUI::TabControl::EventSelectionChanged, CEGUI::Event::Subscriber( &CGUITabPanel_Impl::Event_OnSelectionChange, this ) );
	AddEvents();

	// Set default variables
	SetSize( Vector2( 300, 150 ) );

	// If a parent is set, add it to it's childs list, if not add it as a child to the gui manager
	if( pParent )
		SetParent( pParent );
	else
	{
		pGUI->AddChild( this );
		SetParent( NULL );
	}
}

CGUITabPanel_Impl::~CGUITabPanel_Impl( void )
{
	// Destroy the element
	DestroyElement();
}

CGUITab_Impl * CGUITabPanel_Impl::CreateTab( String strCaption )
{
	return new CGUITab_Impl( m_pManager, strCaption, this );
}

void CGUITabPanel_Impl::DeleteTab( CGUITab_Impl * pTab )
{
	// Remove the tab
	((CEGUI::TabControl *)m_pWindow)->removeTab( pTab->GetWindow()->getName() );
}

void CGUITabPanel_Impl::SetSelectedTab( CGUITab_Impl * pTab )
{
	try
	{
		((CEGUI::TabControl *)m_pWindow)->setSelectedTab( pTab->GetWindow()->getName() );
	}
	catch ( CEGUI::Exception ) {}
}

CGUITab_Impl * CGUITabPanel_Impl::GetSelectedTab( void )
{
	// Get the tab control
	CEGUI::TabControl * pTabControl = (CEGUI::TabControl *)m_pWindow;

	// Do we have any tabs?
	if ( pTabControl->getTabCount() > 0 )
	{
		try
		{
			// Get the selected tab's window
			CEGUI::Window * pTabWindow = pTabControl->getTabContentsAtIndex( pTabControl->getSelectedTabIndex() );
			return (CGUITab_Impl *)pTabWindow->getUserData();
		}
		catch ( CEGUI::Exception )
		{
			return NULL;
		}
	}

	return NULL;
}

bool CGUITabPanel_Impl::IsTabSelected( CGUITab_Impl * pTab )
{
	return ((CEGUI::TabControl *)m_pWindow)->isTabContentsSelected( pTab->GetWindow() );
}

bool CGUITabPanel_Impl::Event_OnSelectionChange( const CEGUI::EventArgs &e )
{
	// Call the callback handler (if we have one)
	if( m_pfnHandler )
		m_pfnHandler( (CGUIElement_Impl *)GetSelectedTab() );

	// Pass the event to the client gui manager
	if( pCore->GetClientScriptingManager() && pCore->GetClientScriptingManager()->GetScriptGUIManager() )
		pCore->GetClientScriptingManager()->GetScriptGUIManager()->HandleEvent( "onGuiElementTabChange", GetSelectedTab() );

	return true;
}