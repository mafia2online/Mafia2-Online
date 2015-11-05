/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : SharedUtility.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"SharedUtility.h"
#include	"CCommon.h"
#include	"md5/md5.h"

#ifdef _WIN32
#include	<winsock2.h>
#include	<windows.h>
#include	<mmsystem.h>
#include	<direct.h>
#include	<tlhelp32.h>
#include	<time.h>
#include	<ws2tcpip.h>
#else
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<netdb.h>
#include	<limits.h>
#include	<unistd.h>
#include	<stdarg.h>
#include	<cstdlib>
#include	<sys/time.h>
#define		MAX_PATH		PATH_MAX
#endif

#include	<sys/stat.h>
#include	<errno.h>
#include	<stdio.h>

#include	"CLogFile.h"

#ifndef _LAUNCHER
#include	"../Libraries/squirrel/squirrel.h"
#include	"../Libraries/squirrel/sqstdio.h"

void CompileScript_ErrorHandler( HSQUIRRELVM vm, const SQChar * desc, const SQChar * source, SQInteger line, SQInteger column )
{
	CLogFile::Printf( "ERROR: Failed to compile script '%s'. (Line: %d, Column: %d, Error: %s)", source, line, column, desc );
}
#endif

namespace SharedUtility
{

	bool StripPath1( char * szString )
	{
		for( size_t i = strlen(szString); i > 0; --i )
		{
#ifdef _WIN32
			if( szString[i] == '\\' )
#else
			if( szString[i] == '/' )
#endif
			{
				szString[i + 1] = '\0';
				return true;
			}
		}

		return false;
	}

	char * StripPath2( char * szString )
	{
		char * szPath = szString;
		char * szStrippedPath = (szString + strlen(szString));

		while( szStrippedPath != szPath )
		{
#ifdef _WIN32
			if( *szStrippedPath == '\\' )
#else
			if( *szStrippedPath == '/' )
#endif
			{
				return (szStrippedPath + 1);
			}

			szStrippedPath--;
		}

		return szPath;
	}

	const char * GetAppPath( void )
	{
#ifdef _WIN32
		static unsigned int uiDummy;
#endif
		static char szAppPath[MAX_PATH];

#ifdef _WIN32
		HMODULE hModuleHandle;
		GetModuleHandleEx( GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&uiDummy, &hModuleHandle );
		GetModuleFileName( hModuleHandle, szAppPath, MAX_PATH );
#else
		readlink( "/proc/self/exe", szAppPath, MAX_PATH );
#endif
	
		StripPath1( szAppPath );
		return szAppPath;
	}

	const char * GetExePath( void )
	{
		static char szExePath[MAX_PATH];

#ifdef _WIN32
		GetModuleFileName( GetModuleHandle( NULL ), szExePath, MAX_PATH );
#else
		readlink( "/proc/self/exe", szExePath, MAX_PATH );
#endif

		StripPath1( szExePath );
		return szExePath;
	}

	unsigned long GetTime( void )
	{
#ifdef _WIN32
		return timeGetTime ();
#else
		timeval ts;
		gettimeofday ( &ts, 0 );
		return (DWORD)(ts.tv_sec * 1000 + (ts.tv_usec / 1000));
#endif
	}

	bool Exists( const char * szPath )
	{
		struct stat St;
		return (stat(szPath, &St) == 0);
	}

	const char * GetTimeString( void )
	{
		static char szTime[256];
		time_t t = time(NULL);
		const struct tm * tm = localtime(&t);
		strftime(szTime, sizeof(szTime), "%H:%M:%S", tm);
		return szTime;
	}

	double GetSecondCount( void )
	{
		return GetTime() * (1 / 1000.0);
	}

	String GetAbsolutePath( const char * szFormat, ... )
	{
		va_list args;
		char szBuffer[1024];
		va_start(args, szFormat);
		vsnprintf(szBuffer, sizeof(szBuffer), szFormat, args);
		va_end(args);
		return String("%s%s", GetAppPath(), szBuffer);
	}

