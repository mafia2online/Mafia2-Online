/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CFileTransferManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include "CCore.h"

#include "CGUI.h"
#include "CDownloadProgress.h"
#include "CMafia.h"
#include "CClientScriptingManager.h"

#include "Threading/CThread.h"

#include "CFileTransfer.h"

class CFileTransferManager
{

private:

	std::list < CFileTransfer* >	m_transferList;
	CThread							m_workerThread;
	CMutex							m_transferListMutex;

	String							m_strHost;
	unsigned short					m_usHttpPort;

	static	void					WorkerThread(CThread * pCreator);

public:

	CFileTransferManager(void);
	~CFileTransferManager(void);

	void							Add(String strFileName, String strFilePath, CFileChecksum fileChecksum, bool bIsScript);

	void							Reset(bool bKillThread);
	void							Pulse(void);

	void							Complete(void);

	void							SetServerInformation(const char * szHost, unsigned short usHttpPort);

};