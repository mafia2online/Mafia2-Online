/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CFileTransferManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"BaseInc.h"

#include	"CCore.h"

#include	"CString.h"
#include	<Threading/CThread.h>

#include	"CGUI.h"
#include	"CMafia.h"
#include	"CDownloadProgress.h"

#include	"SharedUtility.h"

#include	"CClientScriptingManager.h"

#include	"CRC.h"

#include	"CFileTransfer.h"
#include	"CFileTransferManager.h"

void CFileTransferManager::WorkerThread()
{
	if (m_mutex.try_lock()){
		for ( std::list< CFileTransfer* >::iterator iter = m_transferList.begin (); iter != m_transferList.end (); iter ++ )
		{
			m_mutex.unlock ();

			(*iter)->Pulse ( m_strHost, m_usHttpPort );
			m_mutex.lock ();
		}
		m_mutex.unlock ();
	}
}

CFileTransferManager::CFileTransferManager ( void ) :
	m_strHost ( "" ),
	m_usHttpPort ( 0 )
{
	if ( !SharedUtility::Exists ( "cache" ) )
		SharedUtility::CreateDirectory ( "cache" );
}

CFileTransferManager::~CFileTransferManager ( void )
{
	m_processTransfer = false;
	Reset ( false );
}

void CFileTransferManager::Add ( String strFileName, String strFileType, CFileChecksum fileChecksum, bool bIsScript )
{
	CFileTransfer * pFileTransfer = new CFileTransfer ( strFileName, fileChecksum, bIsScript );
	if ( !pFileTransfer )
		return;

	if (m_mutex.try_lock()){
		m_transferList.push_back(pFileTransfer);
		m_mutex.unlock();
	}
}

// todo: Remove this?
bool CFileTransferManager::Remove ( String strFileName )
{
	return false;
}

void CFileTransferManager::Reset ( bool bKillThread )
{
	if ( bKillThread )
	{
		m_processTransfer = false;
	}
	for ( std::list < CFileTransfer* >::iterator iter = m_transferList.begin (); iter != m_transferList.end (); iter ++ )
	{
		SAFE_DELETE ( *iter );
	}
	m_transferList.clear ();
}

void CFileTransferManager::Pulse ( void )
{
	if ( m_thread.joinable () )
		return;

	for ( std::list < CFileTransfer* >::iterator iter = m_transferList.begin (); iter != m_transferList.end (); iter ++ )
	{
		if ( (*iter)->IsComplete () )
		{
			if ( (*iter)->HasSucceeded () )
			{
				if ( (*iter)->IsScript () )
				{
					CCore::Instance()->GetClientScriptingManager()->AddScript((*iter)->m_strFileName, SharedUtility::GetAbsolutePath("%s%s", SharedUtility::GetClientScriptFolder(CCore::Instance()->GetHost(), CCore::Instance()->GetPort()).Get(), (*iter)->m_strFileName.Get()));
					CCore::Instance()->GetClientScriptingManager()->Load((*iter)->m_strFileName.Get());
				}
			}
			m_transferList.remove ( *iter );
			iter = m_transferList.begin ();
			if ( iter == m_transferList.end () )
			{
				Complete ();
				break;
			}
		}
	}
}

void CFileTransferManager::Complete ()
{
	Reset ( true );
	CCore::Instance()->GetGUI()->GetDownloadProgress()->SetVisible(false);
	CCore::Instance()->GetGame()->Spawn();
}

void CFileTransferManager::SetServerInformation ( const char * szHost, unsigned short usHttpPort )
{
	m_strHost.Set ( szHost );
	m_usHttpPort = usHttpPort;

	if (!m_thread.joinable() && !m_processTransfer){
		m_thread = std::thread(&CFileTransferManager::WorkerThread, this);
		m_processTransfer = true;
	}
}