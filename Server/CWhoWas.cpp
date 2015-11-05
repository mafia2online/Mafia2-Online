/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CWhoWas.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

std::list< SWhoWas > CWhoWas::m_whoList;

void CWhoWas::Add( String strName, String strSerial, String strSocket )
{
	// Create the struct and copy the data
	SWhoWas Entry;
	Entry.strNick = strName;
	Entry.strSerial = strSerial;
	Entry.uiTime = SharedUtility::GetTime();
	
	// Split the socket to get the ip and port
	std::vector< String > socket = strSocket.split( ':' );

	//
	Entry.strIp = socket.front();
	Entry.usPort = (unsigned short)socket.back().ToInteger();

	// Add it to our list
	m_whoList.push_front( Entry );

	// Is the list too big?
	if( m_whoList.size() > MAX_WHO_WAS )
	{
		// Pop the list back
		m_whoList.pop_back();
	}
}