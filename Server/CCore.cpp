/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CCore.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

CCore::CCore( void )
{
	// Reset variables
	SetActive( true );
	m_iResourcesLoaded = 0;
	m_iFailedResources = 0;
	
	// Reset pointers
	m_pNetworkModule = NULL;
	m_pPlayerManager = NULL;
	m_pScriptingManager = NULL;
	m_pEvents = NULL;
	m_pCommands = NULL;
	m_pMasterList = NULL;
	m_pQuery = NULL;
	m_pWebServer = NULL;
	m_pClientScriptingManager = NULL;
	m_pTimerManager = NULL;
	m_pBanManager = NULL;
	m_pModuleManager = NULL;
	m_pBlipManager = NULL;
	m_pVehicleManager = NULL;
	m_pResourceManager = NULL;
	m_pWebRequest = NULL;
	m_pPedManager = NULL;

	// Set the default gamemode text, map and weather
	SetGameModeText( "Unknown" );
	SetMapName( "Empire Bay" );
	SetWeather( "DT_RTRclear_day_noon" );
	SetSummer( true );

	// Generate a random master code string
	SetMasterCode ( SharedUtility::str_rand ( 24 ) );
}

CCore::~CCore( void )
{
	// Unload all scripts
	m_pScriptingManager->UnloadAll ();

	// Cleanup
	SAFE_DELETE( m_pPedManager );
	SAFE_DELETE( m_pModuleManager );
	SAFE_DELETE( m_pMasterList );
	SAFE_DELETE( m_pQuery );
	SAFE_DELETE( m_pBlipManager );
	SAFE_DELETE( m_pTimerManager );
	SAFE_DELETE( m_pVehicleManager );
	SAFE_DELETE( m_pPlayerManager );
	SAFE_DELETE( m_pResourceManager );
	SAFE_DELETE( m_pNetworkModule );
	SAFE_DELETE( m_pClientScriptingManager );
	SAFE_DELETE( m_pScriptingManager );
	SAFE_DELETE( m_pWebServer );
	SAFE_DELETE( m_pWebRequest );
	SAFE_DELETE( m_pCommands );
	SAFE_DELETE( m_pEvents );

	// Close the log file
	CLogFile::Close();
}

