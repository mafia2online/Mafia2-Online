/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CBanManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;

CBanManager::CBanManager( void )
{
	// Reset variables
	m_iTotalBansLoaded = 0;
	m_bBansLoaded = false;
	m_iTotalUnbans = 0;
	m_pBanFile = NULL;
	m_pRootNode = NULL;
}

CBanManager::~CBanManager( void )
{
	// Reset
	m_bBansLoaded = false;

	// Clear the ban list
	m_banList.clear();

	// Delete the XML ban file instance
	SAFE_DELETE( m_pBanFile );
}

bool CBanManager::Load( const char * szFileName )
{
	bool bParse = true;

	// Create the xml ban file instance
	m_pBanFile = new CXML( szFileName );

	// Get the root node
	m_pRootNode = m_pBanFile->GetRootNode();

	// Is the root node invalid?
	if( !m_pRootNode )
	{
		// Create the root node
		m_pRootNode = m_pBanFile->CreateRootNode( "bans" );

		// Save the file
		m_pBanFile->Save();

		// Mark as no need to parse
		bParse = false;
	}

	// Should we parse the file?
	if( bParse )
	{
		//
		CXMLNode * pNode = NULL;
		unsigned int uiCount = m_pRootNode->GetChildCount();

		// Is there no bans?
		if( uiCount == 0 )
			return true;

		// Loop over all children
		for( unsigned int i = 0; i < uiCount; i++ )
		{
			// Get the current node
			pNode = m_pRootNode->GetNode( i );

			// Is the node invalid?
			if( !pNode )
				continue;

			// Get the current node name
			const char * szName = pNode->GetName();

			// Is this a ban?
			if( !strcmp( szName, "ban" ) )
			{
				const char *szSerial = "", *szBanner = "", *szReason = "";
				unsigned long ulBanTime, ulUnbanTime;

				// Get the ban data
				szSerial = pNode->GetValue();
				szBanner = pNode->GetAttribute( "banner" );
				ulBanTime = (unsigned long)atol( pNode->GetAttribute( "time" ) );
				ulUnbanTime = (unsigned long)atol( pNode->GetAttribute( "unban" ) );
				szReason = pNode->GetAttribute( "reason" );

				// Add the ban to the manager
				Add( szSerial, szBanner, ulBanTime, ulUnbanTime, szReason, false );

				// Increase the total loaded bans
				m_iTotalBansLoaded++;
			}
		}

		// Mark as bans loaded
		m_bBansLoaded = true;

		// Pulse the ban manager
		Pulse( true );

		// Were any bans loaded?
		if( m_iTotalBansLoaded > 0 )
		{
			// Subtract the total purged bans from the total loaded.
			m_iTotalBansLoaded -= m_iTotalUnbans;

			// Did we purge ALL bans?
			if( m_iTotalBansLoaded == 0 )
				CLogFile::Printf( "Successfully purged %d bans.", m_iTotalUnbans );
			else
				CLogFile::Printf( "Successfully loaded %d bans. (%d purged)", m_iTotalBansLoaded, m_iTotalUnbans );

			// Reset the total unban count
			m_iTotalUnbans = 0;
		}

		return true;
	}

	return false;
}

