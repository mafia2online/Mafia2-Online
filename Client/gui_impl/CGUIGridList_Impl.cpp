/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIGridList_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"../StdInc.h"

extern	CCore				* pCore;

CGUIGridList_Impl::CGUIGridList_Impl( CGUI_Impl * pGUI, CGUIElement_Impl * pParent )
{
	// Store the manager instance
	m_pManager = pGUI;

	// Reset
	m_uiUniqueHandle = 0;
	m_iIndex = 0;

	// Get a unique name for cegui
	String strName = pGUI->GetUniqueName();

	// Create the window and set default settings
	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/MultiColumnList", strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	m_pWindow->setRect( CEGUI::Relative, CEGUI::Rect( 0.00f, 0.00f, 0.40f, 0.40f ) );
	((CEGUI::MultiColumnList *)m_pWindow)->setShowHorzScrollbar( false );
	((CEGUI::MultiColumnList *)m_pWindow)->setUserColumnDraggingEnabled( false );
	((CEGUI::MultiColumnList *)m_pWindow)->setSortDirection( CEGUI::ListHeaderSegment::None );
	m_pWindow->setVisible( true );

	// Store the pointer to this element
	m_pWindow->setUserData( (void *)this );

	// Set default variables
	SetSize( Vector2( 128, 24 ) );
	SetSelectionMode( CEGUI::MultiColumnList::SelectionMode::RowSingle );

	// Register our events
	m_pWindow->subscribeEvent( CEGUI::MultiColumnList::EventSortColumnChanged, CEGUI::Event::Subscriber( &CGUIGridList_Impl::Event_OnSortColumn, this ) );
	m_pWindow->subscribeEvent( CEGUI::MultiColumnList::EventSelectionChanged, CEGUI::Event::Subscriber( &CGUIGridList_Impl::Event_OnSelectionChanged, this ) );
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

CGUIGridList_Impl::~CGUIGridList_Impl( void )
{
	// Clear the gridlist
	Clear();

	// Destroy the element
	DestroyElement();
}

unsigned int CGUIGridList_Impl::GetUniqueHandle( void )
{
	return ++m_uiUniqueHandle;
}

unsigned int CGUIGridList_Impl::AddColumn( String strTitle, float fWidth )
{
	// Create a new column with a unique handle
	unsigned int uiHandle = GetUniqueHandle();
	((CEGUI::MultiColumnList *)m_pWindow)->addColumn( strTitle.Get(), uiHandle, fWidth );

	// Get the column id
	int iColumnIndex = ((CEGUI::MultiColumnList *)m_pWindow)->getColumnWithID( uiHandle );

	// Adjust the header style
	((CEGUI::MultiColumnList *)m_pWindow)->getHeaderSegmentForColumn( iColumnIndex ).setFont( "tahoma" );

	return uiHandle;
}

void CGUIGridList_Impl::RemoveColumn( unsigned int uiIndex )
{
	try
	{
		// Remove the column
		((CEGUI::MultiColumnList *)m_pWindow)->removeColumn( uiIndex );
	}
	catch ( CEGUI::Exception ) {}
}

void CGUIGridList_Impl::SetColumnWidth( unsigned int uiIndex, float fWidth, bool bRelative )
{
	try
	{
		// Set the column header width
		((CEGUI::MultiColumnList *)m_pWindow)->setColumnHeaderWidth( uiIndex, fWidth, bRelative );
	}
	catch ( CEGUI::Exception ) {}
}

void CGUIGridList_Impl::SetSelectionMode( CEGUI::MultiColumnList::SelectionMode mode )
{
	// Set the gridlist selection mode
	((CEGUI::MultiColumnList *)m_pWindow)->setSelectionMode( mode );
}

int CGUIGridList_Impl::AddRow( bool bFast )
{
	// Add the row
	return ((CEGUI::MultiColumnList *)m_pWindow)->addRow( m_iIndex++, bFast );
}

void CGUIGridList_Impl::RemoveRow( int iRow )
{
	// Remove the row
	((CEGUI::MultiColumnList *)m_pWindow)->removeRow( iRow );
}

int CGUIGridList_Impl::InsertRowAfter( int iRow )
{
	// Insert the row
	return ((CEGUI::MultiColumnList *)m_pWindow)->insertRow( iRow + 1, m_iIndex++ );
}

int CGUIGridList_Impl::SetItemText( int iRow, int iColumn, String strText, bool bNumber, bool bSection, bool bFast )
{
	// Cast the window to a multicolumn list
	CEGUI::MultiColumnList * pList = (CEGUI::MultiColumnList *)m_pWindow;

	// Get the list item
	CGUIListItem_Impl * pItem = GetItem( iRow, iColumn );

	// Is the item pointer valid?
	if( pItem )
	{
		// Is this a section?
		if( bSection )
		{
			// Set the item font
			pItem->SetFont( "tahoma-bold" );

			// Set the item disabled
			pItem->SetDisabled( true );

			// Set the item text
			pItem->SetText( strText );
		}
	}
	else
	{
		// Create the item
		pItem = new CGUIListItem_Impl( strText, bNumber );

		// Get the cegui listbox item
		CEGUI::ListboxItem * pCEGUIItem = pItem->GetListItem();

		// Add the item to the list
		pList->setItem( pCEGUIItem, iColumn, iRow, bFast );

		// Add our new item into the map
		m_Items[ pItem->GetListItem() ] = pItem;

		// Is this a section?
		if( bSection )
		{
			// Set the section properies
			pItem->SetFont( "tahoma-bold" );
			pItem->SetDisabled( true );
		}
	}

	// Has the column been sorted?
	if( pList->getSortDirection() != CEGUI::ListHeaderSegment::SortDirection::None && pList->getSortColumn() == GetColumnIndex( iColumn ) )
	{
		// Resort it
		pList->setSortColumn( pList->getSortColumn() );

		//
		return GetItemRowIndex( pItem );
	}
	else
		return iRow;

	return 0;
}

void CGUIGridList_Impl::SetItemImage( int iRow, int iColumn, CGUIStaticImage_Impl * pImage )
{
	// Get the current item at the offset
	CGUIListItem_Impl * pItem = GetItem( iRow, iColumn );

	// Is the item valid?
	if( pItem )
	{
		// Set the item image
		pItem->SetImage( pImage );
	}
	else
	{
		// Create the item
		pItem = new CGUIListItem_Impl( "", CGUIListItem_Impl::Type::ImageItem, pImage );

		// Get the cegui item
		CEGUI::ListboxItem * pCEGUIItem = pItem->GetListItem();

		// Add the item to the gridlist
		((CEGUI::MultiColumnList *)m_pWindow)->setItem( pCEGUIItem, iColumn, iRow );

		// Add our new item into the map
		m_Items[ pItem->GetListItem() ] = pItem;
	}
}

char * CGUIGridList_Impl::GetItemText( int iRow, int iColumn )
{
	// Get the item at the row and column
	CEGUI::ListboxItem * pItem = ((CEGUI::MultiColumnList *)m_pWindow)->getItemAtGridReference( CEGUI::MCLGridRef( iRow, GetColumnIndex( iColumn ) ) );

	// Did we find the item?
	if( pItem )
		return const_cast< char* >( pItem->getText().c_str() );

	return "";
}

void CGUIGridList_Impl::Clear( void )
{
	try
	{
		// Reset counts
		m_iIndex = 0;

		// Clear the list
		((CEGUI::MultiColumnList *)m_pWindow)->resetList();

		// Loop through the list items
		std::map< CEGUI::ListboxItem*, CGUIListItem_Impl* >::iterator it = m_Items.begin();
		for( ; it != m_Items.end(); it++ )
		{
			// Delete the list item memory
			SAFE_DELETE( it->second );
		}

		// Clear the list itms
		m_Items.clear();
	}
	catch ( CEGUI::Exception ) {}
}

CGUIListItem_Impl * CGUIGridList_Impl::GetItem( int iRow, int iColumn )
{
	// Get a pointer to the cegui list item
	CEGUI::ListboxItem * pItem = ((CEGUI::MultiColumnList *)m_pWindow)->getItemAtGridReference( CEGUI::MCLGridRef( iRow, GetColumnIndex( iColumn ) ) );

	// Is the item pointer valid?
	if( pItem )
		return NULL; // todo

	return NULL;
}

int CGUIGridList_Impl::GetColumnIndex( int iColumn )
{
	return ((CEGUI::MultiColumnList *)m_pWindow)->getColumnWithID( iColumn );
}

int CGUIGridList_Impl::GetItemRowIndex( CGUIListItem_Impl * pItem )
{
	// Is the item valid?
	if( pItem )
		return ((CEGUI::MultiColumnList *)m_pWindow)->getItemRowIndex( pItem->GetListItem() );

	return -1;
}

int CGUIGridList_Impl::GetItemColumnIndex( CGUIListItem_Impl * pItem )
{
	// Is the item valid?
	if( pItem )
		return ((CEGUI::MultiColumnList *)m_pWindow)->getItemColumnIndex( pItem->GetListItem() );

	return -1;
}

int CGUIGridList_Impl::GetSelectedCount( void )
{
	return ((CEGUI::MultiColumnList *)m_pWindow)->getSelectedCount();
}

int CGUIGridList_Impl::GetSelectedItemRow( void )
{
	// Get the selected item
	CEGUI::ListboxItem * pItem = ((CEGUI::MultiColumnList *)m_pWindow)->getFirstSelectedItem();

	// Do we have anything selected?
	if( pItem )
	{
		// Get the CGUIListItem_Impl pointer
		CGUIListItem_Impl * pListItem = GetListItem( pItem );

		// Is the list item valid?
		if( pListItem )
			return GetItemRowIndex( pListItem );
	}

	return -1;
}

int CGUIGridList_Impl::GetSelectedItemColumn( void )
{
	// Get the selected item
	CEGUI::ListboxItem * pItem = ((CEGUI::MultiColumnList *)m_pWindow)->getFirstSelectedItem();

	// Do we have anything selected?
	if( pItem )
	{
		// Get the CGUIListItem_Impl pointer
		CGUIListItem_Impl * pListItem = GetListItem( pItem );

		// Is the list item valid?
		if( pListItem )
			return GetItemColumnIndex( pListItem );
	}

	return -1;
}

int CGUIGridList_Impl::GetRowCount( void )
{
	try
	{
		return ((CEGUI::MultiColumnList *)m_pWindow)->getRowCount();
	}
	catch ( CEGUI::Exception )
	{
		return 0;
	}
}

CGUIListItem_Impl * CGUIGridList_Impl::GetSelectedItem( void )
{
	return GetListItem( ((CEGUI::MultiColumnList *)m_pWindow)->getFirstSelectedItem() );
}

CGUIListItem_Impl * CGUIGridList_Impl::GetNextSelectedItem( CGUIListItem_Impl * pItem )
{
	if( pItem )
		return GetListItem( ((CEGUI::MultiColumnList *)m_pWindow)->getNextSelected( pItem->GetListItem() ) );
	else
		return GetListItem( ((CEGUI::MultiColumnList *)m_pWindow)->getFirstSelectedItem() );
}

CGUIListItem_Impl * CGUIGridList_Impl::GetListItem( CEGUI::ListboxItem * pItem )
{
	// Try find the item in the map
	std::map< CEGUI::ListboxItem*, CGUIListItem_Impl* >::iterator it = m_Items.find( pItem );

	// Couldn't we find it?
	if( it == m_Items.end() )
		return NULL;

	//
	return it->second;
}

bool CGUIGridList_Impl::Event_OnSortColumn( const CEGUI::EventArgs &e )
{
	// Call the sort column handler
	if( m_pfnOnSortColumn )
		m_pfnOnSortColumn( this );

	// Pass the event to the client gui manager
	if( pCore->GetClientScriptingManager() && pCore->GetClientScriptingManager()->GetScriptGUIManager() )
		pCore->GetClientScriptingManager()->GetScriptGUIManager()->HandleEvent( "onGuiElementSortColumn", this );

	return true;
}

bool CGUIGridList_Impl::Event_OnSelectionChanged( const CEGUI::EventArgs &e )
{
	// Call the sort column handler
	if( m_pfnOnSelectionChanged )
		m_pfnOnSelectionChanged( this );

	// Pass the event to the client gui manager
	if( pCore->GetClientScriptingManager() && pCore->GetClientScriptingManager()->GetScriptGUIManager() )
		pCore->GetClientScriptingManager()->GetScriptGUIManager()->HandleEvent( "onGuiElementSelectionChange", this );

	return true;
}