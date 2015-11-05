/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIComboBox_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"../StdInc.h"

CGUIComboBox_Impl::CGUIComboBox_Impl( CGUI_Impl * pGUI, const char * szCaption, CGUIElement_Impl * pParent )
{
	// Store the manager instance
	m_pManager = pGUI;

	// Get a unique name for cegui
	String strName = pGUI->GetUniqueName();

	// Create the window and set default settings
	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/Combobox", strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	SetText( szCaption );
	m_pWindow->setSize( CEGUI::Absolute, CEGUI::Size( 128.0f, 24.0f ) );
	m_pWindow->setVisible( true );

	// Store the pointer to this element
	m_pWindow->setUserData( (void *)this );

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

CGUIComboBox_Impl::~CGUIComboBox_Impl( void )
{
	// Clear the list items
	Clear();
	
	// Destroy the element
	DestroyElement();
}

CGUIListItem_Impl * CGUIComboBox_Impl::AddItem( const char * szText )
{
	// Create the item
	CGUIListItem_Impl * pItem = new CGUIListItem_Impl( szText, CGUIListItem_Impl::Type::TextItem, NULL );

	// Get the CEGUI list box item
	CEGUI::ListboxItem * pListBoxItem = pItem->GetListItem();

	// Add the list item to this
	((CEGUI::Combobox *)m_pWindow)->addItem( pListBoxItem );

	// Add the list box item to the map
	m_Items[ pListBoxItem ] = pItem;

	return pItem;
}

CGUIListItem_Impl * CGUIComboBox_Impl::GetSelectedItem( void )
{
	return GetListItem( ((CEGUI::Combobox *)m_pWindow)->getSelectedItem() );
}

void CGUIComboBox_Impl::Clear( void )
{
	// Reset the cegui list
	((CEGUI::Combobox *)m_pWindow)->getDropList()->resetList();

	// Delete all items in the map
	for( std::map< CEGUI::ListboxItem*, CGUIListItem_Impl* >::iterator iter = m_Items.begin(); iter != m_Items.end(); iter++ )
		delete iter->second;

	// Clear the map
	m_Items.clear();
}

void CGUIComboBox_Impl::SetReadOnly( bool bReadOnly )
{
	((CEGUI::Combobox *)m_pWindow)->setReadOnly( bReadOnly );
}

CGUIListItem_Impl * CGUIComboBox_Impl::GetListItem( CEGUI::ListboxItem * pItem )
{
	// Try and find the item in our map
	std::map< CEGUI::ListboxItem*, CGUIListItem_Impl* >::iterator iter = m_Items.find( pItem );

	// Was the item not found?
	if( iter == m_Items.end() )
		return NULL;

	return iter->second;
}