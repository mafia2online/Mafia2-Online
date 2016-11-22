/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CFileTransferManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	<list>

#include "StdInc.h"

#include "CFileTransferManager.h"

void CFileTransferManager::WorkerThread(CThread * pCreator)
{
	CFileTransferManager * pFileTransferManager = pCreator->GetUserData < CFileTransferManager* >();

	pFileTransferManager->m_transferListMutex.Lock();
	for (std::list< CFileTransfer* >::iterator iter = pFileTransferManager->m_transferList.begin(); iter != pFileTransferManager->m_transferList.end(); iter++)
	{
		pFileTransferManager->m_transferListMutex.Unlock();
		(*iter)->Pulse(pFileTransferManager->m_strHost, pFileTransferManager->m_usHttpPort);
		pFileTransferManager->m_transferListMutex.Lock();
	}
	pFileTransferManager->m_transferListMutex.Unlock();
}

CFileTransferManager::CFileTransferManager(void) :
	m_strHost(""),
	m_usHttpPort(0)
{
	m_workerThread.SetUserData < CFileTransferManager* >(this);
	if (!SharedUtility::Exists("cache"))
		SharedUtility::CreateDirectory("cache");
}

CFileTransferManager::~CFileTransferManager(void)
{
	if (m_workerThread.IsRunning())
	{
		m_workerThread.Stop();
	}
	Reset(false);
}

void CFileTransferManager::Add(String strFileName, String strFileType, CFileChecksum fileChecksum, bool bIsScript)
{
	CFileTransfer * pFileTransfer = new CFileTransfer(strFileName, fileChecksum, bIsScript);
	if (!pFileTransfer)
		return;

	m_transferListMutex.Lock();
	m_transferList.push_back(pFileTransfer);
	m_transferListMutex.Unlock();
}

void CFileTransferManager::Reset(bool bKillThread)
{
	if (bKillThread && m_workerThread.IsRunning())
	{
		m_workerThread.Stop(false, true);
	}
	for (std::list < CFileTransfer* >::iterator iter = m_transferList.begin(); iter != m_transferList.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_transferList.clear();
}

void CFileTransferManager::Pulse(void)
{
	if (m_workerThread.IsRunning())
		return;

	for (std::list < CFileTransfer* >::iterator iter = m_transferList.begin(); iter != m_transferList.end(); iter++)
	{
		if ((*iter)->IsComplete())
		{
			if ((*iter)->HasSucceeded())
			{
				if ((*iter)->IsScript())
				{
					CCore::Instance()->GetClientScriptingManager()->AddScript((*iter)->m_strFileName, SharedUtility::GetAbsolutePath("%s%s", SharedUtility::GetClientScriptFolder(CCore::Instance()->GetHost(), CCore::Instance()->GetPort()).Get(), (*iter)->m_strFileName.Get()));
					CCore::Instance()->GetClientScriptingManager()->Load((*iter)->m_strFileName.Get());
				}
			}

			m_transferList.remove(*iter);
			iter = m_transferList.begin();
			if (iter == m_transferList.end())
			{
				Complete();
				break;
			}
		}
	}
}

void CFileTransferManager::Complete(void)
{
	Reset(true);
	CCore::Instance()->GetGUI()->GetDownloadProgress()->SetVisible(false);
	CCore::Instance()->GetGame()->Spawn();
}

void CFileTransferManager::SetServerInformation(const char * szHost, unsigned short usHttpPort)
{
	m_strHost.Set(szHost);
	m_usHttpPort = usHttpPort;

	if (m_transferList.size() == 0)
	{
		CCore::Instance()->GetGUI()->GetDownloadProgress()->SetVisible(false);
		CCore::Instance()->GetGame()->Spawn();
		return;
	}

	if (!m_workerThread.IsRunning())
	{
		m_workerThread.Start(WorkerThread);
	}
}