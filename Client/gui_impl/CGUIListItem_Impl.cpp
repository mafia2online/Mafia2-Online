/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIListItem_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"../StdInc.h"

CGUIListItem_Impl::CGUIListItem_Impl( String strText, unsigned int uiType, CGUIStaticImage_Impl * pImage )
{
	// Store the item type
	ItemType = uiType;

	// Reset
	m_pListItem = NULL;
	m_pData = NULL;

	// Create the requested list type
	switch( uiType )
	{
	case Type::TextItem:
		m_pListItem = new CEGUI::ListboxTextItem( strText.Get() );
		break;

	case Type::NumberItem:
		m_pListItem = new CEGUI::ListboxNumberItem( strText.Get() );
		break;

	case Type::ImageItem:
		m_pListItem = new CEGUI::ListboxImageItem( *pImage->GetImage() );
		break;
	}

	// Make sure the item is valid
	if( m_pListItem )
	{
		// Set stuff
		m_pListItem->setAutoDeleted( false );
		m_pListItem->setSelectionBrushImage( "CGUI-Images", "ListboxSelectionBrush" );
	}
}

CGUIListItem_Impl::~CGUIListItem_Impl( void )
{
	// Delete the list item
	SAFE_DELETE( m_pListItem );
}

void CGUIListItem_Impl::SetText( String strText )
{
	// Set the list item text
	m_pListItem->setText( strText.Get() );
}

String CGUIListItem_Impl::GetText( void )
{
	return String( m_pListItem->getText().c_str() );
}

void CGUIListItem_Impl::SetDisabled( bool bDisabled )
{
	// Toggle the list item
	((CEGUI::ListboxItem *)m_pListItem)->setDisabled( bDisabled );
}

void CGUIListItem_Impl::SetFont( String strFont )
{
	// Set the item font
	((CEGUI::ListboxTextItem *)m_pListItem)->setFont( strFont.Get() );
}

void CGUIListItem_Impl::SetImage( CGUIStaticImage_Impl * pImage )
{
	// Is the list item not an image?
	if( ItemType != Type::ImageItem )
		return;

	// Set the image
	((CEGUI::ListboxImageItem *)m_pListItem)->setImage( pImage->GetImage() );
}

void CGUIListItem_Impl::SetSelected( bool bSelected )
{
	// Toggle the selection
	m_pListItem->setSelected( bSelected );
}

bool CGUIListItem_Impl::GetSelected( void )
{
	return m_pListItem->isSelected();
}