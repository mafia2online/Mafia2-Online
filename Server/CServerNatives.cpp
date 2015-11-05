/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CServerNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"CServerNatives.h"

extern CCore		* pCore;

void CServerNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "setGameModeText", SetGameModeText, 1, "s" );
	pScriptingManager->RegisterFunction( "setMapName", SetMapName, 1, "s" );
	pScriptingManager->RegisterFunction( "setWeather", SetWeather, 1, "s" );
	pScriptingManager->RegisterFunction( "setSummer", SetSummer, 1, "b" );
	pScriptingManager->RegisterFunction( "getNetworkStats", GetNetworkStats, 0, NULL );
	pScriptingManager->RegisterFunction( "getWhoWas", GetWhoWasList, 0, NULL );
	pScriptingManager->RegisterFunction( "clearWhoWas", ClearWhoWas, 0, NULL );
	pScriptingManager->RegisterFunction( "getWhoWasCount", GetWhoWasCount, 0, NULL );
	pScriptingManager->RegisterFunction( "shutdown", Shutdown, 0, NULL );
}

SQInteger CServerNatives::SetGameModeText( SQVM * pVM )
{
	// Get the gamemode text
	const SQChar * szGamemode;
	sq_getstring( pVM, -1, &szGamemode );

	// Store the gamemode text
	pCore->SetGameModeText( szGamemode );

	sq_pushbool( pVM, true );
	return 1;
}

SQInteger CServerNatives::SetMapName( SQVM * pVM )
{
	// Get the map name
	const SQChar * szMap;
	sq_getstring( pVM, -1, &szMap );

	// Store the map name
	pCore->SetMapName( szMap );

	sq_pushbool( pVM, true );
	return 1;
}

SQInteger CServerNatives::SetWeather( SQVM * pVM )
{
	// Get the weather
	const SQChar * szWeather;
	sq_getstring( pVM, -1, &szWeather );

	// Set the weather
	pCore->SetWeather( szWeather );

	// Construct a new bitstream
	RakNet::BitStream pBitStream;

	// Write the weather string
	pBitStream.Write( RakNet::RakString( szWeather ) );

	// Send to all clients
	pCore->GetNetworkModule()->Call( RPC_SETPLAYERWEATHER, &pBitStream, HIGH_PRIORITY, RELIABLE, INVALID_ENTITY_ID, true );

	sq_pushbool( pVM, true );
	return 1;
}

SQInteger CServerNatives::SetSummer( SQVM * pVM )
{
	// Get the toggle
	SQBool bSummer;
	sq_getbool( pVM, -1, &bSummer );

	// Set the server season
	pCore->SetSummer( bSummer );

	sq_pushbool( pVM, true );
	return 1;
}

SQInteger CServerNatives::GetNetworkStats( SQVM * pVM )
{
	/*
	unsigned int uiBytesPerSecondReceived, uiBytesPerSecondSent, uiTotalBytesReceived, uiTotalBytesSent, uiConnectionTime;
	float fCurrentPacketLoss, fAveragePacketLoss;

	// Get the player network stats
	CNetworkStats::GetStats( &uiBytesPerSecondReceived, &uiBytesPerSecondSent, &uiTotalBytesReceived, &uiTotalBytesSent, &uiConnectionTime, &fCurrentPacketLoss, &fAveragePacketLoss );

	// Create a new table
	sq_newtable( pVM );

	sq_pushstring( pVM, "BytesPerSecondReceived", strlen("BytesPerSecondReceived") );
	sq_pushinteger( pVM, uiBytesPerSecondReceived );
	sq_createslot( pVM, -3 );

	sq_pushstring( pVM, "BytesPerSecondSent", strlen("BytesPerSecondSent") );
	sq_pushinteger( pVM, uiBytesPerSecondSent );
	sq_createslot( pVM, -3 );

	sq_pushstring( pVM, "TotalBytesReceived", strlen("TotalBytesReceived") );
	sq_pushinteger( pVM, uiTotalBytesReceived );
	sq_createslot( pVM, -3 );

	sq_pushstring( pVM, "TotalBytesSent", strlen("TotalBytesSent") );
	sq_pushinteger( pVM, uiTotalBytesSent );
	sq_createslot( pVM, -3 );

	sq_pushstring( pVM, "CurrentPacketLoss", strlen("CurrentPacketLoss") );
	sq_pushfloat( pVM, fCurrentPacketLoss );
	sq_createslot( pVM, -3 );

	sq_pushstring( pVM, "AveragePacketLoss", strlen("AveragePacketLoss") );
	sq_pushfloat( pVM, fAveragePacketLoss );
	sq_createslot( pVM, -3 );

	sq_pushstring( pVM, "ConnectionTime", strlen("ConnectionTime") );
	sq_pushinteger( pVM, uiConnectionTime );
	sq_createslot( pVM, -3 );
	*/

	return 1;
}

SQInteger CServerNatives::GetWhoWasList( SQVM * pVM )
{
	// Create a new array
	sq_newarray( pVM, 0 );

	// Loop over each who was item
	for( std::list< SWhoWas >::iterator iter = CWhoWas::m_whoList.begin(); iter != CWhoWas::m_whoList.end(); iter++ )
	{
		// Create a new table
		sq_newtable( pVM );

		// Push the name
		sq_pushstring( pVM, "name", strlen("name") );
		sq_pushstring( pVM, (*iter).strNick.Get(), (*iter).strNick.GetLength() );
		sq_createslot( pVM, -3 );

		// Push the serial
		sq_pushstring( pVM, "serial", strlen("serial") );
		sq_pushstring( pVM, (*iter).strSerial.Get(), (*iter).strSerial.GetLength() );
		sq_createslot( pVM, -3 );

		// Push the ip
		sq_pushstring( pVM, "ip", strlen("ip") );
		sq_pushstring( pVM, (*iter).strIp.Get(), (*iter).strIp.GetLength() );
		sq_createslot( pVM, -3 );

		// Push the port
		sq_pushstring( pVM, "port", strlen("port") );
		sq_pushinteger( pVM, (*iter).usPort );
		sq_createslot( pVM, -3 );

		// Push the disconnection time
		sq_pushstring( pVM, "time", strlen("time") );
		sq_pushinteger( pVM, (*iter).uiTime );
		sq_createslot( pVM, -3 );

		// Append the table to the array
		sq_arrayappend( pVM, -2 );
	}
	return 1;
}

SQInteger CServerNatives::ClearWhoWas( SQVM * pVM )
{
	// Clear the who was list
	CWhoWas::Clear();

	sq_pushbool( pVM, true );
	return 1;
}

SQInteger CServerNatives::GetWhoWasCount( SQVM * pVM )
{
	// Get the who was count
	sq_pushinteger( pVM, CWhoWas::Count() );
	return 1;
}

SQInteger CServerNatives::Shutdown( SQVM * pVM )
{
	sq_pushbool( pVM, true );

	// Shutdown the server
	pCore->SetActive ( false );
	return 1;
}