/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIGridList_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"
#include "CCore.h"

#include "CString.h"
#include "Math/CVector3.h"

#include "CGUI.h"
#include "CGUICallback.h"

#include "CClientScriptGUIManager.h"
#include "CClientScriptingManager.h"

#include "CGUI_Impl.h"
#include "CGUIWindow_Impl.h"
#include "CGUIListItem_Impl.h"
#include "CGUIGridList_Impl.h"

CGUIGridList_Impl::CGUIGridList_Impl( CGUI_Impl * pGUI, CGUIElement_Impl * pParent )
	: CGUIElement_Impl(pGUI)
{
	m_uiUniqueHandle = 0;
	m_iIndex = 0;

	String strName = pGUI->GetUniqueName();

	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/MultiColumnList", strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	m_pWindow->setRect( CEGUI::Relative, CEGUI::Rect( 0.00f, 0.00f, 0.40f, 0.40f ) );
	reinterpret_cast<CEGUI::MultiColumnList *>(m_pWindow)->setShowHorzScrollbar( false );
	reinterpret_cast<CEGUI::MultiColumnList *>(m_pWindow)->setUserColumnDraggingEnabled( false );
	reinterpret_cast<CEGUI::MultiColumnList *>(m_pWindow)->setSelectionMode(CEGUI::MultiColumnList::RowSingle);
	reinterpret_cast<CEGUI::MultiColumnList *>(m_pWindow)->setSortDirection( CEGUI::ListHeaderSegment::None );
	m_pWindow->setVisible( true );

	m_pWindow->setUserData( reinterpret_cast<void *>(this) );

	SetSize( Vector2( 128, 24 ) );

	m_pWindow->subscribeEvent( CEGUI::MultiColumnList::EventSortColumnChanged, CEGUI::Event::Subscriber( &CGUIGridList_Impl::Event_OnSortColumn, this ) );
	m_pWindow->subscribeEvent( CEGUI::MultiColumnList::EventSelectionChanged, CEGUI::Event::Subscriber( &CGUIGridList_Impl::Event_OnSelectionChanged, this ) );
	AddEvents();
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
	Clear();
	DestroyElement();
}

unsigned int CGUIGridList_Impl::GetUniqueHandle( void )
{
	return ++m_uiUniqueHandle;
}

unsigned int CGUIGridList_Impl::AddColumn( String strTitle, float fWidth )
{
	unsigned int uiHandle = GetUniqueHandle();
	reinterpret_cast<CEGUI::MultiColumnList *>(m_pWindow)->addColumn( strTitle.Get(), uiHandle, fWidth );

	int iColumnIndex = reinterpret_cast<CEGUI::MultiColumnList *>(m_pWindow)->getColumnWithID( uiHandle );

	reinterpret_cast<CEGUI::MultiColumnList *>(m_pWindow)->getHeaderSegmentForColumn( iColumnIndex ).setFont( "tahoma" );

	return iColumnIndex + 1;
}

void CGUIGridList_Impl::RemoveColumn( unsigned int uiIndex )
{
	try
	{
		reinterpret_cast<CEGUI::MultiColumnList *>(m_pWindow)->removeColumn( uiIndex );
	}
	catch ( CEGUI::Exception ) {}
}

void CGUIGridList_Impl::SetColumnWidth( unsigned int uiIndex, float fWidth, bool bRelative )
{
	try
	{
		reinterpret_cast<CEGUI::MultiColumnList *>(m_pWindow)->setColumnHeaderWidth( uiIndex, fWidth, bRelative );
	}
	catch ( CEGUI::Exception ) {}
}

void CGUIGridList_Impl::SetSelectionMode( CEGUI::MultiColumnList::SelectionMode mode )
{
	reinterpret_cast<CEGUI::MultiColumnList *>(m_pWindow)->setSelectionMode( mode );
}

int CGUIGridList_Impl::AddRow( bool bFast )
{
	return reinterpret_cast<CEGUI::MultiColumnList *>(m_pWindow)->addRow( m_iIndex++, bFast );
}

