/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CXMLNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "CXMLNatives.h"
#include "../../CXML.h"

#ifdef _CLIENT
#include	"../../../Client/StdInc.h"
#else
#include	"../../../Server/StdInc.h"
#endif

extern	CCore			* pCore;

_MEMBER_FUNCTION_IMPL(xml, constructor);
_MEMBER_FUNCTION_IMPL(xml, save);
_MEMBER_FUNCTION_IMPL(xml, createRootNode);
_MEMBER_FUNCTION_IMPL(xml, getRootNode);
_MEMBER_FUNCTION_IMPL(xml, createNode);
_MEMBER_FUNCTION_IMPL(xml, findNode);
_MEMBER_FUNCTION_IMPL(xml, setNodeName);
_MEMBER_FUNCTION_IMPL(xml, getNodeName);
_MEMBER_FUNCTION_IMPL(xml, setNodeValue);
_MEMBER_FUNCTION_IMPL(xml, getNodeValue);
_MEMBER_FUNCTION_IMPL(xml, setNodeAttribute);
_MEMBER_FUNCTION_IMPL(xml, getNodeAttribute);

_BEGIN_CLASS(xml)
_MEMBER_FUNCTION(xml, constructor, 1, "s")				// xml( const char* )
_MEMBER_FUNCTION(xml, save, 0, NULL)					// save();
_MEMBER_FUNCTION(xml, createRootNode, 1, "s")			// createRootNode( const char* );
_MEMBER_FUNCTION(xml, getRootNode, 0, NULL)				// getRootNode();
_MEMBER_FUNCTION(xml, createNode, 2, "ps")				// createNode( CXMLNode*, const char* );
_MEMBER_FUNCTION(xml, findNode, 2, "ps")				// findNode( CXMLNode*, const char* );
_MEMBER_FUNCTION(xml, setNodeName, 2, "ps")				// setNodeName( CXMLNode*, const char* ); 
_MEMBER_FUNCTION(xml, getNodeName, 1, "p")				// getNodeName( CXMLNode* );
_MEMBER_FUNCTION(xml, setNodeValue, 2, "ps")			// setNodeValue( CXMLNode*, const char* );
_MEMBER_FUNCTION(xml, getNodeValue, 1, "p")				// getNodeValue( CXMLNode* );
_MEMBER_FUNCTION(xml, setNodeAttribute, 3, "pss")		// setNodeAttribute( CXMLNode*, const char*, const char* );
_MEMBER_FUNCTION(xml, getNodeAttribute, 2, "ps")		// getNodeAttribute( CXMLNode*, const char* );
_END_CLASS(xml)

void CXMLNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->NewClass( &_CLASS_DECL(xml) );
}

