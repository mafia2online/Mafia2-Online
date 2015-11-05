/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIEdit_Impl.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CGUIEdit_Impl : public CGUIElement_Impl
{

protected:

	GUI_CALLBACK									m_pfnOnTextChanged;
	GUI_CALLBACK									m_pfnOnTextAccepted;

	bool				Event_OnTextChanged			( const CEGUI::EventArgs &e );
	bool				Event_OnKeyDown				( const CEGUI::EventArgs &e );

public:

						CGUIEdit_Impl				( class CGUI_Impl * pGUI, String strText, CGUIElement_Impl * pParent = NULL );
						~CGUIEdit_Impl				( void );

	void				SetReadOnly					( bool bChecked );
	bool				IsReadOnly					( void );

	void				SetMasked					( bool bMasked );
	bool				IsMasked					( void );

	void				SetMaxLength				( unsigned int uiMaxLength );
	unsigned int		GetMaxLength				( void );

	void				SetSelection				( unsigned int uiStart, unsigned int uiEnd );
	unsigned int		GetSelectionStart			( void );
	unsigned int		GetSelectionEnd				( void );
	unsigned int		GetSelectionLength			( void );

	void				SetTextChangedHandler		( GUI_CALLBACK pfnOnTextChanged ) { m_pfnOnTextChanged = pfnOnTextChanged; }
	void				SetTextAcceptedHandler		( GUI_CALLBACK pfnOnTextAccepted ) { m_pfnOnTextAccepted = pfnOnTextAccepted; }

	bool				ActivateOnTab				( void );

	eGUIType			GetType						( void ) { return GUI_EDIT; }

};