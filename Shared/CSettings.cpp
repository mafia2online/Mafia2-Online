/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSettings.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "CSettings.h"
#include "SharedUtility.h"
#include "CLogFile.h"

CXML								* CSettings::m_pXML = NULL;
CXMLNode							* CSettings::m_pRootNode = NULL;
bool								CSettings::m_bOpen = false;

bool CSettings::Load( const char * szFileName )
{
	// Create the xml file instance
	m_pXML = new CXML( szFileName );

	// Is the xml instance valid?
	if( m_pXML )
	{
		// Mark as file open
		m_bOpen = true;

		// Get the root node
		m_pRootNode = m_pXML->GetRootNode();

		// Is the root node invalid?
		if( !m_pRootNode )
			m_pRootNode = m_pXML->CreateRootNode( "settings" );

		// Load the default variables
		LoadDefaults( );

		// Save the file
		Save();
	}

	return true;
}

bool CSettings::Save( void )
{
	// Don't we have a valid xml instance?
	if( !m_pXML )
		return false;

	// Save the file
	return m_pXML->Save();
}

void CSettings::LoadDefaults( void )
{
	#define DEFAULT(__x,__y)				if(!Exists(__x)) \
											Set(__x, __y)

#ifdef _CLIENT
	// Default client settings
	DEFAULT( "nick", "Player" );
	DEFAULT( "auto-updates", true );
	DEFAULT( "multisample-anti-aliasing", 0 );
	DEFAULT( "anisotropic-filtering", 0 );
	DEFAULT( "ambient-occlusion", false );
	DEFAULT( "audio-quality", 0 );
	DEFAULT( "sfx-volume", 100 );
	DEFAULT( "voice-volume", 100 );
	DEFAULT( "music-volume", 100 );
	DEFAULT( "radio-volume", 100 );
#else
	// Default server settings
	DEFAULT( "hostname", "Default "MOD_NAME" "MOD_VERS_STR" Server" );
	DEFAULT( "serverip", "" );
	DEFAULT( "port", 27015 );
	DEFAULT( "maxplayers", MAX_PLAYERS );
	DEFAULT( "weburl", "www.m2-multiplayer.com" );
	DEFAULT( "password", "" );
	DEFAULT( "modules", "" );
	DEFAULT( "resources", "" );
#endif
}

bool CSettings::Exists( const char * szName )
{
	// Don't we have a root node?
	if( !m_pRootNode )
		return false;

	return (m_pRootNode->FindNode( szName ) != NULL);
}

void CSettings::ParseCommandLine( int argc, char ** argv )
{
	// Loop over all arguments
	for( int i = 0; i < argc; i++ )
	{
		// Is the current character a '-' ?
		if( argv[i][0] == '-' )
		{
			// Is there a value?
			if( (i + 1) < argc )
			{
				// Get the name and value strings
				const char * szName = (argv[i] + 1);
				const char * szValue = argv[i + 1];

				// Set the cvar
				Set( szName, szValue );
			}
		}
	}
}

void CSettings::ParseCommandLine( char * szCommandLine )
{
	// Loop until we reach the end of the command line string
	while( *szCommandLine )
	{
		// Is the current character not a space?
		if( !isspace( *szCommandLine ) )
		{
			// Is the current character a '-' ?
			if( *szCommandLine == '-' )
			{
				// Skip the '-' character
				szCommandLine++;

				// Get the name string
				const char * szName = "";
				while( *szCommandLine && !isspace( *szCommandLine ) )
				{
					szName += *szCommandLine;
					szCommandLine++;
				}

				// Have we run out of command line to process?
				if( !(*szCommandLine) )
					break;

				// Skip the space between the name and the value
				while( *szCommandLine && isspace( *szCommandLine ) )
					szCommandLine++;

				// Have we run out of command line to process?
				if( !(*szCommandLine) )
					break;

				// Get the value string
				const char * szValue = "";
				while( *szCommandLine && !isspace( *szCommandLine ) )
				{
					szValue += *szCommandLine;
					szCommandLine++;
				}

				// Set the cvar
				Set( szName, szValue );

				// Have we run out of command line to process?
				if( !(*szCommandLine) )
					break;
			}
		}

		// Increase the command line string
		szCommandLine++;
	}
}

