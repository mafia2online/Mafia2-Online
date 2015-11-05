/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CXML.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"../Libraries/tinyxml/tinyxml.h"
#include	"../Libraries/tinyxml/ticpp.h"
#include	<list>
#include	"CString.h"

class CXMLNode;
class CXML
{

private:

	_TiXmlDocument										m_document;
	_TiXmlElement										* m_pRootNode;
	CXMLNode											* m_pNode;

	const char											* m_szFileName;

	void						BuildChildren			( CXMLNode * pNode );

public:

								CXML					( const char * szFileName );
								~CXML					( void );

	void						SetFileName				( const char * szFileName ) { m_szFileName = szFileName; }
	const char					* GetFileName			( void ) { return m_szFileName; }

	bool						Load					( void );
	bool						Save					( void );

	CXMLNode					* CreateRootNode		( const char * szName );
	CXMLNode					* GetRootNode			( void ) { return m_pNode; }

	_TiXmlDocument				GetXMLDocument			( void ) { return m_document; }
	_TiXmlElement				* GetXMLRootNode		( void ) { return m_pRootNode; }

};

class CXMLNode
{

private:

	_TiXmlElement										* m_pNode;
	CXMLNode											* m_pParent;

	std::list< CXMLNode* >								m_Children;
	
public:

								CXMLNode				( _TiXmlElement * pNode, CXMLNode * pParent = NULL );
								~CXMLNode				( void );

	void						SetName					( const char * szName ) { m_pNode->SetValue( szName ); }
	const char					* GetName				( void ) { return m_pNode->Value(); }

	void						SetValue				( const char * szValue );
	const char					* GetValue				( void ) { return m_pNode->GetText(); }

	CXMLNode					* CreateSubNode			( const char * szName );
	CXMLNode					* FindNode				( const char * szName );
	CXMLNode					* GetNode				( unsigned int uiIndex );

	void						SetAttribute			( const char * szName, const char * szValue ) { m_pNode->SetAttribute( szName, szValue ); }
	const char					* GetAttribute			( const char * szName ) { return m_pNode->Attribute( szName ); }

	void						AddChild				( CXMLNode * pNode ) { m_Children.push_back( pNode ); }
	void						RemoveChild				( CXMLNode * pNode ) { if( !m_Children.empty() ) { m_Children.remove( pNode ); } }
	unsigned int				GetChildCount			( void ) { return m_Children.size(); }
	std::list< CXMLNode* >		GetChildren				( void ) { return m_Children; }

	_TiXmlElement				* GetNode				( void ) { return m_pNode; }
	CXMLNode					* GetParent				( void ) { return m_pParent; }

};