	String DiconnectReasonToString( unsigned int uiReason )
	{
		String strReason( "Unknown" );

		switch( uiReason )
		{
		case DISCONNECT_TIMEOUT:	strReason.Set( "Timed Out" ); break;
		case DISCONNECT_QUIT:		strReason.Set( "Quit" ); break;
		case DISCONNECT_KICKED:		strReason.Set( "Kicked" ); break;
		case DISCONNECT_BANNED:		strReason.Set( "Banned" ); break;
		}

		return strReason;
	}

	String GetFileNameForScriptFile( const char * szName, const char * szScriptPath, const char * szHost, unsigned long ulPort )
	{
#ifdef _CLIENT
		// Strip illegal characters from the name
		String strName = RemoveIllegalCharacters( String( szName ) );

		//
		return String( "%s%s", GetClientScriptFolder( szHost, ulPort ).Get(), strName.Get() ).Get();
#else
		// Strip illegal characters from the name
		String sName( szName );
		String strName = RemoveIllegalCharacters( sName );

		// Split the script path
		std::vector< String > data = String( szScriptPath ).split( '/' );

		// Get the absolute path to the the resource script files
		String strScriptFiles( "resources/%s/scriptfiles", data[1].Get() );

		// Create the script files path if it doesn't exist
		if( !SharedUtility::Exists( strScriptFiles.Get() ) )
			SharedUtility::CreateDirectory( strScriptFiles.Get() );

		return String ( "%s/%s", strScriptFiles.Get(), strName.Get() );
#endif
	}

#ifdef _CLIENT
	DWORD GetSerial( void )
	{
		// Get the C: serial
		DWORD dwSerial;
		GetVolumeInformation( "C:\\", NULL, 0, &dwSerial, NULL, NULL, NULL, 0 );

		return dwSerial;
	}

	String GetSerialHash( void )
	{
		// Get the serial
		DWORD dwSerial = GetSerial( );

		char szSerialMask[] = "0000-0000";
		int i = 8;

		while( dwSerial > 0 && i >= 0 )
		{
			int v = ( dwSerial & 0xF );
			char ch = ( v < 10 ) ? '0' + v : 'A' + ( v + 10 );
			szSerialMask[i--] = ch;

			if( i == 4 )
				i--;

			dwSerial >>= 4;
		}

		return String( "%s", md5( szSerialMask ).Get() ).ToUpper();
	}

	String GetClientScriptFolder( String strHost, unsigned long ulPort )
	{
		return String( "cache/%s/", md5( String( "%s:%d", strHost.Get(), ulPort ) ).ToUpper().Get() );
	}
#endif

	int CreateDirectory( const char * szPath )
	{
		// Create the directory
		int
#ifdef _WIN32
			iReturn = _mkdir( szPath );
#else
			iReturn = mkdir( szPath, 0755 );
#endif

		// Was the directory created?
		if( !iReturn )
			return 1;

		// Does the directory already exist?
		if( iReturn == EEXIST )
			return 2;

		// Failed to create directory
		return 0;
	}

	bool CopyFile( const char * szSource, const char * szDestination )
	{
		// Open the source file
		FILE * fSource = fopen( szSource, "rb" );

		// Did the file fail to open?
		if( !fSource )
			return false;

		// Open the destination file
		FILE * fDestination = fopen( szDestination, "wb" );

		// Did the file fail to open?
		if( !fDestination )
			return false;

		// Copy the source file to the destination file
		char szBuffer[8192];
		size_t sBytesRead = 0;

		while( (sBytesRead = fread( szBuffer, 1, sizeof(szBuffer), fSource ) ) > 0 )
		{
			fwrite( szBuffer, 1, sBytesRead, fDestination );
		}

		// Close the destination file
		fclose( fDestination );

		// Close the source file
		fclose( fSource );
		return true;
	}

	void FatalExit( String strError )
	{
		// Print the error
		CLogFile::Print( strError.Get() );

#ifdef _WIN32
		// Wait for input
		CLogFile::Print( "" );
		CLogFile::Print( "Press enter to close." );
		getc(stdin);
#endif

		// Exit
		exit(1);
	}

	String RemoveIllegalCharacters( String& strString )
	{
		// Reset the found count
		unsigned int uiFound = 0;

		// Loop until there's no more illegal characters
		do
		{
			uiFound = strString.Substitute( "../", "" );
			uiFound += strString.Substitute( "..\\", "" );
			uiFound += strString.Substitute( ":/", "" );
			uiFound += strString.Substitute( ":\\", "" );
		}
		while( uiFound != 0 );

		return strString;
	}

