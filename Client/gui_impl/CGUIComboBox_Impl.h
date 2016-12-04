/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIComboBox_Impl.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CGUIListItem_Impl;
class CGUIComboBox_Impl : public CGUIElement_Impl
{

private:

	std::map< CEGUI::ListboxItem*, CGUIListItem_Impl* > m_Items;
	CGUIListItem_Impl	* GetListItem				( CEGUI::ListboxItem * pItem );

protected:
	GUI_CALLBACK		m_onSelectChange;
	GUI_CALLBACK		m_onDropListRemoved;

public:

						CGUIComboBox_Impl			( class CGUI_Impl * pGUI, const char * szCaption = "", CGUIElement_Impl * pParent = NULL );
						~CGUIComboBox_Impl			( void );

	CGUIListItem_Impl	* AddItem					( const char * szText );
	CGUIListItem_Impl	* GetSelectedItem			( void );
	void				Clear						( void );

	void				SetReadOnly					( bool bReadOnly );

	eGUIType			GetType						( void ) { return GUI_COMBOBOX; }

	bool				Event_OnSelectionAccepted	(const CEGUI::EventArgs & e);
	bool				Event_OnDropListRemoved		(const CEGUI::EventArgs& e);
};