bool CCore::Initialise( int argc, char * argv[] )
{
	// Does the resources path not exist?
	if( !SharedUtility::Exists( "resources" ) )
	{
		// Create the scripts directory
		SharedUtility::CreateDirectory( "resources" );
	}

	// Does the logs path not exist?
	if( !SharedUtility::Exists( "logs" ) )
	{
		// Create the logs directory
		SharedUtility::CreateDirectory( "logs" );
	}

#ifdef WIN32
	// Set the console window
	SetConsoleTitle( SERVER_TITLE );
#endif

	// Get the current date string
	const char * szDateStr = SharedUtility::DateToString();

	// Create the folder if needed for the current date
	if( !SharedUtility::Exists( String( "logs/%s", szDateStr ).Get() ) )
		SharedUtility::CreateDirectory( String( "logs/%s", szDateStr ).Get() );

	// Get the current time
	time_t t = time( NULL );
	const struct tm * tm = localtime( &t );

	// Get the log file path and name
	String strLogFile( "logs/%s/m2mp-server-%d-%d-%d.log", szDateStr, tm->tm_mday, (tm->tm_mon + 1), (1900 + tm->tm_year) );

	// Open the log file
	CLogFile::Open( (char *)strLogFile.Get(), true );

#ifdef _WIN324
	// Install the crash reporter
	CrashRpt::InstallMain( "Mafia 2 Multiplayer (Server)", MOD_VERS_STR );

	// Check if restarted after crash	
	if ( strstr( GetCommandLineA(), "/restart_crash_occurred" ) )
		CLogFile::Print( "WARNING: Server restarted after crash!\n" );
#endif

	CLogFile::Print( "==============================================================" );
	CLogFile::Printf( "= Mafia 2 Multiplayer v%s [%s]", MOD_VERS_STR, MOD_OS_STRING );
	CLogFile::Print( "==============================================================" );

	// Load the server settings
	CSettings::Load( "config.xml" );

	// Parse the command line
	CSettings::ParseCommandLine( argc, argv );

	CLogFile::Printf( "= Server name    : %s", CVAR_GET_STRING( "hostname" ) );
	CLogFile::Printf( "= Server port    : %d", CVAR_GET_INTEGER( "port" ) );
	CLogFile::Printf( "= Max players    : %d%s", CVAR_GET_INTEGER( "maxplayers" ), !IsPasswordProtected() ? "\n" : "" );

	// Is the server passworded?
	if( IsPasswordProtected() )
	{
		CLogFile::Printf( "= Password	    : %s\n", SharedUtility::HideCharacters( CVAR_GET_STRING( "password" ) ).Get() );
	}

	CLogFile::Printf( "= Log file       : %s", strLogFile.Get() );

	CLogFile::Print( "==============================================================\n" );

	// Do we have a custom host set?
	if ( strlen ( CVAR_GET_STRING ( "serverip" ) ) > 0 )
	{
		// Is the custom host invalid?
		if ( !SharedUtility::IsValidIP ( CVAR_GET_STRING ( "serverip" ) ) )
		{
			CLogFile::Printf ( "[ERROR] Custom server ip is invalid. Please check your server config!" );

			// Close the server after 3 seconds
			Sleep ( 3000 );
			return false;
		}
	}

	// Create the player manager instance
	m_pPlayerManager = new CPlayerManager;

	// Create the timer manager instance
	m_pTimerManager = new CTimerManager;

	// Create the blip manager instance
	m_pBlipManager = new CBlipManager;

	// Create the vehicle manager instance
	m_pVehicleManager = new CVehicleManager;

	// Create the ped manager instance
	m_pPedManager = new CPedManager;

	// Create the ban manager instance
	m_pBanManager = new CBanManager;

	// Load the bans
	m_pBanManager->Load( "bans.xml" );

	// Create the webserver instance
	m_pWebServer = new CWebServer;

	// Create the web request instance
	m_pWebRequest = new CWebRequest;

	// Create the masterlist instance
	m_pMasterList = new CMasterList;

	// Create the query instance
	m_pQuery = new CQuery( (CVAR_GET_INTEGER( "port" ) + 1) );

	// Create the network module instance
	m_pNetworkModule = new CNetworkModule;

	// Startup the network module
	m_pNetworkModule->Startup ();

	// Create the events instance
	m_pEvents = new CEvents;

	// Create the commands instance
	m_pCommands = new CCommands;

	// Create the scripting manager instance
	m_pScriptingManager = new CScriptingManager;

	// Create the resource manager instance
	m_pResourceManager = new CResourceManager;

	// Create the module manager instance
	m_pModuleManager = new CModuleManager;

	// Load all the modules
	m_pModuleManager->LoadAll( CVAR_GET_LIST( "modules", "module" ) );

	// Create the client scripting manager instance
	m_pClientScriptingManager = new CClientScriptingManager( true );

	// Load all resources
	m_pResourceManager->LoadAll( CVAR_GET_LIST( "resources", "resource" ) );

	CLogFile::Printf( "Successfully loaded %d resources. (%d failed)\n", m_iResourcesLoaded, m_iFailedResources );

	return true;
}

void CCore::Deinitialise( void )
{
	// Delete ourselfs
	delete this;
}

void CCore::Pulse( void )
{
	// Pulse the network module
	if( m_pNetworkModule )
		m_pNetworkModule->Pulse ();

	// Pulse the vehicle manager
	if( m_pPlayerManager )
		m_pPlayerManager->Pulse ();

	// Pulse the vehicle manager
	if( m_pVehicleManager )
		m_pVehicleManager->Pulse ();

	// Pulse the timer manager
	if( m_pTimerManager )
		m_pTimerManager->Pulse ();

	// Pulse the ban manager
	if( m_pBanManager )
		m_pBanManager->Pulse ();

	// Pulse the module
	if( m_pModuleManager )
		m_pModuleManager->Pulse ();

	// Pulse the masterlist
	if( m_pMasterList )
		m_pMasterList->Pulse ();

	// Call the pulse event
	if( m_pEvents )
		m_pEvents->Call( "onServerPulse" );
}
