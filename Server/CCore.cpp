/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CCore.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "StdInc.h"

CCore::CCore( void )
{
	SetActive( true );
	m_iResourcesLoaded = 0;
	m_iFailedResources = 0;

	m_pNetworkModule = nullptr;
	m_pPlayerManager = nullptr;
	m_pScriptingManager = nullptr;
	m_pEvents = nullptr;
	m_pCommands = nullptr;
	m_pMasterList = nullptr;
	m_pQuery = nullptr;
	m_pWebServer = nullptr;
	m_pClientScriptingManager = nullptr;
	m_pTimerManager = nullptr;
	m_pBanManager = nullptr;
	m_pModuleManager = nullptr;
	m_pBlipManager = nullptr;
	m_pVehicleManager = nullptr;
	m_pResourceManager = nullptr;
	m_pWebRequest = nullptr;
	m_pPedManager = nullptr;

	SetGameModeText( "Unknown" );
	SetMapName( "Empire Bay" );
	SetWeather( "DT_RTRclear_day_noon" );
	SetSummer( true );

	SetMasterCode ( SharedUtility::str_rand ( 24 ) );
}

CCore::~CCore( void )
{
	m_pScriptingManager->UnloadAll ();

	SAFE_DELETE( m_pPedManager );
	SAFE_DELETE( m_pMasterList );
	SAFE_DELETE( m_pQuery );
	SAFE_DELETE( m_pBlipManager );
	SAFE_DELETE( m_pTimerManager );
	SAFE_DELETE( m_pVehicleManager );
	SAFE_DELETE( m_pPlayerManager );
	SAFE_DELETE( m_pResourceManager );
	SAFE_DELETE( m_pClientScriptingManager );
	SAFE_DELETE( m_pScriptingManager );
	SAFE_DELETE( m_pWebServer );
	SAFE_DELETE( m_pWebRequest );
	SAFE_DELETE( m_pCommands );
	SAFE_DELETE( m_pEvents );

	SAFE_DELETE(m_pModuleManager);
	SAFE_DELETE(m_pNetworkModule);

	CLogFile::Close();
}

bool CCore::Initialise( int argc, char * argv[] )
{
	srand ( SharedUtility::GetTime () );

	if( !SharedUtility::Exists( "resources" ) )
	{
		SharedUtility::CreateDirectory( "resources" );
	}

	if( !SharedUtility::Exists( "logs" ) )
	{
		SharedUtility::CreateDirectory( "logs" );
	}

#ifdef WIN32
	SetConsoleTitle( SERVER_TITLE );
#endif

	const char * szDateStr = SharedUtility::DateToString();

	String logFolder;
	logFolder.Format( "logs/%s", szDateStr );

	if( !SharedUtility::Exists( logFolder ) )
		SharedUtility::CreateDirectory( logFolder );

	time_t t = time( NULL );
	const struct tm * tm = localtime( &t );

	String strLogFile;
	strLogFile.Format( "logs/%s/m2online-server-%d-%d-%d.log", szDateStr, tm->tm_mday, (tm->tm_mon + 1), (1900 + tm->tm_year) );

	CLogFile::Open( (char *)strLogFile.Get(), true );
	CLogFile::Print( "==============================================================" );
	CLogFile::Printf("= %s v%s [%s]", MOD_NAME, MOD_VERS_STR, MOD_OS_STRING);
	CLogFile::Print( "==============================================================" );

	CSettings::Load( "config.xml" );
	CSettings::ParseCommandLine( argc, argv );

	CLogFile::Printf( "= Server name    : %s", CVAR_GET_STRING( "hostname" ) );
	CLogFile::Printf( "= Server port    : %d", CVAR_GET_INTEGER( "port" ) );
	CLogFile::Printf( "= Max players    : %d", CVAR_GET_INTEGER( "maxplayers" ) );
	CLogFile::Printf( "= Has password   : %s", IsPasswordProtected() ? "Yes" : "No" );
	CLogFile::Printf( "= Log file       : %s", strLogFile.Get() );
	CLogFile::Print( "==============================================================" );

	if ( strlen ( CVAR_GET_STRING ( "serverip" ) ) > 0 )
	{
		if ( !SharedUtility::IsValidIP ( CVAR_GET_STRING ( "serverip" ) ) )
		{
			CLogFile::Printf ( "[ERROR] Custom server ip is invalid. Please check your server config!" );
			Sleep ( 3000 );
			return false;
		}
	}

	m_pPlayerManager = new CPlayerManager;
	m_pTimerManager = new CTimerManager;
	m_pBlipManager = new CBlipManager;
	m_pVehicleManager = new CVehicleManager;
	m_pPedManager = new CPedManager;
	m_pBanManager = new CBanManager;
	m_pBanManager->Load( "bans.xml" );
	m_pWebServer = new CWebServer;
	m_pWebRequest = new CWebRequest;
	m_pMasterList = new CMasterList;
	m_pQuery = new CQuery( (CVAR_GET_INTEGER( "port" ) + 1) );
	m_pNetworkModule = new CNetworkModule;
	m_pNetworkModule->Startup ();
	m_pEvents = new CEvents;
	m_pCommands = new CCommands;
	m_pScriptingManager = new CScriptingManager;
	m_pResourceManager = new CResourceManager;
	m_pModuleManager = new CModuleManager;
	m_pModuleManager->LoadAll( CVAR_GET_LIST( "modules", "module" ) );
	m_pClientScriptingManager = new CClientScriptingManager( true );
	m_pResourceManager->LoadAll( CVAR_GET_LIST( "resources", "resource" ) );

	CLogFile::Printf( "Successfully loaded %d resources. (%d failed)\n", m_iResourcesLoaded, m_iFailedResources );

	return true;
}

void CCore::Deinitialise( void )
{
	if (m_pEvents)
		m_pEvents->Call( "onServerShutdown" );

	delete this;
}

void CCore::Pulse( void )
{
	if (!IsActive())
		return;

	if (m_pNetworkModule)
		m_pNetworkModule->Pulse();

	if (m_pPlayerManager)
		m_pPlayerManager->Pulse();

	if (m_pVehicleManager)
		m_pVehicleManager->Pulse();

	if (m_pTimerManager)
		m_pTimerManager->Pulse();

	if (m_pBanManager)
		m_pBanManager->Pulse();

	if (m_pModuleManager)
		m_pModuleManager->Pulse();

	if (m_pResourceManager)
		m_pResourceManager->Pulse();
}
