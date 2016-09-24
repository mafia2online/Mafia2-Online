/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CScreenShot.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "CString.h"
#include "CScreenShot.h"

#include "SharedUtility.h"

#include "CChat.h"
#include "CM2Camera.h"

#include "CColor.h"

#include "png.h"

#include "Scripting/CSquirrelArguments.h"

#include "CClientScriptingManager.h"
#include "CEvents.h"

#include "CLogFile.h"

const size_t	BYTES_PER_PIXEL			= 4;
const D3DFORMAT SCREEN_SHOT_FORMAT		= D3DFMT_A8R8G8B8;
const size_t SCREEN_SHOT_FORMAT_BYTES_PER_PIXEL = (32 / 8);

CScreenShot::CScreenShot()
	: m_thread()
	, m_jobState(JOB_STATE_IDLE)
	, m_imageData(nullptr)
	, m_saveTime(0)

	, m_fileName()

	, m_imageWidth(0)
	, m_imageHeight(0)
{
}

CScreenShot::~CScreenShot()
{
	FinalizeJob();
}

void CScreenShot::FinalizeJob(void)
{
	if (m_jobState == JOB_STATE_IDLE)
		return;

	m_thread.join();

	if (m_jobState == JOB_STATE_DONE) {
		CSquirrelArguments pArguments;
		pArguments.push( m_fileName );
		pArguments.push( (int)m_saveTime );
		if (CCore::Instance()->GetClientScriptingManager() && CCore::Instance()->GetClientScriptingManager()->GetEvents() && CCore::Instance()->GetClientScriptingManager()->GetEvents()->Call("onClientScreenshot", &pArguments).GetInteger() == 1)
			CCore::Instance()->GetChat()->AddInfoMessage("Screenshot taken: %s (Took %d seconds)", m_fileName.Get(), m_saveTime);
	}
	else if (m_jobState == JOB_STATE_FAILED_FILE_OPEN) {
		CCore::Instance()->GetChat()->AddInfoMessage(CColor(255, 0, 0, 255), "Failed to save screenshot. (Can't open target file)");
		CLogFile::Printf( "Failed to save screenshot. (Can't open target file)" );
	}


	delete[] m_imageData;
	m_fileName.clear();
	m_jobState = JOB_STATE_IDLE;
}

DWORD CScreenShot::WorkerThread()
{
	unsigned long ulStartTime = SharedUtility::GetTime();

	unsigned uiRequestDataSize = (m_imageWidth * m_imageHeight * 4);
	unsigned uiLinePitch = (m_imageWidth * 4);

	unsigned char ** ucPtrs = new png_bytep [ m_imageHeight ];

	for( unsigned int i = 0; i < m_imageHeight; i++ )
		ucPtrs[i] = (m_imageData + (uiLinePitch * i));

	FILE * fFile = fopen( m_fileName.Get(), "wb" );
	if( !fFile )
	{
		delete [] ucPtrs;
		m_jobState = JOB_STATE_FAILED_FILE_OPEN;
		return 0;
	}

	png_struct * png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
	png_info * info_ptr = png_create_info_struct( png_ptr );

	png_init_io( png_ptr, fFile );
	png_set_filter( png_ptr, 0, PNG_FILTER_NONE );
	png_set_compression_level( png_ptr, 1 );
	png_set_IHDR( png_ptr, info_ptr, m_imageWidth, m_imageHeight, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT );
	png_set_rows( png_ptr, info_ptr, ucPtrs );
	png_write_png( png_ptr, info_ptr, PNG_TRANSFORM_BGR | PNG_TRANSFORM_STRIP_ALPHA, NULL );
	png_write_end( png_ptr, info_ptr );
	png_destroy_write_struct( &png_ptr, &info_ptr );

	fclose( fFile );
	delete []ucPtrs;

	m_saveTime = ((SharedUtility::GetTime() - ulStartTime) / 1000);
	m_jobState = JOB_STATE_DONE;
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

	if( m_jobState != JOB_STATE_IDLE )
		return false;

	m_jobState = JOB_STATE_WORKING;
	m_imageData = ucData;
	m_fileName = GetValidScreenshotName();

	m_imageWidth = CCore::Instance()->GetCamera()->GetWindowWidth();
	m_imageHeight = CCore::Instance()->GetCamera()->GetWindowHeight();

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
	return m_jobState != JOB_STATE_IDLE;
}

void CScreenShot::ProcessRenderThread(void)
{
	if (m_jobState == JOB_STATE_WORKING || m_jobState == JOB_STATE_IDLE)
		return;

	FinalizeJob();
}