void CGUIGridList_Impl::RemoveRow( int iRow )
{
	reinterpret_cast<CEGUI::MultiColumnList *>(m_pWindow)->removeRow( iRow );
}

int CGUIGridList_Impl::InsertRowAfter( int iRow )
{
	return reinterpret_cast<CEGUI::MultiColumnList *>(m_pWindow)->insertRow( iRow + 1, m_iIndex++ );
}

int CGUIGridList_Impl::SetItemText( int iRow, int iColumn, String strText, bool bNumber, bool bSection, bool bFast )
{
	try
	{
		CEGUI::MultiColumnList * pList = reinterpret_cast<CEGUI::MultiColumnList *>(m_pWindow);

		CGUIListItem_Impl * pItem = reinterpret_cast < CGUIListItem_Impl* > (GetItem(iRow, iColumn));

		if (pItem)
		{
			if (bSection)
			{
				pItem->SetFont("tahoma-bold");
				pItem->SetDisabled(true);
				pItem->SetText(strText);
			}
		}
		else
		{
			pItem = new CGUIListItem_Impl(strText, bNumber);

			CEGUI::ListboxItem * pCEGUIItem = pItem->GetListItem();
			pList->setItem(pCEGUIItem, iColumn, iRow, bFast);

			m_Items[pItem->GetListItem()] = pItem;
			if (bSection)
			{
				pItem->SetFont("tahoma-bold");
				pItem->SetDisabled(true);
			}
		}
		if (pList->getSortDirection() != CEGUI::ListHeaderSegment::SortDirection::None && pList->getSortColumn() == GetColumnIndex(iColumn))
		{
			pList->setSortColumn(pList->getSortColumn());
			return GetItemRowIndex(pItem);
		}
		else
			return iRow;
	}
	catch (CEGUI::Exception) {}

	return 0;
}

void CGUIGridList_Impl::SetItemImage( int iRow, int iColumn, CGUIStaticImage_Impl * pImage )
{
	CGUIListItem_Impl * pItem = GetItem( iRow, iColumn );

	if( pItem )
	{
		pItem->SetImage( pImage );
	}
	else
	{
		pItem = new CGUIListItem_Impl( "", CGUIListItem_Impl::Type::ImageItem, pImage );
		CEGUI::ListboxItem * pCEGUIItem = pItem->GetListItem();
		reinterpret_cast<CEGUI::MultiColumnList*>(m_pWindow)->setItem( pCEGUIItem, iColumn, iRow );
		m_Items[ pItem->GetListItem() ] = pItem;
	}
}

char * CGUIGridList_Impl::GetItemText( int iRow, int iColumn )
{
	CEGUI::ListboxItem * pItem = reinterpret_cast<CEGUI::MultiColumnList*>(m_pWindow)->getItemAtGridReference( CEGUI::MCLGridRef( iRow, GetColumnIndex( iColumn ) ) );
	if( pItem )
		return const_cast< char* >( pItem->getText().c_str() );

	return "";
}

void CGUIGridList_Impl::Clear( void )
{
	try
	{
		m_iIndex = 0;
		reinterpret_cast<CEGUI::MultiColumnList*>(m_pWindow)->resetList();

		std::map< CEGUI::ListboxItem*, CGUIListItem_Impl* >::iterator it = m_Items.begin();
		for( ; it != m_Items.end(); it++ )
		{
			SAFE_DELETE( it->second );
		}
		m_Items.clear();
	}
	catch ( CEGUI::Exception ) {}
}

CGUIListItem_Impl * CGUIGridList_Impl::GetItem( int iRow, int iColumn )
{
	try
	{
		CEGUI::ListboxItem * pItem = reinterpret_cast<CEGUI::MultiColumnList*>(m_pWindow)->getItemAtGridReference(CEGUI::MCLGridRef(iRow, GetColumnIndex(iColumn)));

		if (pItem)
			return GetListItem(pItem);
	}
	catch (CEGUI::Exception) {}

	return nullptr;
}

