/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CWhoWas.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#define	MAX_WHO_WAS						30

struct SWhoWas
{
	String strNick;
	String strSerial;
	String strIp;
	unsigned short usPort;
	unsigned int uiTime;
};

class CWhoWas
{

public:

	static	std::list< SWhoWas >				m_whoList;

	static	void								Add			( String strNick, String strSerial, String strSocket );
	static	void								Clear		( void ) { m_whoList.clear(); }
	static	unsigned int						Count		( void ) { return m_whoList.size(); }

};