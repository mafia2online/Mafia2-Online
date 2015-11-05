/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIRadioButton_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"../StdInc.h"

CGUIRadioButton_Impl::CGUIRadioButton_Impl( CGUI_Impl * pGUI, String strCaption, CGUIElement_Impl * pParent )
{
	// Store the manager instance
	m_pManager = pGUI;

	// Get a unique name for cegui
	String strName = pGUI->GetUniqueName();

	// Create the window and set default settings
	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/RadioButton", strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	SetText( strCaption );
	m_pWindow->setVisible( true );

	// Store the pointer to this element
	m_pWindow->setUserData( (void *)this );

	// Set default variables
	SetSize( Vector2( 128, 16 ) );
	SetChecked( false );

	// Register our events
	AddEvents();

	// If a parent is set, add it to it's childs list, if not add it as a child to the gui manager
	if( pParent )
		SetParent( pParent );
	else
	{
		pGUI->AddChild( this );
		SetParent( NULL );
	}
}

CGUIRadioButton_Impl::~CGUIRadioButton_Impl( void )
{
	// Destroy the element
	DestroyElement();
}

void CGUIRadioButton_Impl::SetChecked( bool bChecked )
{
	((CEGUI::RadioButton *)m_pWindow)->setSelected( bChecked );
}

bool CGUIRadioButton_Impl::IsChecked( void )
{
	return ((CEGUI::RadioButton *)m_pWindow)->isSelected();
}