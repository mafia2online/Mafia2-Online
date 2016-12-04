/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIListItem_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"
#include "CCore.h"

#include "CString.h"
#include "Math/CVector3.h"

#include "CGUIStaticImage_Impl.h"
#include "CGUITypes.h"

#include "CGUI.h"

#include "CClientScriptGUIManager.h"
#include "CClientScriptingManager.h"

#include "CGUI_Impl.h"
#include "CGUIListItem_Impl.h"

CGUIListItem_Impl::CGUIListItem_Impl( String strText, unsigned int uiType, CGUIStaticImage_Impl * pImage )
{
	ItemType = uiType;
	m_pListItem = NULL;
	m_pData = NULL;

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

	if( m_pListItem )
	{
		m_pListItem->setAutoDeleted( false );
		m_pListItem->setSelectionBrushImage( "CGUI-Images", "ListboxSelectionBrush" );
	}
}

CGUIListItem_Impl::~CGUIListItem_Impl( void )
{
	SAFE_DELETE( m_pListItem );
}

void CGUIListItem_Impl::SetText( String strText )
{
	m_pListItem->setText( strText.Get() );
}

String CGUIListItem_Impl::GetText( void )
{
	return String( m_pListItem->getText().c_str() );
}

void CGUIListItem_Impl::SetDisabled( bool bDisabled )
{
	reinterpret_cast<CEGUI::ListboxItem*>(m_pListItem)->setDisabled( bDisabled );
}

void CGUIListItem_Impl::SetFont( String strFont )
{
	reinterpret_cast<CEGUI::ListboxTextItem *>(m_pListItem)->setFont( strFont.Get() );
}

void CGUIListItem_Impl::SetImage( CGUIStaticImage_Impl * pImage )
{
	if( ItemType != Type::ImageItem )
		return;

	reinterpret_cast<CEGUI::ListboxImageItem *>(m_pListItem)->setImage( pImage ? pImage->GetImage() : nullptr);
}

void CGUIListItem_Impl::SetSelected( bool bSelected )
{
	m_pListItem->setSelected( bSelected );
}

bool CGUIListItem_Impl::GetSelected( void )
{
	return m_pListItem->isSelected();
}