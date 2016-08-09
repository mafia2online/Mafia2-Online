/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CScreenShot.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"BaseInc.h"

#include	"CCore.h"

#include	"CScreenShot.h"

#include	"SharedUtility.h"

#include	"CChat.h"
#include	"CM2Camera.h"

#include	"CString.h"
#include	"CColor.h"

#include	"../Libraries/lpng142/png.h"

#include	"Scripting\CSquirrelArguments.h"

#include	"CClientScriptingManager.h"
#include	"CEvents.h"

static unsigned char * m_ucData = NULL;

#define	BYTES_PER_PIXEL								4
#define SCREEN_SHOT_FORMAT							D3DFMT_A8R8G8B8
#define SCREEN_SHOT_FORMAT_BYTES_PER_PIXEL			(32 / 8)

CScreenShot::CScreenShot() : m_bSaving(false)
{
}

DWORD CScreenShot::WorkerThread()
{
	unsigned long ulStartTime = SharedUtility::GetTime();

	unsigned long ulScreenWidth = CCore::Instance()->GetCamera()->GetWindowWidth();
	unsigned long ulScreenHeight = CCore::Instance()->GetCamera()->GetWindowHeight();
	unsigned int uiRequestDataSize = (ulScreenHeight * ulScreenWidth * 4);
	unsigned int uiLinePitch = (ulScreenWidth * 4);

	unsigned char * tmpData = new unsigned char [ ulScreenHeight * uiLinePitch ];
	unsigned char ** ucPtrs = new png_bytep [ ulScreenHeight ];

	memcpy( tmpData, m_ucData, (ulScreenHeight * uiLinePitch) );
	for( unsigned int i = 0; i < ulScreenHeight; i++ )
		ucPtrs[i] = (tmpData + (uiLinePitch * i));

	String strFileName( GetValidScreenshotName() );

	FILE * fFile = fopen( strFileName.Get(), "wb" );
	if( !fFile )
	{
		CCore::Instance()->GetChat()->AddInfoMessage(CColor(255, 0, 0, 255), "Failed to save screenshot. (Can't open target file)");
		CLogFile::Printf( "Failed to save screenshot. (Can't open target file)" );

		delete[] tmpData;
		m_bSaving = false;
		delete[] m_ucData;
		m_ucData = NULL;
		return 0;
	}

	png_struct * png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
	png_info * info_ptr = png_create_info_struct( png_ptr );

	png_init_io( png_ptr, fFile );
	png_set_filter( png_ptr, 0, PNG_FILTER_NONE );
	png_set_compression_level( png_ptr, 1 );
	png_set_IHDR( png_ptr, info_ptr, ulScreenWidth, ulScreenHeight, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT );
	png_set_rows( png_ptr, info_ptr, ucPtrs );
	png_write_png( png_ptr, info_ptr, PNG_TRANSFORM_BGR | PNG_TRANSFORM_STRIP_ALPHA, NULL );
	png_write_end( png_ptr, info_ptr );
	png_destroy_write_struct( &png_ptr, &info_ptr );

	fclose( fFile );
	if( tmpData )
		delete[] tmpData;

	m_bSaving = false;

	unsigned long ulElapsed = ((SharedUtility::GetTime() - ulStartTime) / 1000);

	CSquirrelArguments pArguments;
    pArguments.push( strFileName.Get() );
    pArguments.push( (int)ulElapsed );
	if (CCore::Instance()->GetClientScriptingManager() && CCore::Instance()->GetClientScriptingManager()->GetEvents() && CCore::Instance()->GetClientScriptingManager()->GetEvents()->Call("onClientScreenshot", &pArguments).GetInteger() == 1)
		CCore::Instance()->GetChat()->AddInfoMessage("Screenshot taken: %s (Took %d seconds)", strFileName.Get(), ulElapsed);

	delete[] m_ucData;
	m_ucData = NULL;
	return 0;
}

const char * CScreenShot::GetValidScreenshotName( void )
{
	SYSTEMTIME sysTime;
	GetLocalTime( &sysTime );

	if( !SharedUtility::Exists( "screenshots" ) )
	{
		SharedUtility::CreateDirectory( "screenshots" );
	}
	return RakNet::RakString( "screenshots\\%02d-%02d-%02d %02d-%02d-%02d.png", sysTime.wDay, sysTime.wMonth, sysTime.wYear, sysTime.wHour, sysTime.wMinute, sysTime.wSecond ).C_String();
}

bool CScreenShot::BeginWrite( unsigned char * ucData )
{
	if( !ucData )
		return false;

	if( m_bSaving )
		return false;


	m_bSaving = true;
	m_ucData = ucData;
	m_thread = std::thread(&CScreenShot::WorkerThread, this);

	if( !m_thread.joinable())
	{
		CCore::Instance()->GetChat()->AddInfoMessage(CColor(255, 0, 0, 255), "Failed to save screenshot. (Can't create worker thread)");
		CLogFile::Printf( "Failed to save screenshot. (Can't create worker thread)" );
		return false;
	}
	return true;
}

bool CScreenShot::IsSaving( void )
{
	return m_bSaving;
}