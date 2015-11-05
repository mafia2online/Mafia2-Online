/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CScreenShot.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;

bool CScreenShot::m_bSaving = false;

// Saving thread variables
static unsigned char * m_ucData = NULL;

#define	BYTES_PER_PIXEL								4
#define SCREEN_SHOT_FORMAT							D3DFMT_A8R8G8B8
#define SCREEN_SHOT_FORMAT_BYTES_PER_PIXEL			(32 / 8)

DWORD CScreenShot::WorkerThread( LPVOID lpParam )
{
	// Get the start write time
	unsigned long ulStartTime = SharedUtility::GetTime();

	// Get the screen dimensions
	unsigned long ulScreenWidth = pCore->GetCamera()->GetWindowWidth();
	unsigned long ulScreenHeight = pCore->GetCamera()->GetWindowHeight();
	unsigned int uiRequestDataSize = (ulScreenHeight * ulScreenWidth * 4);
	unsigned int uiLinePitch = (ulScreenWidth * 4);

	// Create the screen data buffer
	unsigned char * tmpData = new unsigned char [ ulScreenHeight * uiLinePitch ];
	unsigned char ** ucPtrs = new png_bytep [ ulScreenHeight ];

	// Copy the surface data
	memcpy( tmpData, m_ucData, (ulScreenHeight * uiLinePitch) );

	// Copy the unk data
	for( unsigned int i = 0; i < ulScreenHeight; i++ )
		ucPtrs[i] = (tmpData + (uiLinePitch * i));

	// Get a valid screenshot file name
	String strFileName( GetValidScreenshotName() );

	// Open the file
	FILE * fFile = fopen( strFileName.Get(), "wb" );

	// Did the file fail to open?
	if( !fFile )
	{
		pCore->GetChat()->AddInfoMessage( CColor( 255, 0, 0, 255 ), "Failed to save screenshot. (Can't open target file)" );
		CLogFile::Printf( "Failed to save screenshot. (Can't open target file)" );

		// Cleanup the screen data buffer
		delete[] tmpData;

		// Mark as not saving
		m_bSaving = false;

		// Delete the data
		delete[] m_ucData;
		m_ucData = NULL;

		return 0;
	}

	// Create the png pointers
	png_struct * png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
	png_info * info_ptr = png_create_info_struct( png_ptr );

	// Write the screen data to the file
	png_init_io( png_ptr, fFile );
	png_set_filter( png_ptr, 0, PNG_FILTER_NONE );
	png_set_compression_level( png_ptr, 1 );
	png_set_IHDR( png_ptr, info_ptr, ulScreenWidth, ulScreenHeight, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT );
	png_set_rows( png_ptr, info_ptr, ucPtrs );
	png_write_png( png_ptr, info_ptr, PNG_TRANSFORM_BGR | PNG_TRANSFORM_STRIP_ALPHA, NULL );
	png_write_end( png_ptr, info_ptr );
	png_destroy_write_struct( &png_ptr, &info_ptr );

	// Close the file
	fclose( fFile );

	// Cleanup the screen data buffer
	if( tmpData )
		delete[] tmpData;

	// Mark as not saving
	m_bSaving = false;

	// Calculate the elapsed time in seconds
	unsigned long ulElapsed = ((SharedUtility::GetTime() - ulStartTime) / 1000);

	// Build event arguments
	CSquirrelArguments pArguments;
    pArguments.push( strFileName.Get() );
    pArguments.push( (int)ulElapsed );

	// Should we output a message?
	if( pCore->GetClientScriptingManager() && pCore->GetClientScriptingManager()->GetEvents() && pCore->GetClientScriptingManager()->GetEvents()->Call( "onClientScreenshot", &pArguments ).GetInteger() == 1 )
		pCore->GetChat()->AddInfoMessage( "Screenshot taken: %s (Took %d seconds)", strFileName.Get(), ulElapsed );

	// Delete the data
	delete[] m_ucData;
	m_ucData = NULL;

	return 0;
}

const char * CScreenShot::GetValidScreenshotName( void )
{
	// Get the current system time
	SYSTEMTIME sysTime;
	GetLocalTime( &sysTime );

	// Does the screenshots path not exist?
	if( !SharedUtility::Exists( "screenshots" ) )
	{
		// Create the screenshot path
		SharedUtility::CreateDirectory( "screenshots" );
	}

	return RakNet::RakString( "screenshots\\%02d-%02d-%02d %02d-%02d-%02d.png", sysTime.wDay, sysTime.wMonth, sysTime.wYear, sysTime.wHour, sysTime.wMinute, sysTime.wSecond ).C_String();
}

bool CScreenShot::BeginWrite( unsigned char * ucData )
{
	// Don't we have any valid data?
	if( !ucData )
		return false;

	// Are we already saving a screenshot?
	if( m_bSaving )
		return false;

	// Create the thread to save the screenshot
	HANDLE hThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CScreenShot::WorkerThread, NULL, CREATE_SUSPENDED, NULL );

	// Did the thread fail to create?
	if( !hThread )
	{
		pCore->GetChat()->AddInfoMessage( CColor( 255, 0, 0, 255 ), "Failed to save screenshot. (Can't create worker thread)" );
		CLogFile::Printf( "Failed to save screenshot. (Can't create worker thread)" );
		return false;
	}

	// Mark as saving
	m_bSaving = true;

	// Store the data
	m_ucData = ucData;

	// Set the thread priority to low
	SetThreadPriority( hThread, THREAD_PRIORITY_LOWEST );

	// Resume the worker thread
	ResumeThread( hThread );

	return true;
}

bool CScreenShot::IsSaving( void )
{
	return m_bSaving;
}