int CGUIGridList_Impl::GetColumnIndex( int iColumn )
{
	return reinterpret_cast<CEGUI::MultiColumnList*>(m_pWindow)->getColumnWithID( iColumn );
}

int CGUIGridList_Impl::GetItemRowIndex( CGUIListItem_Impl * pItem )
{
	if( pItem )
		return reinterpret_cast<CEGUI::MultiColumnList*>(m_pWindow)->getItemRowIndex( pItem->GetListItem() );

	return -1;
}

int CGUIGridList_Impl::GetItemColumnIndex( CGUIListItem_Impl * pItem )
{
	if( pItem )
		return reinterpret_cast<CEGUI::MultiColumnList*>(m_pWindow)->getItemColumnIndex( pItem->GetListItem() );

	return -1;
}

int CGUIGridList_Impl::GetSelectedCount( void )
{
	return reinterpret_cast<CEGUI::MultiColumnList*>(m_pWindow)->getSelectedCount();
}

int CGUIGridList_Impl::GetSelectedItemRow( void )
{
	CEGUI::ListboxItem * pItem = reinterpret_cast<CEGUI::MultiColumnList*>(m_pWindow)->getFirstSelectedItem();
	if( pItem )
	{
		CGUIListItem_Impl * pListItem = GetListItem( pItem );
		if( pListItem )
			return GetItemRowIndex( pListItem );
	}

	return -1;
}

int CGUIGridList_Impl::GetSelectedItemColumn( void )
{
	CEGUI::ListboxItem * pItem = reinterpret_cast<CEGUI::MultiColumnList*>(m_pWindow)->getFirstSelectedItem();
	if( pItem )
	{
		CGUIListItem_Impl * pListItem = GetListItem( pItem );
		if( pListItem )
			return GetItemColumnIndex( pListItem );
	}

	return -1;
}

int CGUIGridList_Impl::GetRowCount( void )
{
	try
	{
		return reinterpret_cast<CEGUI::MultiColumnList*>(m_pWindow)->getRowCount();
	}
	catch ( CEGUI::Exception )
	{
		return 0;
	}
}

CGUIListItem_Impl * CGUIGridList_Impl::GetSelectedItem( void )
{
	return GetListItem(reinterpret_cast<CEGUI::MultiColumnList*>(m_pWindow)->getFirstSelectedItem() );
}

CGUIListItem_Impl * CGUIGridList_Impl::GetNextSelectedItem( CGUIListItem_Impl * pItem )
{
	if( pItem )
		return GetListItem(reinterpret_cast<CEGUI::MultiColumnList*>(m_pWindow)->getNextSelected( pItem->GetListItem() ) );
	else
		return GetListItem(reinterpret_cast<CEGUI::MultiColumnList*>(m_pWindow)->getFirstSelectedItem() );
}

CGUIListItem_Impl * CGUIGridList_Impl::GetListItem( CEGUI::ListboxItem * pItem )
{
	std::map< CEGUI::ListboxItem*, CGUIListItem_Impl* >::iterator it = m_Items.find( pItem );
	if( it == m_Items.end() )
		return nullptr;

	return it->second;
}

bool CGUIGridList_Impl::Event_OnSortColumn( const CEGUI::EventArgs &e )
{
	if( m_pfnOnSortColumn )
		m_pfnOnSortColumn( reinterpret_cast<CGUIElement_Impl *>(this) );

	if (CCore::Instance()->GetClientScriptingManager() && CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager())
		CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager()->HandleEvent("onGuiElementSortColumn", this);

	return true;
}

bool CGUIGridList_Impl::Event_OnSelectionChanged( const CEGUI::EventArgs &e )
{
	if( m_pfnOnSelectionChanged )
		m_pfnOnSelectionChanged( reinterpret_cast<CGUIElement_Impl *>(this) );

	if (CCore::Instance()->GetClientScriptingManager() && CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager())
		CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager()->HandleEvent("onGuiElementSelectionChange", this);

	return true;
}