/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUITab_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"../StdInc.h"

CGUITab_Impl::CGUITab_Impl( CGUI_Impl * pGUI, String strCaption, CGUIElement_Impl * pParent )
{
	// Store the manager instance
	m_pManager = pGUI;

	// Get a unique name for cegui
	String strName = pGUI->GetUniqueName();

	// Create the window and set default settings
	m_pWindow = pGUI->GetWindowManager()->createWindow( "DefaultWindow", strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	SetText( strCaption );

	// Store the pointer to this element
	m_pWindow->setUserData( (void *)this );

	// Register our events
	AddEvents();

	// If a parent is set, add it to it's childs list, if not add it as a child to the gui manager
	if( pParent )
	{
		SetParent( pParent );

		// Adjust the tab button
		((CEGUI::TabControl *)((CGUITabPanel_Impl *)pParent)->GetWindow())->setAbsoluteTabHeight( 24.0f );
	}
	else
	{
		pGUI->AddChild( this );
		SetParent( NULL );
	}
}

CGUITab_Impl::~CGUITab_Impl( void )
{
	// Destroy the element
	DestroyElement();
}

void CGUITab_Impl::SetCaption( String strText )
{
	// Set the tab caption
	m_pWindow->setText( strText.Get() );
}

void CGUITab_Impl::SetVisible( bool bVisible )
{
	// Cast the tab window to a tab control window
	CEGUI::TabControl * pTabControl = reinterpret_cast < CEGUI::TabControl* > ( ((CGUITabPanel_Impl *)m_pParent)->GetWindow() );

	//
	pTabControl->getButtonForTabContents( m_pWindow )->setVisible ( bVisible );
}

bool CGUITab_Impl::IsVisible( void )
{
	// Cast the tab window to a tab control window
	CEGUI::TabControl * pTabControl = reinterpret_cast < CEGUI::TabControl* > ( ((CGUITabPanel_Impl *)m_pParent)->GetWindow() );

	return pTabControl->getButtonForTabContents( m_pWindow )->isVisible ();
}

void CGUITab_Impl::SetEnabled( bool bEnabled )
{
	// Cast the tab window to a tab control window
	CEGUI::TabControl * pTabControl = reinterpret_cast < CEGUI::TabControl* > ( ((CGUITabPanel_Impl *)m_pParent)->GetWindow() );

	//
	pTabControl->getButtonForTabContents( m_pWindow )->setEnabled ( bEnabled );
}

bool CGUITab_Impl::IsEnabled( void )
{
	// Cast the tab window to a tab control window
	CEGUI::TabControl * pTabControl = reinterpret_cast < CEGUI::TabControl* > ( ((CGUITabPanel_Impl *)m_pParent)->GetWindow() );

	return !pTabControl->getButtonForTabContents( m_pWindow )->isDisabled ();
}