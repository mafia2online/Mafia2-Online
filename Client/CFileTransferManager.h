/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CFileTransferManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CFileTransferManager
{

private:

	std::list < CFileTransfer* >	m_transferList;
	CThread							m_workerThread;
	CMutex							m_transferListMutex;

	String							m_strHost;
	unsigned short					m_usHttpPort;

	static	void					WorkerThread					( CThread * pCreator );

public:

									CFileTransferManager			( void );
									~CFileTransferManager			( void );

	void							Add								( String strFileName, String strFilePath, CFileChecksum fileChecksum, bool bIsScript );
	bool							Remove							( String strFileName );

	void							Reset							( bool bKillThread );
	void							Pulse							( void );

	void							Complete						( void );

	void							SetServerInformation			( const char * szHost, unsigned short usHttpPort );

};