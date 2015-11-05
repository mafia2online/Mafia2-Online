/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CClientScriptGUIManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

struct GUIElement
{
	CGUIElement_Impl	* pElement;
	CSquirrel			* pScript;
	bool				bState;
};

class CClientScriptGUIManager
{

private:

	std::list< GUIElement* >									m_elements;
	bool														m_bHidden;

public:

						CClientScriptGUIManager					( void );
						~CClientScriptGUIManager				( void );

	void				Add										( CGUIElement_Impl * pElement, CSquirrel * pScript );
	void				Delete									( CGUIElement_Impl * pElement );
	bool				Exists									( CGUIElement_Impl * pElement );

	void				DeleteAll								( void );
	void				DeleteAll								( CSquirrel * pScript );

	void				Show									( void );
	void				Hide									( void );

	CSquirrel			* GetScript								( CGUIElement_Impl * pElement );

	void				HandleEvent								( const char * szEventName, CGUIElement_Impl * pElement );

};