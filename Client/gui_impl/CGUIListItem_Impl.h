/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIListItem_Impl.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CGUIListItem_Impl
{

protected:

	CEGUI::ListboxItem								* m_pListItem;
	void											* m_pData;
	String											m_strData;

public:

	enum Type
	{
		TextItem = 0,
		NumberItem,
		ImageItem
	};

	unsigned int									ItemType;

						CGUIListItem_Impl			( String strText, unsigned int uiType = 0, CGUIStaticImage_Impl * pImage = NULL );
						~CGUIListItem_Impl			( void );

	void				SetText						( String strText );
	String				GetText						( void );

	void				SetData						( void * pUserData ) { m_pData = pUserData; }
	void				* GetData					( void ) { return m_pData; }

	void				SetDisabled					( bool bDisabled );
	void				SetFont						( String strFont );
	void				SetImage					( CGUIStaticImage_Impl * pImage );

	void				SetSelected					( bool bSelected );
	bool				GetSelected					( void );
	
	CEGUI::ListboxItem	* GetListItem				( void ) { return m_pListItem; }

	eGUIType			GetType						( void ) { return GUI_LISTITEM; }

};