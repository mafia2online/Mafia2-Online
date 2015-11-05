/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CBanManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

struct SServerBan
{
	const char * szSerial;
	const char * szBanner;
	unsigned long ulBanTime;
	unsigned long ulUnbanTime;
	const char * szReason;
};

class CBanManager
{

private:

	std::list< SServerBan *>			m_banList;
	String								m_strBanFile;
	int									m_iTotalBansLoaded;
	bool								m_bBansLoaded;
	int									m_iTotalUnbans;

	CXML								* m_pBanFile;
	CXMLNode							* m_pRootNode;

public:

	CBanManager( void );
	~CBanManager( void );

	bool								Load						( const char * szFileName );

	bool								Add							( const char * szSerial, const char * szBanner, unsigned long ulBanTime, unsigned long ulUnbanTime, const char * szReason, bool bNewNode = true );
	void								Remove						( const char * szSerial );
	bool								IsSerialBanned				( const char * szSerial );

	void								Pulse						( bool bPreLoad = false );

};