bool CBanManager::Add( const char * szSerial, const char * szBanner, unsigned long ulBanTime, unsigned long ulUnbanTime, const char * szReason, bool bNewNode )
{
	// Is this serial already banned?
	if( IsSerialBanned( szSerial ) )
	{
		CLogFile::Print( "Failed to add ban to file. (Serial is already banned!)" );
		return false;
	}

	// Should we create a new node?
	if( bNewNode )
	{
		// Do we not have the root node?
		if( !m_pRootNode )
			m_pRootNode = m_pBanFile->GetRootNode();

		// Create a new node
		CXMLNode * pBanNode = m_pRootNode->CreateSubNode( "ban" );
		pBanNode->SetAttribute( "banner", szBanner );
		pBanNode->SetAttribute( "time", String( "%i", ulBanTime ).Get() );
		pBanNode->SetAttribute( "unban", String( "%i", ulUnbanTime ).Get() );
		pBanNode->SetAttribute( "reason", szReason );
		pBanNode->SetValue( szSerial );

		// Save the ban XML file
		m_pBanFile->Save();
	}

	// Create the ban instance
	SServerBan * pServerBan = new SServerBan;
	pServerBan->szSerial = szSerial;
	pServerBan->szBanner = szBanner;
	pServerBan->ulBanTime = ulBanTime;
	pServerBan->ulUnbanTime = ulUnbanTime;
	pServerBan->szReason = szReason;

	// Add the ban into the banlist
	m_banList.push_back( pServerBan );

	// Get the player from the serial
	CNetworkPlayer * pPlayer = pCore->GetPlayerManager()->GetPlayerFromSerial( szSerial );

	// Ensure the player is valid
	if( pPlayer )
	{
		// Kick the player from the server
		pPlayer->Kick();
	}

	return true;
}

void CBanManager::Remove( const char * szSerial )
{
	// Loop through the ban list
	for( std::list< SServerBan *>::iterator iter = m_banList.begin(); iter != m_banList.end(); iter++ )
	{
		// Is this the ban we're looking for?
		if( !strcmp( (*iter)->szSerial, szSerial ) )
		{
			// Unban
			(*iter)->ulBanTime = 0;
			(*iter)->ulUnbanTime = 0;

			break;
		}
	}
}

bool CBanManager::IsSerialBanned( const char * szSerial )
{
	// Loop through the ban list
	for( std::list< SServerBan *>::iterator iter = m_banList.begin(); iter != m_banList.end(); iter++ )
	{
		// Is this the ban we're looking for?
		if( !strcmp( (*iter)->szSerial, szSerial ) )
			return true;
	}

	return false;
}

void CBanManager::Pulse( bool bPreLoad )
{
	// Are bans not loaded?
	if( !m_bBansLoaded )
		return;

	// Loop through the ban list
	for( std::list< SServerBan* >::iterator iter = m_banList.begin(); iter != m_banList.end(); )
	{
		// Is this ban not permanent?
		if( (*iter)->ulUnbanTime != UNBAN_NEVER )
		{
			// Should we unban?
			if( SharedUtility::GetTime() >= ((*iter)->ulBanTime + (*iter)->ulUnbanTime) )
			{
				//
				CXMLNode * pNode = NULL;
				unsigned int uiCount = m_pRootNode->GetChildCount();

				// Is there no bans?
				if( uiCount == 0 )
					continue;

				// Loop over all children
				for( unsigned int i = 0; i < uiCount; i++ )
				{
					// Get the current node
					pNode = m_pRootNode->GetNode( i );

					// Is the node invalid?
					if( !pNode )
						continue;

					// Is this the node we're looking for?
					if( !strcmp( (*iter)->szSerial, pNode->GetValue() ) )
					{
						CLogFile::Printf( "REMOVED NODE: %s", pNode->GetValue() );

						// Remove this node from the ban file
						m_pRootNode->RemoveChild( pNode );

						// Increase the unban count
						m_iTotalUnbans++;

						break;
					}
				}

				// Save the ban file
				m_pBanFile->Save();

				// Delete this server ban instance
				delete *iter;

				// Remove this ban from the list
				iter = m_banList.erase( iter );
			}
		}
		else
		{
			// Increase the iterator
			++iter;
		}
	}

	// Was pulse not called before we loaded?
	if( !bPreLoad )
	{
		// Was there anyone unbanned?
		if( m_iTotalUnbans > 0 )
		{
			CLogFile::Printf( "Successfully purged %d bans.", m_iTotalUnbans );

			// Reset the total unban count
			m_iTotalUnbans = 0;
		}
	}
}