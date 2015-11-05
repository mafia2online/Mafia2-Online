/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CFileTransferManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore				* pCore;

void CFileTransferManager::WorkerThread ( CThread * pCreator )
{
	// Get the file transfer manager singleton
	CFileTransferManager * pFileTransferManager = pCreator->GetUserData < CFileTransferManager* > ();

	// Lock the transfer list mutex
	pFileTransferManager->m_transferListMutex.Lock ();

	// Loop through the transfer list
	for ( std::list< CFileTransfer* >::iterator iter = pFileTransferManager->m_transferList.begin (); iter != pFileTransferManager->m_transferList.end (); iter ++ )
	{
		// Unlock the transfer list mutex
		pFileTransferManager->m_transferListMutex.Unlock ();

		// Pulse the current file
		(*iter)->Pulse ( pFileTransferManager->m_strHost, pFileTransferManager->m_usHttpPort );

		// Lock the transfer list mutex
		pFileTransferManager->m_transferListMutex.Lock ();
	}

	// Unlock the transfer list mutex
	pFileTransferManager->m_transferListMutex.Unlock ();
}

CFileTransferManager::CFileTransferManager ( void ) :
	m_strHost ( "" ),
	m_usHttpPort ( 0 )
{
	// Set our thread user data
	m_workerThread.SetUserData < CFileTransferManager* > ( this );

	// Create the cache folder if it doesn't exists
	if ( !SharedUtility::Exists ( "cache" ) )
		SharedUtility::CreateDirectory ( "cache" );
}

CFileTransferManager::~CFileTransferManager ( void )
{
	// Is the worker thread running?
	if ( m_workerThread.IsRunning () )
	{
		// Stop the worker thread
		m_workerThread.Stop ();
	}

	// Reset
	Reset ( false );
}

void CFileTransferManager::Add ( String strFileName, String strFileType, CFileChecksum fileChecksum, bool bIsScript )
{
	// Create the file transfer pointer
	CFileTransfer * pFileTransfer = new CFileTransfer ( strFileName, fileChecksum, bIsScript );

	// Did the file transfer instance fail to create?
	if ( !pFileTransfer )
		return;

	// Lock the transfer list mutex
	m_transferListMutex.Lock ();

	// Add the file transfer to the transfer list
	m_transferList.push_back ( pFileTransfer );

	// Unlock the transfer list mutex
	m_transferListMutex.Unlock ();
}

// todo: Remove this?
bool CFileTransferManager::Remove ( String strFileName )
{
	return false;
}

void CFileTransferManager::Reset ( bool bKillThread )
{
	// Is the worker thread running?
	if ( bKillThread && m_workerThread.IsRunning () )
	{
		// Kill the worker thread
		m_workerThread.Stop ( false, true );
	}

	// Loop through the transfer list
	for ( std::list < CFileTransfer* >::iterator iter = m_transferList.begin (); iter != m_transferList.end (); iter ++ )
	{
		// Delete this transfer item
		SAFE_DELETE ( *iter );
	}

	// Clear the transfer list
	m_transferList.clear ();
}

void CFileTransferManager::Pulse ( void )
{
	// Is the worker thread running?
	if ( m_workerThread.IsRunning () )
		return;

	// Loop over each file transfer
	for ( std::list < CFileTransfer* >::iterator iter = m_transferList.begin (); iter != m_transferList.end (); iter ++ )
	{
		// Is the current transfer complete?
		if ( (*iter)->IsComplete () )
		{
			// Has the download succeeded?
			if ( (*iter)->HasSucceeded () )
			{
				// Is this transfer a script?
				if ( (*iter)->IsScript () )
				{
					// Add the client script to the client scripting manager
					pCore->GetClientScriptingManager()->AddScript ( (*iter)->m_strFileName, SharedUtility::GetAbsolutePath ( "%s%s", SharedUtility::GetClientScriptFolder ( pCore->GetHost (), pCore->GetPort () ).Get (), (*iter)->m_strFileName.Get () ) );

					// Load the client script
					pCore->GetClientScriptingManager()->Load((*iter)->m_strFileName.Get());
				}
			}

			// Remove the current item from the transfer list
			m_transferList.remove ( *iter );

			// Start from the begining again
			iter = m_transferList.begin ();

			// Are we done?
			if ( iter == m_transferList.end () )
			{
				// Done
				Complete ();

				break;
			}
		}
	}
}

void CFileTransferManager::Complete ( void )
{
	// Reset everything
	Reset ( true );

	// Hide the download progress bar
	pCore->GetGUI()->GetDownloadProgress()->SetVisible ( false );

	// Start the game
	pCore->GetGame()->Spawn ();
}

void CFileTransferManager::SetServerInformation ( const char * szHost, unsigned short usHttpPort )
{
	// Set the server information
	m_strHost.Set ( szHost );
	m_usHttpPort = usHttpPort;

	// Start the worker thread
	if ( !m_workerThread.IsRunning () )
	{
		// Start the worker thread
		m_workerThread.Start ( WorkerThread );
	}
}