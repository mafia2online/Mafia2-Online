/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CFileTransferManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"CFileTransfer.h"
#include	<thread>
#include	<mutex>

class CFileTransferManager
{

private:

	std::list < CFileTransfer* >	m_transferList;
	std::thread						m_thread;
	std::mutex						m_mutex;

	bool							m_processTransfer;

	String							m_strHost;
	unsigned short					m_usHttpPort;

	void							WorkerThread					();

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