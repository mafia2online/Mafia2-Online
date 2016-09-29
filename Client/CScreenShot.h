/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CScreenShot.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include <thread>

/**
 * Lock-less screenshot saving utility class.
 */
class CScreenShot
{
private:

	std::thread							m_thread;

	enum JobState
	{
		JOB_STATE_IDLE,
		JOB_STATE_WORKING,
		JOB_STATE_FAILED_FILE_OPEN,
		JOB_STATE_DONE
	};

	/** Variable indicating the state of the screenshot save job. */
	mutable JobState					m_jobState;

	/** The save operation time */
	unsigned							m_saveTime;

	/** The image data storage */
	std::unique_ptr<uint8_t[]>			m_imageData;

	/** Screenshot file name */
	String								m_fileName;

	/** Image size */
	//@{
	unsigned							m_imageWidth;
	unsigned							m_imageHeight;
	//@}

	DWORD			WorkerThread					( );
	const char		* GetValidScreenshotName		( void );

	void			FinalizeJob						( );
public:

	CScreenShot();
	~CScreenShot();

	bool			BeginWrite						( std::unique_ptr<uint8_t[]> &pixels, const unsigned width, const unsigned height );
	bool			IsSaving						( void );

	void			ProcessRenderThread				( void );
};