	String HideCharacters( String strString )
	{
		String strReturn;

		// Get the string length
		size_t sLen = strString.GetLength();

		// Loop through all the characters
		for( size_t i = 0; i < sLen; i++ )
		{
			// Add a * character
			strReturn += "*";
		}

		return strReturn;
	}

	String StripForWebRequest( String strUrl )
	{
		String newStrUrl = strUrl;

		// Strip the url down if needed
		size_t sOffset = newStrUrl.Find( "http://" );

		// Did we find http:// ?
		if( sOffset != String::nPos )
		{
			// Erase http://
			newStrUrl.Erase( sOffset, 7 );
		}

		// Try find www.
		sOffset = newStrUrl.Find( "www." );

		// Did we find www. ?
		if( sOffset != String::nPos )
		{
			// Erase www.
			newStrUrl.Erase( sOffset, 4 );
		}

		return newStrUrl;
	}

	String GetTimePassedFromTime( unsigned long ulTime )
	{
		int iSecondsPassed = ((GetTime() - ulTime) / 1000);
		int iSeconds       = (iSecondsPassed % 60);
		int iMinutesPassed = (iSecondsPassed / 60);
		int iMinutes       = (iMinutesPassed % 60);
		int iHoursPassed   = (iMinutesPassed / 60);
		int iHours         = (iHoursPassed % 60);
		int iDaysPassed    = (iHoursPassed / 24);
		int iDays          = (iDaysPassed % 24);
		return String( "%d day(s), %d hour(s), %d minute(s) and %d second(s)", iDays, iHours, iMinutes, iSeconds );
	}

#ifdef _WIN32
	int AnsiToUnicode(const char * szAnsiString, size_t sAnsiStringLength, wchar_t * wszUnicodeString, size_t sUnicodeStringLength, DWORD dwFlags)
	{
		return MultiByteToWideChar(CP_ACP, dwFlags, szAnsiString, sAnsiStringLength, wszUnicodeString, sUnicodeStringLength);
	}

	int UnicodeToAnsi(const wchar_t * wszUnicodeString, size_t sUnicodeStringLength, char * szAnsiString, size_t sAnsiStringLength, DWORD dwFlags)
	{
		return WideCharToMultiByte(CP_UTF8, dwFlags, wszUnicodeString, sUnicodeStringLength, szAnsiString, sAnsiStringLength, NULL, NULL);
	}

	bool SetClipboardText(const char * szString, size_t sStringSize)
	{
		// Attempt to open the clipboard
		if(OpenClipboard(NULL))
		{
			// Empty the clipboard
			EmptyClipboard();

			// Allocate the memory globally to store our cut text
			HGLOBAL hMemory = GlobalAlloc(GMEM_DDESHARE, sStringSize);

			// Lock the globally allocated memory then get a pointer to it
			char * szMemory = (char *)GlobalLock(hMemory);

			// Copy the cut text to the globally allocated memory
			strcpy(szMemory, szString);

			// Unlock the globally allocated memory
			GlobalUnlock(hMemory);

			// Set the clipboard data to the allocated memory
			SetClipboardData(CF_TEXT, hMemory);

			// Close the clipboard
			CloseClipboard();
			return true;
		}

		// Failed to open the clipboard
		return false;
	}

	const char * GetClipboardText()
	{
		// Attempt to open the clipboard
		if(OpenClipboard(NULL))
		{
			// Get a pointer to the clipboard text
			char * szClipboardText = (char *)GetClipboardData(CF_TEXT);

			// Close the clipboard
			CloseClipboard();

			// Return the clipboard text pointer
			return szClipboardText;
		}

		// Failed to open the clipboard
		return NULL;
	}

