/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIGridList_Impl.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CGUIGridList_Impl : public CGUIElement_Impl
{

protected:

	unsigned int									m_uiUniqueHandle;
	int												m_iIndex;

	CGUIListItem_Impl								* GetListItem( CEGUI::ListboxItem * pItem );
	std::map< CEGUI::ListboxItem*, CGUIListItem_Impl* > m_Items;

	GUI_CALLBACK									m_pfnOnSortColumn;
	GUI_CALLBACK									m_pfnOnSelectionChanged;

	bool				Event_OnSortColumn			( const CEGUI::EventArgs &e );
	bool				Event_OnSelectionChanged	( const CEGUI::EventArgs &e );

public:

						CGUIGridList_Impl			( class CGUI_Impl * pGUI, CGUIElement_Impl * pParent = NULL );
						~CGUIGridList_Impl			( void );

	unsigned int		GetUniqueHandle				( void );

	unsigned int		AddColumn					( String strTitle, float fWidth );
	void				RemoveColumn				( unsigned int uiIndex );

	void				SetColumnWidth				( unsigned int uiIndex, float fWidth, bool bRelative = true );

	void				SetSelectionMode			( CEGUI::MultiColumnList::SelectionMode mode );

	int					AddRow						( bool bFast = true );
	void				RemoveRow					( int iRow );
	int					InsertRowAfter				( int iRow );

	int					SetItemText					( int iRow, int iColumn, String strText, bool bNumber = false, bool bSection = false, bool bFast = false );
	void				SetItemImage				( int iRow, int iColumn, CGUIStaticImage_Impl * pImage );
	
	char				* GetItemText				( int iRow, int iColumn );

	void				Clear						( void );

	CGUIListItem_Impl	* GetItem					( int iRow, int iColumn );
	int					GetColumnIndex				( int iColumn );
	int					GetItemRowIndex				( CGUIListItem_Impl * pItem );
	int					GetItemColumnIndex			( CGUIListItem_Impl * pItem );

	int					GetSelectedCount			( void );
	int					GetSelectedItemRow			( void );
	int					GetSelectedItemColumn		( void );
	int					GetRowCount					( void );

	CGUIListItem_Impl	* GetSelectedItem			( void );
	CGUIListItem_Impl	* GetNextSelectedItem		( CGUIListItem_Impl * pItem );

	void				SetSortColumnHandler		( GUI_CALLBACK callback ) { m_pfnOnSortColumn = callback; }
	void				SetSelectionChangeHandler	( GUI_CALLBACK callback ) { m_pfnOnSortColumn = callback; }

	eGUIType			GetType						( void ) { return GUI_GRIDLIST; }

};