_MEMBER_FUNCTION_RELEASE_HOOK(xml)
{
	// Get the xml instance pointer
	CXML * pXML = reinterpret_cast< CXML* >( pInst );

	// Delete the xml instance
	if( pXML )
		SAFE_DELETE( pXML );

	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, constructor)
{
	// Get the filename
	const char * szFileName;
	sq_getstring( pVM, -1, &szFileName );

	// Do we have a valid filename?
	if( szFileName )
	{
		// Generate the path string
		String strPath;
#ifdef _CLIENT
		strPath = pCore->GetClientScriptingManager()->GetScriptingManager()->Get( pVM )->GetPath();
		strPath = SharedUtility::GetFileNameForScriptFile( szFileName, strPath.Get(), pCore->GetHost().Get(), pCore->GetPort() );
#else
		strPath = pCore->GetScriptingManager()->Get( pVM )->GetPath();
		strPath = SharedUtility::GetFileNameForScriptFile( szFileName, strPath.Get() );
#endif

		// Create the XML instance
		CXML * pXML = new CXML( strPath.Get() );

		// Did the XML instance fail to create or set the squirrel instance?
		if( !pXML || SQ_FAILED( sq_setinstance( pVM, pXML ) ) )
		{
			// Delete the xml instance
			if( pXML )
				SAFE_DELETE( pXML );

			//
			sq_pushbool( pVM, false );
			return 1;
		}

		//_SET_RELEASE_HOOK(xml);
		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, save)
{
	// Get the xml instance pointer
	CXML * pXML = sq_getinstance< CXML* >( pVM );

	// Is the xml instance invalid?
	if( !pXML )
	{
		sq_pushbool( pVM, false );
		return 1;
	}

	sq_pushbool( pVM, pXML->Save() );
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, createRootNode)
{
	// Get the root node name
	const SQChar * szName;
	sq_getstring( pVM, 2, &szName );

	// Is the root name valid?
	if( szName )
	{
		// Get the xml instance pointer
		CXML * pXML = sq_getinstance< CXML* >( pVM );

		// Is the xml instance valid?
		if( pXML )
		{
			// Is the root node not valid?
			if( !pXML->GetRootNode() )
			{
				// Create the root node
				CXMLNode * pRootNode = pXML->CreateRootNode( szName );

				// Push the root node instance
				sq_pushpointer< CXMLNode* >( pVM, pRootNode );
				return 1;
			}
		}
	}

	sq_pushbool( pVM, false );
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, getRootNode)
{
	// Get the XML instance pointer
	CXML * pXML = sq_getinstance< CXML* >( pVM );

	// Is the xml instance valid?
	if( pXML )
	{
		// Is the root node valid?
		if( pXML->GetRootNode() )
		{
			// Push the root node instance
			sq_pushpointer< CXMLNode* >( pVM, pXML->GetRootNode() );
			return 1;
		}
	}

	sq_pushbool( pVM, false );
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, createNode)
{
	// Get the XML instance pointer
	CXML * pXML = sq_getinstance< CXML* >( pVM );

	// Is the xml instance valid?
	if( pXML )
	{
		// Get the XML node pointer
		CXMLNode * pNode = sq_getpointer< CXMLNode* >( pVM, 2 );

		// Is the node valid?
		if( pNode )
		{
			// Get the node name
			const char * szName;
			sq_getstring( pVM, 3, &szName );

			// Create the node
			CXMLNode * pNewNode = pNode->CreateSubNode( szName );

			// Did the new node create?
			if( pNewNode )
			{
				// Push the new node instance
				sq_pushpointer< CXMLNode* >( pVM, pNewNode );
				return 1;
			}
		}
	}

	sq_pushbool( pVM, false );
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, findNode)
{
	// Get the XML instance pointer
	CXML * pXML = sq_getinstance< CXML* >( pVM );

	// Is the xml instance valid?
	if( pXML )
	{
		// Get the XML node pointer
		CXMLNode * pNode = sq_getpointer< CXMLNode* >( pVM, 2 );

		// Is the node valid?
		if( pNode )
		{
			// Get the node name
			const char * szName;
			sq_getstring( pVM, 3, &szName );

			// Find the node
			CXMLNode * pFoundNode = pNode->FindNode( szName );

			// Did we find the node?
			if( pFoundNode )
			{
				// Push the found node instance
				sq_pushpointer< CXMLNode* >( pVM, pFoundNode );
				return 1;
			}
		}
	}

	sq_pushbool( pVM, false );
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, setNodeName)
{
	// Get the XML instance pointer
	CXML * pXML = sq_getinstance< CXML* >( pVM );

	// Is the xml instance valid?
	if( pXML )
	{
		// Get the XML node pointer
		CXMLNode * pNode = sq_getpointer< CXMLNode* >( pVM, 2 );

		// Is the node valid?
		if( pNode )
		{
			// Get the node name
			const char * szName;
			sq_getstring( pVM, 3, &szName );

			// Set the node name
			pNode->SetName( szName );
			
			sq_pushbool( pVM, true );
			return 1;
		}
	}

	sq_pushbool( pVM, false );
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, getNodeName)
{
	// Get the XML instance pointer
	CXML * pXML = sq_getinstance< CXML* >( pVM );

	// Is the xml instance valid?
	if( pXML )
	{
		// Get the XML node pointer
		CXMLNode * pNode = sq_getpointer< CXMLNode* >( pVM, 2 );

		// Is the node valid?
		if( pNode )
		{
			// Get the node name
			const char * szName = pNode->GetName();
			
			sq_pushstring( pVM, szName, strlen(szName) );
			return 1;
		}
	}

	sq_pushbool( pVM, false );
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, setNodeValue)
{
	// Get the XML instance pointer
	CXML * pXML = sq_getinstance< CXML* >( pVM );

	// Is the xml instance valid?
	if( pXML )
	{
		// Get the XML node pointer
		CXMLNode * pNode = sq_getpointer< CXMLNode* >( pVM, 2 );

		// Is the node valid?
		if( pNode )
		{
			// Get the node value
			const char * szValue;
			sq_getstring( pVM, 3, &szValue );

			// Set the node value
			pNode->SetValue( szValue );
			
			sq_pushbool( pVM, true );
			return 1;
		}
	}

	sq_pushbool( pVM, false );
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, getNodeValue)
{
	// Get the XML instance pointer
	CXML * pXML = sq_getinstance< CXML* >( pVM );

	// Is the xml instance valid?
	if( pXML )
	{
		// Get the XML node pointer
		CXMLNode * pNode = sq_getpointer< CXMLNode* >( pVM, 2 );

		// Is the node valid?
		if( pNode )
		{
			// Get the node value
			const char * szValue = pNode->GetValue();
			
			sq_pushstring( pVM, szValue, strlen(szValue) );
			return 1;
		}
	}

	sq_pushbool( pVM, false );
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, setNodeAttribute)
{
	// Get the XML instance pointer
	CXML * pXML = sq_getinstance< CXML* >( pVM );

	// Is the xml instance valid?
	if( pXML )
	{
		// Get the XML node pointer
		CXMLNode * pNode = sq_getpointer< CXMLNode* >( pVM, 2 );

		// Is the node valid?
		if( pNode )
		{
			// Get the attribute name
			const char * szName;
			sq_getstring( pVM, 3, &szName );

			// Get the attribute value
			const char * szValue;
			sq_getstring( pVM, 4, &szValue );

			// Set the node attribute
			pNode->SetAttribute( szName, szValue );
			
			sq_pushbool( pVM, true );
			return 1;
		}
	}

	sq_pushbool( pVM, false );
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, getNodeAttribute)
{
	// Get the XML instance pointer
	CXML * pXML = sq_getinstance< CXML* >( pVM );

	// Is the xml instance valid?
	if( pXML )
	{
		// Get the XML node pointer
		CXMLNode * pNode = sq_getpointer< CXMLNode* >( pVM, 2 );

		// Is the node valid?
		if( pNode )
		{
			// Get the attribute name
			const char * szName;
			sq_getstring( pVM, 3, &szName );
			
			// Get the attribute value
			const char * szValue = pNode->GetAttribute( szName );

			sq_pushstring( pVM, szValue, strlen(szValue) );
			return 1;
		}
	}

	sq_pushbool( pVM, false );
	return 1;
}