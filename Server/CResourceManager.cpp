/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CResourceManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;
int iLoaded = 0, iFailed = 0;

CResourceManager::CResourceManager( void )
{
}

CResourceManager::~CResourceManager( void )
{
}

void CResourceManager::LoadAll( std::list< const char * > resources )
{
	// Loop through all resources
	for( std::list< const char* >::iterator iter = resources.begin(); iter != resources.end(); iter++ )
	{
		// Parse the resource
		StartResource( *iter ) ? iLoaded++ : iFailed++;
	}

	// Update resource loaded totals
	pCore->UpdateResourceTotals( iLoaded, iFailed );
}

bool CResourceManager::StartResource( const char * szResource )
{
	// Is the resource already running?
	if( IsResourceRunning( szResource ) )
	{
		CLogFile::Printf( "Failed to start resource '%s'. (Resource is already running)", szResource );
		return false;
	}

	// Get the resource path
	String strPath( "resources/%s", szResource );

	// Does the path not exist?
	if( !SharedUtility::Exists( strPath.Get() ) )
	{
		CLogFile::Printf( "Failed to load resource '%s'. (Path doesn't exist)", szResource );
		return false;
	}

	// Get the meta file
	String strMeta( "%s/meta.xml", strPath.Get() );

	// Does the file not exist?
	if( !SharedUtility::Exists( strMeta.Get() ) )
	{
		CLogFile::Printf( "Failed to load resource '%s'. (Can't find meta.xml)", szResource );
		return false;
	}

	// Load the XML file
	CXML * pFile = new CXML( strMeta.Get() );

	// Is the file invalid?
	if( !pFile )
		return false;

	// Create the resource structure
	SResource resource;
	resource.strName.Set ( szResource );
	resource.strPath = strPath;
	resource.iScripts = 0;
	resource.iFiles = 0;

	// Get the root node
	CXMLNode * pRootNode = pFile->GetRootNode();

	// Is the root node invalid?
	if( !pRootNode )
	{
		// Delete the xml file
		SAFE_DELETE( pFile );
		return false;
	}

	// Get all the root node children
	std::list< CXMLNode* > children = pRootNode->GetChildren();

	// Loop all the children in the root node
	for( std::list< CXMLNode* >::iterator iter = children.begin(); iter != children.end(); iter++ )
	{
		// Get a pointer to the current node
		CXMLNode * pCurNode = *iter;

		// Is the current node valid?
		if( pCurNode )
		{
			// Get the current node type
			const char * szNodeType = pCurNode->GetName();

			// Get the current node data
			const char * szNodeData = pCurNode->GetValue();

			// Is this node a script?
			if( !strcmp( szNodeType, "script" ) )
			{
				// Create the script struct
				SScript script;

				// Set the script name
				script.strName.Set ( szNodeData );

				// Set the script type
				const char * szScriptType = pCurNode->GetAttribute( "type" );

				// Update the path
				strPath.Format( "resources/%s/%s", szResource, szScriptType  );

				// Is this a client script?
				if( !strcmp( szScriptType, "client" ) )
				{
					// Load the clientscript
					if( !pCore->GetClientScriptingManager()->Start( szNodeData, strPath ) )
						continue;

					// Set the script type
					script.eType = E_SCRIPT_CLIENT;

					// Increase the resource script count
					resource.iScripts++;
				}
				else if( !strcmp( szScriptType, "server" ) )
				{
					// Load the script
					CSquirrel * pScript = pCore->GetScriptingManager()->Load( szNodeData, String( "%s/%s", strPath.Get(), szNodeData ) );

					// Did the script fail to load?
					if( !pScript )
						continue;

					// Set the script type
					script.eType = E_SCRIPT_SERVER;

					// Increase the resource script count
					resource.iScripts++;
				}
				else
				{
					SAFE_DELETE( pFile );
					CLogFile::Printf( "WARNING: Unknown script type '%s' in resource '%s'.", szScriptType, szResource );
					continue;
				}

				// Add this script to the resource's script list
				resource.scripts.push_back( script );
			}
			else if( !strcmp( szNodeType, "file" ) )
			{
				// Update the path
				strPath.Format( "resources/%s/files", szResource );

				// Load the file with the client scripting manager
				if( !pCore->GetClientScriptingManager()->Start( szNodeData, strPath ) )
					continue;

				// Increase the resource file count
				resource.iFiles++;
			}
		}
	}

	// Add this to the resource list
	m_resourceList.push_back( resource );

	return true;
}

bool CResourceManager::StopResource( const char * szResource )
{
	// Is the resource not running?
	if( !IsResourceRunning( szResource ) )
	{
		CLogFile::Printf( "Failed to stop resource '%s'. (Resource isn't running)", szResource );
		return false;
	}

	// Loop through the resource list
	for( std::list< SResource >::iterator iter = m_resourceList.begin(); iter != m_resourceList.end(); )
	{
		// Is the current resource this resource?
		if ( !(*iter).strName.Compare ( szResource ) )
		{
			// Loop through the resources script list
			for( std::list< SScript >::iterator iter2 = (*iter).scripts.begin(); iter2 != (*iter).scripts.end(); iter2++ )
			{
				if( (*iter2).eType == E_SCRIPT_CLIENT )
				{
					// Stop the client script
					pCore->GetClientScriptingManager()->Stop( (*iter2).strName );
				}
				else
				{
					// Stop the server script
					pCore->GetScriptingManager()->Unload( (*iter2).strName );
				}
			}

			// Remove this resource from the list
			m_resourceList.erase( iter );
			break;
		}
		else
			iter++;
	}

	return true;
}

bool CResourceManager::IsResourceRunning( const char * szResource )
{
	// Loop through the resource list
	for( std::list< SResource >::iterator iter = m_resourceList.begin(); iter != m_resourceList.end(); iter++ )
	{
		// Is the current resource this resource?
		if ( !(*iter).strName.Compare ( szResource ) )
		{
			return true;
		}
	}

	return false;
}

void CResourceManager::GetRunningResources( void )
{
	// Loop through the resource list
	for( std::list< SResource >::iterator iter = m_resourceList.begin(); iter != m_resourceList.end(); iter++ )
	{
		CLogFile::Printf( "= %s (%d scripts, %d files)", (*iter).strName.Get(), (*iter).iScripts, (*iter).iFiles );
	}
}