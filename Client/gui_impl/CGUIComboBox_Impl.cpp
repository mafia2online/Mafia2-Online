/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIComboBox_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CString.h"

#include "CGUI.h"

#include "CGUI_Impl.h"
#include "CGUIElement_Impl.h"
#include "CGUIListItem_Impl.h"
#include "CGUIComboBox_Impl.h"

CGUIComboBox_Impl::CGUIComboBox_Impl( CGUI_Impl * pGUI, const char * szCaption, CGUIElement_Impl * pParent )
	: CGUIElement_Impl(pGUI)
{
	String strName = pGUI->GetUniqueName();

	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/Combobox", strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	SetText(szCaption);
	m_pWindow->setSize( CEGUI::Absolute, CEGUI::Size( 128.0f, 24.0f ) );
	m_pWindow->setVisible( true );

	m_pWindow->setUserData( reinterpret_cast<void *>(this) );

	m_pWindow->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&CGUIComboBox_Impl::Event_OnSelectionAccepted, this));
	m_pWindow->subscribeEvent(CEGUI::Combobox::EventDropListRemoved, CEGUI::Event::Subscriber(&CGUIComboBox_Impl::Event_OnDropListRemoved, this));

	AddEvents();
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
	Clear();
	DestroyElement();
}

CGUIListItem_Impl * CGUIComboBox_Impl::AddItem( const char * szText )
{
	CGUIListItem_Impl * pItem = new CGUIListItem_Impl( szText, CGUIListItem_Impl::Type::TextItem, NULL );
	CEGUI::ListboxItem * pListBoxItem = pItem->GetListItem();
	reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->addItem(pListBoxItem);
	m_Items[ pListBoxItem ] = pItem;
	return pItem;
}

CGUIListItem_Impl * CGUIComboBox_Impl::GetSelectedItem( void )
{
	return GetListItem(reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->getSelectedItem());
}

void CGUIComboBox_Impl::Clear( void )
{
	reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->getDropList()->resetList();

	for( std::map< CEGUI::ListboxItem*, CGUIListItem_Impl* >::iterator iter = m_Items.begin(); iter != m_Items.end(); iter++ )
		delete iter->second;

	m_Items.clear();
	m_pWindow->setText("");
}

void CGUIComboBox_Impl::SetReadOnly( bool bReadOnly )
{
	reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->setReadOnly(bReadOnly);
}

CGUIListItem_Impl * CGUIComboBox_Impl::GetListItem( CEGUI::ListboxItem * pItem )
{
	std::map< CEGUI::ListboxItem*, CGUIListItem_Impl* >::iterator iter = m_Items.find( pItem );
	if( iter == m_Items.end() )
		return NULL;

	return iter->second;
}

bool CGUIComboBox_Impl::Event_OnSelectionAccepted(const CEGUI::EventArgs& e)
{
	if (m_onSelectChange)
		m_onSelectChange(reinterpret_cast < CGUIElement_Impl* > (this));
	return true;
}


bool CGUIComboBox_Impl::Event_OnDropListRemoved(const CEGUI::EventArgs& e)
{
	if (m_onDropListRemoved)
		m_onDropListRemoved(reinterpret_cast < CGUIElement_Impl* > (this));
	return true;
}