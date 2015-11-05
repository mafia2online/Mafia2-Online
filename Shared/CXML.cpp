/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CXML.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"CXML.h"
#include	"CString.h"
#include	"CLogFile.h"
#include	"SharedUtility.h"

CXML::CXML( const char * szFileName )
{
	// Initialise
	m_pRootNode = NULL;
	m_pNode = NULL;

	// Set the filename
	SetFileName( szFileName );

	// Load the xml file
	Load();
}

CXML::~CXML( void )
{
	// Is the node valid?
	if( m_pNode )
	{
		// Delete the root node wrapper
		delete m_pNode;

		// Reset
		m_pNode = NULL;
	}
}

bool CXML::Load( void )
{
	// Try and load the file
	if( !m_document.LoadFile( m_szFileName ) )
		return false;

	// Get the root node instance
	m_pRootNode = m_document.RootElement();

	// Create the wrapper node
	m_pNode = new CXMLNode( m_pRootNode, NULL );

	// Build all the root node children
	BuildChildren( m_pNode );

	return true;
}

bool CXML::Save( void )
{
	return m_document.SaveFile();
}

void CXML::BuildChildren( CXMLNode * pNode )
{
	// Get the tiny xml node
	_TiXmlElement * pXmlNode = pNode->GetNode();

	// Is the xml node valid?
	if( pXmlNode )
	{
		// Loop over all child elements
		for( _TiXmlNode * pCurNode = pXmlNode->FirstChildElement(); pCurNode; pCurNode = pCurNode->NextSibling() )
		{
			// Is this node not an element?
			if( pCurNode->Type() != _TiXmlElement::ELEMENT )
				continue;

			// Create a new node wrapper for this element
			CXMLNode * pNewNode = new CXMLNode( pCurNode->ToElement(), pNode );

			// Build all node children
			BuildChildren( pNewNode );
		}
	}
}

CXMLNode * CXML::CreateRootNode( const char * szName )
{
	// Make sure we don't already have a root node
	if( !m_pNode )
	{
		// Get the documents root element
		if( !m_pRootNode )
		{
			// Create the root node
			m_pRootNode = new _TiXmlElement( szName );

			// Link the root node
			m_document.LinkEndChild( m_pRootNode );
		}

		// Create the root node wrapper
		m_pNode = new CXMLNode( m_pRootNode, NULL );
	}

	m_pNode->SetName( szName );
	return m_pNode;
}

// XML Nodes
CXMLNode::CXMLNode( _TiXmlElement * pNode, CXMLNode * pParent ) : m_pNode( pNode )
{
	// Store the parent
	m_pParent = pParent;

	// Add to parent list
	if( pParent )
		pParent->AddChild( this );
}

CXMLNode::~CXMLNode( void )
{
	// Delete all children
	//for( std::list< CXMLNode* >::iterator iter = m_Children.begin(); iter != m_Children.end(); iter++ ) {
	//	CLogFile::Printf( "Deleting node memory 0x%p (%s)...", *iter, (*iter)->GetValue() );
	//	delete *iter;  // todo: MAKE SURE THIS DOESN'T LEAK MEMORY!
	//	CLogFile::Printf( "Done!" );
	//}

	// Reset
	m_Children.clear();

	// Remove from parent list
	if( m_pParent )
		m_pParent->RemoveChild( this );

	// Is the node valid?
	if( m_pNode )
	{
		// Grab the node parent
		_TiXmlNode* pParent = m_pNode->Parent();

		// Is the parent node valid?
		if( pParent )
			pParent->RemoveChild( m_pNode );
		else
			delete m_pNode;
	}
}

void CXMLNode::SetValue( const char * szValue )
{
	// Clear the current value
	m_pNode->Clear();

	// Create a new text element
	_TiXmlText * pText = new _TiXmlText( szValue );

	// Link the text element to the node
	m_pNode->LinkEndChild( pText );
}

CXMLNode * CXMLNode::CreateSubNode( const char * szName )
{
	// Create a new sub node
	_TiXmlElement * pNewNode = new _TiXmlElement( szName );

	// Link the sub name after the end child
	m_pNode->LinkEndChild( pNewNode );

	// Create and return the wrapper element
	return new CXMLNode( pNewNode, this );
}

// todo: More than 1 child return!
CXMLNode * CXMLNode::FindNode( const char * szName )
{
	// Loop through all nodes
	for( std::list< CXMLNode* >::iterator iter = m_Children.begin(); iter != m_Children.end(); iter++ )
	{
		// Is this the node we're looking for?
		if( !strcmp( (*iter)->GetName(), szName ) )
			return *iter;
	}

	return NULL;
}

CXMLNode * CXMLNode::GetNode( unsigned int uiIndex )
{
	unsigned int uiCurrentIndex = 0;

	// Loop through all nodes
	for( std::list< CXMLNode* >::iterator iter = m_Children.begin(); iter != m_Children.end(); iter++ )
	{
		// Is this the index we're looking for?
		if( uiCurrentIndex == uiIndex )
			return *iter;

		// Increase the current index
		uiCurrentIndex++;
	}

	return NULL;
}