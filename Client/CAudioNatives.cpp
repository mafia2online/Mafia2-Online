/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CCameraNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "CAudio.h"
#include "CAudioManager.h"

#include "Scripting/CSquirrel.h"
#include "Scripting/CSquirrelCommon.h"
#include "Scripting/CScriptingManager.h"

#include "CLogFile.h"

#include "SharedUtility.h"

#include "CAudioNatives.h"
_MEMBER_FUNCTION_IMPL(Audio, constructor);
_MEMBER_FUNCTION_IMPL(Audio, deleteSound);
_MEMBER_FUNCTION_IMPL(Audio, play);
_MEMBER_FUNCTION_IMPL(Audio, stop);
_MEMBER_FUNCTION_IMPL(Audio, pause);
_MEMBER_FUNCTION_IMPL(Audio, isStarted);
_MEMBER_FUNCTION_IMPL(Audio, isPlaying);
_MEMBER_FUNCTION_IMPL(Audio, isPaused);
_MEMBER_FUNCTION_IMPL(Audio, setVolume);
_MEMBER_FUNCTION_IMPL(Audio, getVolume);
_MEMBER_FUNCTION_IMPL(Audio, resetVolume);
_MEMBER_FUNCTION_IMPL(Audio, getLength);

_BEGIN_CLASS(Audio)
_MEMBER_FUNCTION(Audio, constructor, 3, "bbs")
_MEMBER_FUNCTION(Audio, deleteSound, 0, NULL)
_MEMBER_FUNCTION(Audio, play, 0, NULL)
_MEMBER_FUNCTION(Audio, stop, 0, NULL)
_MEMBER_FUNCTION(Audio, pause, 0, NULL)
_MEMBER_FUNCTION(Audio, isStarted, 0, NULL)
_MEMBER_FUNCTION(Audio, isPlaying, 0, NULL)
_MEMBER_FUNCTION(Audio, isPaused, 0, NULL)
_MEMBER_FUNCTION(Audio, setVolume, 1, "f")
_MEMBER_FUNCTION(Audio, getVolume, 0, NULL)
_MEMBER_FUNCTION(Audio, resetVolume, 0, NULL)
_MEMBER_FUNCTION(Audio, getLength, 0, NULL)
_END_CLASS(Audio)

void CAudioNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->NewClass(&_CLASS_DECL(Audio));
}

_MEMBER_FUNCTION_RELEASE_HOOK(Audio)
{
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, constructor)
{
	// Required variables
	SQBool sqbIsOnlineStream;
	SQBool sqbReplay;
	const char * szSoundName;

	// Fetch values
	sq_getbool(pVM, -3, &sqbIsOnlineStream);
	bool bIsOnlineStream = (sqbIsOnlineStream != 0);
	sq_getbool(pVM, -2, &sqbReplay);
	bool bReplay = (sqbReplay != 0);
	sq_getstring(pVM, -1, &szSoundName);

	// Local stream ?
	if (!bIsOnlineStream) {
		szSoundName = SharedUtility::GetAbsolutePath("files\\%s", szSoundName);
	}

	// Construct
	CAudio * pAudio = new CAudio(szSoundName, bReplay, bIsOnlineStream);

	// Init
	if (!pAudio || !pAudio->Load() || SQ_FAILED(sq_setinstance(pVM, pAudio)))
	{
		CLogFile::Printf("Failed to load Audio from file %s", szSoundName);
		SAFE_DELETE(pAudio);
		sq_pushbool(pVM, false);
		return 1;
	}

	// Add and return
	CCore::Instance()->GetAudioManager()->Add(pAudio);
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, deleteSound)
{
	// Get the Audio instance
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);

	// Is valid instance ?
	if (!pAudio)
	{
		sq_pushbool(pVM, false);
		return (1);
	}

	// Remove file
	CCore::Instance()->GetAudioManager()->Remove(pAudio);
	sq_setinstance(pVM, NULL);
	// Return
	return (1);
}

void PlayAudioThread(LPVOID lpAudio)
{
	((CAudio*)lpAudio)->Play();
}

void StopAudioThread(LPVOID lpAudio)
{
	((CAudio*)lpAudio)->Stop();
}

_MEMBER_FUNCTION_IMPL(Audio, play)
{
	// Get the Audio instance
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);

	// Is valid instance ?
	if (!pAudio)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	// Play and return
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)PlayAudioThread, pAudio, NULL, NULL);
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, stop)
{
	// Get the Audio instance
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);

	// Is valid instance ?
	if (!pAudio)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	// Stop and return
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)StopAudioThread, pAudio, NULL, NULL);
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, pause)
{
	// Get the Audio instance
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);

	// Is valid instance ?
	if (!pAudio)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	// Pause and return
	pAudio->Pause();
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, getVolume)
{
	// Get the Audio instance
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);

	// Is valid instance ?
	if (!pAudio)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	// Get and return volume
	sq_pushfloat(pVM, pAudio->GetVolume());
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, resetVolume)
{
	// Get the Audio instance
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);

	// Is valid instance ?
	if (!pAudio)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	// Reset and return
	pAudio->Unmute();

	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, setVolume)
{
	// Get the Audio instnace
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);

	// Is valid instance ?
	if (!pAudio)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	// Get, set and return
	float fVolume;
	sq_getfloat(pVM, 2, &fVolume);

	pAudio->SetVolume(fVolume);

	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, isStarted)
{
	// Get the Audio instance
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);

	// Is valid instance ?
	if (!pAudio)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	// Return
	sq_pushbool(pVM, (pAudio->IsPlaying() || pAudio->IsStalled()));
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, isPlaying)
{
	// Get the Audio instance
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);

	// Is valid instance ?
	if (!pAudio)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	// Return
	sq_pushbool(pVM, pAudio->IsPlaying());
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, isPaused)
{
	// Get the Audio instance
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);

	// Is valid instance ?
	if (!pAudio)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	// Return
	sq_pushbool(pVM, pAudio->IsPaused());
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, getLength)
{
	// Get the Audio instance
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);

	// Is valid instance ?
	if (!pAudio)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	// Return
	sq_pushinteger(pVM, pAudio->GetLength());
	return 1;
}