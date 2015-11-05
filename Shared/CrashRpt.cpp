/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CrashRpt.cpp
* Developers : h0pk1nz <bydlocoder@hotmail.com>
*
***************************************************************/

#ifdef _CLIENT
#include	"../Client/StdInc.h"
#else
#include	"../Server/StdInc.h"
#endif

bool CrashRpt::AddProp( char * pszName, char * pszValue )
{
	return ( crAddPropertyA( pszName, pszValue ) == 0 );
}

bool CrashRpt::AddPropf( char * pszName, char * pszFmt, ... )
{
	if ( !pszFmt )
		return false;

	va_list VAList;
	va_start( VAList, pszFmt );

	static char szBuffer[1024];
	ZeroMemory( szBuffer, sizeof( szBuffer ) );
	_vsnprintf_s( szBuffer, sizeof( szBuffer ) - strlen( szBuffer ), pszFmt, VAList );

	va_end( VAList );

	return AddProp( pszName, szBuffer );
}

bool CrashRpt::InstallMain( char * pszName, char * pszVersion )
{
#ifndef _WIN32
#error CrashRpt is Windows Only
#endif

	CR_INSTALL_INFO CrInstallInfo;
	memset( &CrInstallInfo, 0, sizeof( CR_INSTALL_INFO ) );

	CrInstallInfo.cb = sizeof( CR_INSTALL_INFO );
	CrInstallInfo.pszAppName = pszName;
	CrInstallInfo.pszAppVersion = pszVersion;
	CrInstallInfo.dwFlags = 0;
	CrInstallInfo.dwFlags |= CR_INST_ALL_POSSIBLE_HANDLERS;

#ifndef _CLIENT
	CrInstallInfo.dwFlags |= CR_INST_DONT_SEND_REPORT;		// only for devs
	CrInstallInfo.dwFlags |= CR_INST_APP_RESTART;			// auto restart
	CrInstallInfo.dwFlags |= CR_INST_NO_GUI;				// silent mode
#endif

	CrInstallInfo.uMiniDumpType = MiniDumpNormal;

#ifdef _CLIENT
	CrInstallInfo.uPriorities[CR_HTTP] = 1;
#else
	CrInstallInfo.uPriorities[CR_HTTP] = CR_NEGATIVE_PRIORITY;
#endif

	CrInstallInfo.uPriorities[CR_SMTP] = CR_NEGATIVE_PRIORITY;
	CrInstallInfo.uPriorities[CR_SMAPI] = CR_NEGATIVE_PRIORITY;

#ifndef _CLIENT
	std::string sErrorReportDir( SharedUtility::GetAppPath() );
	sErrorReportDir += "logs\\Crash Reports\\";
	CrInstallInfo.pszErrorReportSaveDir = sErrorReportDir.c_str();
	CrInstallInfo.pszRestartCmdLine = "/restart_crash_occurred";
#endif

	CrInstallInfo.pszUrl = "http://m2-multiplayer.com/crashreport/report.php";
	CrInstallInfo.pszPrivacyPolicyURL = "http://m2-multiplayer.com/crashreport/policy.html";

	if ( crInstall( &CrInstallInfo ) != 0 )
	{
		char szErrorMsg[512];
		crGetLastErrorMsg( szErrorMsg, 512 );

	#ifdef _CLIENT
		MessageBoxA( NULL, szErrorMsg, "CrashRpt Error!", MB_OK | MB_ICONERROR );
	#else
		CLogFile::Printf( "CrashRpt Error: %s", szErrorMsg );
	#endif
		
		return false;
	}

#ifdef _CLIENT
	AddFilef( "Settings XML", "m2mp-settings.xml", "%s%s", SharedUtility::GetAppPath(), "settings.xml" );
	AddFilef( "Core Log File", "m2mp-core.log", "%s%s", SharedUtility::GetAppPath(), "logs\\core.log" );
	AddFilef( "GUI Log File", "m2mp-gui.log", "%s%s", SharedUtility::GetAppPath(), "logs\\gui.log" );
#else
	time_t t = time( NULL );
	const struct tm * tm = localtime( &t );
	const char * szDateStr = SharedUtility::DateToString();
	String strLogFile( "logs\\%s\\m2mp-server-%d-%d-%d.log", szDateStr, tm->tm_mday, (tm->tm_mon + 1), (1900 + tm->tm_year) );
	AddFilef( "Server Log File", "m2mp-server.log", "%s%s", SharedUtility::GetAppPath(), strLogFile.Get() );
#endif

	return true;
}

bool CrashRpt::UninstallMain( void )
{
	return ( crUninstall() == 0 );
}

bool CrashRpt::InstallToCurrentThread( void )
{
	return ( crInstallToCurrentThread2( 0 ) == 0 );
}

bool CrashRpt::UninstallFromCurrentThread( void )
{
	return ( crUninstallFromCurrentThread() == 0 );
}

bool CrashRpt::TestGenerateReport( BOOL bManual )
{
	CR_EXCEPTION_INFO ExceptionInfo;
	memset( &ExceptionInfo, 0, sizeof( CR_EXCEPTION_INFO ) );
	ExceptionInfo.cb = sizeof( CR_EXCEPTION_INFO );
	ExceptionInfo.exctype = CR_SEH_EXCEPTION;
	ExceptionInfo.code = 0x1234;
	ExceptionInfo.pexcptrs = NULL;
	ExceptionInfo.bManual = bManual;

	return ( crGenerateErrorReport( &ExceptionInfo ) == 0 );
}

bool CrashRpt::TestEmulateCrash( int nType )
{
	return ( crEmulateCrash( nType ) == 0 );
}

bool CrashRpt::AddFile( char * pszDesc, char * pszDestFile, char * pszFile )
{
	return ( crAddFile2A( pszFile, pszDestFile, pszDesc, 
				CR_AF_MAKE_FILE_COPY | CR_AF_MISSING_FILE_OK ) == 0 );
}

bool CrashRpt::AddFilef( char * pszDesc, char * pszDestFile, char * pszFmt, ... )
{
	va_list VAList;
	va_start( VAList, pszFmt );

	static char szBuffer[1024];
	ZeroMemory( szBuffer, sizeof( szBuffer ) );
	_vsnprintf_s( szBuffer, sizeof( szBuffer ) - strlen( szBuffer ), pszFmt, VAList );

	va_end( VAList );

	return ( crAddFile2A( szBuffer, pszDestFile, pszDesc, 
				CR_AF_MAKE_FILE_COPY | CR_AF_MISSING_FILE_OK ) == 0 );
}