void CSettings::Set( const char * szName, const char * szValue )
{
	// Get the root node
	CXMLNode * pRootNode = m_pXML->GetRootNode();

	// Is the root node valid?
	if( pRootNode )
	{
		// Find the sub node
		CXMLNode * pSubNode = pRootNode->FindNode( szName );

		// Did we fail to find the sub node?
		if( !pSubNode )
			pSubNode = pRootNode->CreateSubNode( szName );

		// Set the value
		pSubNode->SetValue( szValue );
	}

	// Save the XML file
	Save();
}

void CSettings::Set( const char * szName, bool bValue )
{
	Set( szName, String( "%d", (int)bValue ).Get() );
}

void CSettings::Set( const char * szName, int iValue )
{
	Set( szName, String( "%i", iValue ).Get() );
}

void CSettings::Set( const char * szName, unsigned int uiValue )
{
	Set( szName, String( "%u", uiValue ).Get() );
}

void CSettings::Set( const char * szName, float fValue )
{
	Set( szName, String( "%f", fValue ).Get() );
}

void CSettings::Set( const char * szNode, const char * szSubNode, std::list< const char* > list )
{
	// Get the root node
	CXMLNode * pRootNode = m_pXML->GetRootNode();

	// Is the root node valid?
	if( pRootNode )
	{
		// Find the sub node
		CXMLNode * pSubNode = pRootNode->FindNode( szNode );

		// Did we fail to find the sub node?
		if( !pSubNode )
			pSubNode = pRootNode->CreateSubNode( szNode );

		// Loop over the list
		for( std::list< const char* >::iterator iter = list.begin(); iter != list.end(); iter++ )
		{
			// Create the sub node and set the value
			pSubNode->CreateSubNode( szSubNode )->SetValue( *iter );
		}
	}

	// Save the XML file
	Save();
}

void CSettings::Get( const char * szName, const char ** szValue )
{
	// Get the root node
	CXMLNode * pRootNode = m_pXML->GetRootNode();

	// Is the root node valid?
	if( pRootNode )
	{
		// Find the sub node
		CXMLNode * pSubNode = pRootNode->FindNode( szName );

		// Did we find the sub node?
		if( pSubNode && pSubNode->GetValue() != NULL )
		{
			// Get the value
			*szValue = pSubNode->GetValue();
			return;
		}
	}

	// Reset the value
	*szValue = "";
}

void CSettings::Get( const char * szName, bool * bValue )
{
	const char * szValue;
	Get( szName, &szValue );

	if( szValue )
	{
		if( !strcmp( szValue, "true" ) || !strcmp( szValue, "1" ) )
		{
			*bValue = true;
			return;
		}
	}

	*bValue = false;
}

void CSettings::Get( const char * szName, int * iValue )
{
	const char * szValue;
	Get( szName, &szValue );

	if( szValue )
	{
		*iValue = atoi( szValue );
		return;
	}

	*iValue = 0;
}

void CSettings::Get( const char * szName, unsigned int * uiValue )
{
	const char * szValue;
	Get( szName, &szValue );

	if( szValue )
	{
		*uiValue = (unsigned int)atoi( szValue );
		return;
	}

	*uiValue = 0;
}

void CSettings::Get( const char * szName, float * fValue )
{
	const char * szValue;
	Get( szName, &szValue );

	if( szValue )
	{
		*fValue = (float)atof( szValue );
		return;
	}

	*fValue = 0.0f;
}

void CSettings::Get( const char * szNode, const char * szSubNode, std::list< const char* > * list )
{
	// Get the root node
	CXMLNode * pRootNode = m_pXML->GetRootNode();

	// Is the root node valid?
	if( pRootNode )
	{
		// Find the sub node
		CXMLNode * pSubNode = pRootNode->FindNode( szNode );

		// Did we fail to find the sub node?
		if( !pSubNode )
			return;

		// Get all the children of the subnode
		std::list< CXMLNode* > children = pSubNode->GetChildren();

		// Loop over all the children
		for( std::list< CXMLNode* >::iterator iter = children.begin(); iter != children.end(); iter++ )
		{
			// Get a pointer to the current node
			CXMLNode * pCurNode = *iter;

			// Is the current node valid?
			if( pCurNode )
			{
				// Is this node what we're looking for?
				if( !strcmp( pCurNode->GetName(), szSubNode ) )
				{
					// Push the current node value into the list
					list->push_back( pCurNode->GetValue() );
				}
			}
		}
	}
}