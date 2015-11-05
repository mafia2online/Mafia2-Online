/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CClientScriptingManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern		CCore			* pCore;

CClientScriptingManager::CClientScriptingManager( bool bScriptManager )
{
	m_bIsScriptManager = bScriptManager;
}

CClientScriptingManager::~CClientScriptingManager( void )
{

}

bool CClientScriptingManager::Start( String strName, String strDirectory )
{
	// Does the script already exist?
	if( Exists( strName ) )
		return false;

	bool bIsScript = false;

	// Check if this is a script
	std::string scriptName = strName.Get();
	size_t sOff = scriptName.find_last_of( "." );

	if( sOff != std::string::npos )
	{
		if( !strcmp( scriptName.substr(sOff + 1).c_str(), "nut" ) )
		{
			bIsScript = true;
		}
	}

	// Create the file checksum
	CFileChecksum fileChecksum;

	// Copy the file to the webserver
	if( !pCore->GetWebServer()->FileCopy( strDirectory, strName, &fileChecksum, bIsScript ) )
	{
		CLogFile::Printf( "Failed to copy file %s to web server!", strName.Get() );
		return false;
	}

	// Create the client script data
	SClientScript clientScript;
	clientScript.bIsScript = bIsScript;
	clientScript.strName = strName;
	clientScript.strPath = strDirectory;
	clientScript.fileChecksum = fileChecksum;

	// Insert the script
	push_back( clientScript );

	/*
	// Construct a new bitstream
	RakNet::BitStream pBitStream;

	// Write if this is the script manager
	pBitStream.Write( m_bIsScriptManager );

	// Write the script name
	pBitStream.Write( RakNet::RakString( strName.Get() ) );

	// Write the file checksum
	pBitStream.Write( (char *)&fileChecksum, sizeof(CFileChecksum) );

	// Send it to all clients
	pCore->GetNetworkModule()->Call( RPC_NEWFILE, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );*/

	return true;
}

bool CClientScriptingManager::Stop( String strName )
{
	// Loop all scripts
	for( iterator iter = begin(); iter != end(); ++ iter )
	{
		// Is this the script we're looking for ?
		if( (*iter).strName = strName )
		{
			// Construct a new bitstream
			RakNet::BitStream pBitStream;

			// Write if this is the script manager
			pBitStream.Write( (*iter).bIsScript );

			// Write the script name
			pBitStream.Write( RakNet::RakString( strName.Get() ) );

			// Send it to all clients
			pCore->GetNetworkModule()->Call( RPC_DELETEFILE, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );

			// Erase the script
			erase( iter );

			return true;
		}
	}

	return false;
}

bool CClientScriptingManager::Restart( String strName )
{
	// Does the script not exist?
	if( !Exists( strName ) )
		return false;

	// Stop the script
	Stop( strName );

	// Start the script
	return false;
}

bool CClientScriptingManager::Exists( String strName )
{
	// Loop all scripts
	for( iterator iter = begin(); iter != end(); ++ iter )
	{
		// Is this the script we're looking for ?
		if( (*iter).strName == strName )
		{
			return true;
		}
	}

	return false;
}

void CClientScriptingManager::HandlePlayerJoin( EntityId playerId )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;

	// Loop all scripts
	for( iterator iter = begin(); iter != end(); ++ iter )
	{
		// Reset the bitstream
		pBitStream.Reset( );

		// Write if this is a script
		pBitStream.Write( (*iter).bIsScript );

		// Write the script name
		pBitStream.Write( RakNet::RakString( (*iter).strName.Get() ) );

		// Write the directory
		pBitStream.Write( RakNet::RakString( (*iter).strPath.Get() ) );

		// Write the file checksum
		pBitStream.Write( (char *)&((*iter).fileChecksum), sizeof(CFileChecksum) );

		// Send it to the client
		pCore->GetNetworkModule()->Call( RPC_NEWFILE, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false );
	}
}
