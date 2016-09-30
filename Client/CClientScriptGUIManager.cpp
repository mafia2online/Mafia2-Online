/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CClientScriptGUIManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "gui_impl/CGUI_Impl.h"
#include "gui_impl/CGUIElement_Impl.h"

#include "Scripting/CSquirrel.h"

#include "CClientScriptingManager.h"
#include "CEvents.h"

#include "CClientScriptGUIManager.h"

#include "CLogFile.h"

CClientScriptGUIManager::CClientScriptGUIManager( void )
{
	// Flag as not hidden
	m_bHidden = false;
}

CClientScriptGUIManager::~CClientScriptGUIManager( void )
{
	// Delete all elements
	DeleteAll ();
}

void CClientScriptGUIManager::Add( const std::shared_ptr<CGUIElement_Impl> &pElement, CSquirrel * pScript )
{
	// Create a gui element instance
	GUIElement * pGUIElement = new GUIElement;

	// Set the structure data
	pGUIElement->pElement = pElement;
	pGUIElement->pScript = pScript;
	pGUIElement->bState = true;

	// Add the new element to the elements list
	m_elements.push_back( pGUIElement );
}

void CClientScriptGUIManager::Delete( CGUIElement_Impl * pElement )
{
	// Loop through all elements
	for( std::list< GUIElement* >::iterator iter = m_elements.begin(); iter != m_elements.end(); iter++ )
	{
		// Is this the element we're looking for?
		if( *iter && (*iter)->pElement.get() == pElement )
		{
			// Delete the element
			SAFE_DELETE( *iter );

			// Erase the element from the elements list
			m_elements.erase( iter );

			break;
		}
	}
}

bool CClientScriptGUIManager::Exists( CGUIElement_Impl * pElement )
{
	// Loop through all elements
	for( std::list< GUIElement* >::iterator iter = m_elements.begin(); iter != m_elements.end(); iter++ )
	{
		// Get the current gui element pointer
		GUIElement * pGUIElement = *iter;

		// Is this the element we're looking for?
		if( pGUIElement && pGUIElement->pElement.get() == pElement )
			return true;
	}

	return false;
}

void CClientScriptGUIManager::DeleteAll( void )
{
	// Loop through all elements
	for( std::list< GUIElement* >::iterator iter = m_elements.begin(); iter != m_elements.end(); iter++ )
	{
		// Delete the current item
		SAFE_DELETE ( *iter );
	}

	// Clear the element list
	m_elements.clear ();
}

void CClientScriptGUIManager::DeleteAll( CSquirrel * pScript )
{
	std::list< GUIElement* >::iterator iter = m_elements.begin();

	// Loop over all gui elements
	while ( iter != m_elements.end() )
	{
		// Is this the element loaded on the script we're looking for?
		if ( (*iter)->pScript = pScript )
		{
			// Delete and erase the element from the elements list
			delete *iter;
			iter = m_elements.erase( iter );
		}
		else
			iter++;
	}
}

void CClientScriptGUIManager::Show( void )
{
	// Are we hidden?
	if( m_bHidden )
	{
		// Loop through all elements
		for( std::list< GUIElement* >::iterator iter = m_elements.begin(); iter != m_elements.end(); iter++ )
		{
			// Does this element have a parent element?
			if ( (*iter)->pElement->HasParent () )
				continue;

			// Restore the current elements visibility state
			(*iter)->pElement->SetVisible( (*iter)->bState );

			DEBUG_LOG ( "Restored element 0x%p visibility.", (*iter)->pElement );
		}

		// Flag as not hidden
		m_bHidden = false;
	}
}

void CClientScriptGUIManager::Hide( void )
{
	// Are we not hidden?
	if( !m_bHidden )
	{
		// Loop through all elements
		for( std::list< GUIElement* >::iterator iter = m_elements.begin(); iter != m_elements.end(); iter++ )
		{
			// Does this element have a parent element?
			if ( (*iter)->pElement->HasParent () )
				continue;

			// Store the current elements visibility state
			(*iter)->bState = (*iter)->pElement->IsVisible();

			// Hide the current element
			(*iter)->pElement->SetVisible( false );

			DEBUG_LOG ( "Stored element 0x%p visibility. (%s)", (*iter)->pElement, ((*iter)->bState ? "true" : "false") );
		}

		// Flag as hidden
		m_bHidden = true;
	}
}

CSquirrel * CClientScriptGUIManager::GetScript( CGUIElement_Impl * pElement )
{
	// Loop through all elements
	for( std::list< GUIElement* >::iterator iter = m_elements.begin(); iter != m_elements.end(); iter++ )
	{
		// Get the current gui element pointer
		GUIElement * pGUIElement = *iter;

		// Is this the element we're looking for?
		if( pGUIElement && pGUIElement->pElement.get() == pElement )
			return pGUIElement->pScript;
	}

	return NULL;
}

void CClientScriptGUIManager::HandleEvent( const char * szEventName, CGUIElement_Impl * pElement )
{
	// Get the element script
	CSquirrel * pScript = GetScript ( pElement );

	// Is the script invalid?
	if ( !pScript )
		return;

	// Call the scripting event
	CSquirrelArguments args;
	args.pushUserPointer( (void *)pElement );
	CCore::Instance()->GetClientScriptingManager()->GetEvents()->Call( szEventName, &args, pScript );
}