	int InjectLibraryIntoProcess(HANDLE hProcess, const char * szLibraryPath)
	{
		int iReturn = 0;

		// Get the length of the library path
		size_t sLibraryPathLen = (strlen(szLibraryPath) + 1);

		// Allocate the a block of memory in our target process for the library path
		void * pRemoteLibraryPath = VirtualAllocEx(hProcess, NULL, sLibraryPathLen, MEM_COMMIT, PAGE_READWRITE);

		// Write our library path to the allocated block of memory
		SIZE_T sBytesWritten = 0;
		WriteProcessMemory(hProcess, pRemoteLibraryPath, (void *)szLibraryPath, sLibraryPathLen, &sBytesWritten);

		if(sBytesWritten != sLibraryPathLen)
		{
			// Failed to write the library path
			iReturn = 1;
		}
		else
		{
			// Get the handle of Kernel32.dll
			HMODULE hKernel32 = GetModuleHandle("Kernel32");

			// Get the address of the LoadLibraryA function from Kernel32.dll
			FARPROC pfnLoadLibraryA = GetProcAddress(hKernel32, "LoadLibraryA");

			// Create a thread inside the target process to load our library
			HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pfnLoadLibraryA, pRemoteLibraryPath, 0, NULL);

			if(hThread)
			{
				// Wait for the created thread to end
				WaitForSingleObject(hThread, INFINITE);

				// Close our thread handle
				CloseHandle(hThread);
			}
			else
			{
				// Thread creation failed
				iReturn = 2;
			}
		}

		// Free the allocated block of memory inside the target process
		VirtualFreeEx(hProcess, pRemoteLibraryPath, sizeof(pRemoteLibraryPath), MEM_RELEASE);
		return iReturn;
	}

	int InjectLibraryIntoProcess(DWORD dwProcessId, const char * szLibraryPath)
	{
		// Open our target process
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);

		if(!hProcess)
		{
			// Failed to open the process
			return 3;
		}

		// Inject the library into the process
		int iReturn = InjectLibraryIntoProcess(hProcess, szLibraryPath);

		// Close the process handle
		CloseHandle(hProcess);
		return iReturn;
	}

	bool GetProcessIdFromProcessName(char * szProcessName, DWORD * dwProcessId)
	{
		bool bReturn = false;

		// Create a tool help 32 process snapshot
		HANDLE hProcessSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if(!hProcessSnapShot)
			return false;

		PROCESSENTRY32 ProcessEntry;
		ProcessEntry.dwSize = sizeof(ProcessEntry);

		if(Process32First(hProcessSnapShot, &ProcessEntry))
		{
			// Enumerate through all processes
			while(Process32Next(hProcessSnapShot, &ProcessEntry))
			{
				// Check the process name to see if it matches szProcessName
				if(!strcmp(ProcessEntry.szExeFile, szProcessName))
				{
					// It matches, set the process id (if required) and return true
					if(dwProcessId)
						*dwProcessId = ProcessEntry.th32ProcessID;

					bReturn = true;
					break;
				}
			}
		}

		// Close the snapshot handle
		CloseHandle(hProcessSnapShot); 
		return bReturn;
	}

	bool IsProcessRunning(char * szProcessName)
	{
		// Simply return the value of GetProcessIdFromProcessName
		return GetProcessIdFromProcessName(szProcessName, NULL);
	}

	bool _TerminateProcess(char * szProcessName)
	{
		// Check if the process is running
		DWORD dwProcessId = 0;

		if(GetProcessIdFromProcessName(szProcessName, &dwProcessId))
		{
			// Attempt to open a handle to the process
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, false, dwProcessId);

			// Did the process handle open successfully?
			if(hProcess)
			{
				// Attempt to terminate the process
				if(TerminateProcess(hProcess, 0))
				{
					// Process terminated
					return true;
				}
			}
		}

		return false;
	}

	bool ReadRegistryString(HKEY hKeyLocation, const char * szSubKey, const char * szKey, const char * szDefault, char * szData, DWORD dwSize)
	{
		HKEY hKey = NULL;

		if(RegOpenKeyEx(hKeyLocation, szSubKey, NULL, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			DWORD dwType = REG_SZ;
			LONG lStatus = RegQueryValueEx(hKey, szKey, NULL, &dwType, (BYTE *)szData, &dwSize);
			RegCloseKey(hKey);
			return (lStatus == ERROR_SUCCESS);
		}

		if(szDefault)
			strncpy(szData, szDefault, dwSize);

		return false;
	}

	bool WriteRegistryString(HKEY hKeyLocation, const char * szSubKey, const char * szKey, char * szData, DWORD dwSize)
	{
		HKEY hKey = NULL;
		RegOpenKeyEx(hKeyLocation, szSubKey, NULL, KEY_WRITE, &hKey);

		if(!hKey)
			RegCreateKey(hKeyLocation, szSubKey, &hKey);

		if(hKey)
		{
			DWORD dwType = REG_SZ;
			RegSetValueEx(hKey, szKey, NULL, dwType, (BYTE *)szData, dwSize);
			RegCloseKey(hKey);
			return true;
		}

		return false;
	}
#endif

#ifndef _LAUNCHER
	bool CompileScript( const char * szFile, const char * szFileOut )
	{
		// Open a new squirrel vm
		HSQUIRRELVM vm = sq_open( 1024 );

		// Did the vm fail to open?
		if( !vm )
			return false;

		// Set the script compile error handler
		sq_setcompilererrorhandler( vm, CompileScript_ErrorHandler );

		// Compile the source file
		bool bFailed = SQ_FAILED( sqstd_loadfile( vm, szFile, SQTrue ) );

		// Did the script fail to compile?
		if( bFailed )
		{
			// Close the vm
			sq_close( vm );
			return false;
		}

		// Write the closure containing the compiled script
		bFailed = SQ_FAILED( sqstd_writeclosuretofile( vm, szFileOut ) );

		// Did the script fail to write the compiled script?
		if( bFailed )
		{
			// Close the vm
			sq_close( vm );
			return false;
		}

		sq_close( vm );
		return true;
	}
#endif

	const char * MonthToString( int iMonth )
	{
		if( iMonth > 0 && iMonth < 13 )
		{
			switch( iMonth )
			{
			case 1: return "January";
			case 2: return "Febuary";
			case 3: return "March";
			case 4: return "April";
			case 5: return "May";
			case 6: return "June";
			case 7: return "July";
			case 8: return "August";
			case 9: return "September";
			case 10: return "October";
			case 11: return "November";
			default: return "Decemeber";
			}
		}

		return "Unknown";
	}

	const char * DateToString()
	{
		static char szDate[ 256 ];

		// Get the current time
		time_t t = time( NULL );

		// Convert the time into a date/time struct
		const struct tm * tm = localtime( &t );

		// Format the date string
		strftime( szDate, sizeof( szDate ), "%B %Y", tm );

		return szDate;
	}

	String BytesToString( unsigned int uiBytes )
	{
		// 1024 = KB
		// 1048576 = MB
		// 1073741824 = GB
		// 1099511627776 = TB
		// 1125899906842624 = PB

		// If all else fails
		String strResult( "%d B", uiBytes );

		// Convert the bytes to a float (for decimal place)
		float fBytes = (float)uiBytes;

		//
		if( uiBytes >= 1024 && uiBytes < 1048576 )
			strResult.Format( "%.1f KB", (float)(fBytes / 1024) );
		else if( uiBytes >= 1048576 && uiBytes < 1073741824 )
			strResult.Format( "%.1f MB", (float)(fBytes / 1048576) );
		else if( uiBytes >= 1073741824 && uiBytes < 1099511627776 )
			strResult.Format( "%.1f GB", (float)(fBytes / 1073741824) );
		else if( uiBytes >= 1099511627776 && uiBytes < 1125899906842624 )
			strResult.Format( "%.1f TB", (float)(fBytes / 1099511627776) );
		else if( uiBytes >= 1125899906842624 )
			strResult.Format( "%.1f PB", (float)(fBytes / 1125899906842624) );

		return strResult;
	}

	String str_rand ( unsigned int uiLength )
	{
		// Build the charset
		static const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

		//
		String strResult;

		// Generate a random seed
		srand ( GetTime () );

		// Appand a new character to cover the length
		for ( int i = 0; i < uiLength; i ++ )
			strResult.Append  ( charset[ rand() % sizeof ( charset ) - 1 ] );

		return strResult;
	}

	int int_concat(int a, int b)
	{
		int i = 1000000000;

		while (i > 1 && !(b / i))
			i /= 10;

		return a * i * 10 + b;
	}

	bool IsValidIP ( const char * szIpAddress )
	{
		struct sockaddr_in sa;
		return ( inet_pton( AF_INET, szIpAddress, &( sa.sin_addr ) ) != 0 );
	}

};
