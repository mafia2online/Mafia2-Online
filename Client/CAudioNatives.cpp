/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CCameraNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;

SQInteger CAudioNatives::Create(SQVM *pVM)
{
	// Required variables
	SQBool		bReplay;
	SQBool		bOnlineStream;
	const char	* streamName;

	// Fetch values
	sq_getbool(pVM, -1, &bReplay);
	sq_getbool(pVM, -2, &bOnlineStream);
	sq_getstring(pVM, -3, &streamName);

	// Is local file stream ?
	if (bOnlineStream == false){
		streamName = SharedUtility::GetAbsolutePath("files\\%s", streamName);
	}

	// Declare audio
	CAudio *pAudio = new CAudio(streamName, bReplay, bOnlineStream);

	// Succeed ?
	if (!pAudio || !pAudio->Load() || SQ_FAILED(sq_setinstance(pVM, pAudio))){
		CLogFile::Printf("Failed to load audio from file %s", streamName);
		SAFE_DELETE(pAudio);
		sq_pushbool(pVM, false);
		return 1;
	}

	// Add to manager
	pCore->GetAudioManager()->Add(pAudio);

	// Return
	sq_pushbool(pVM, true);
	return (1);
}

SQInteger CAudioNatives::Delete(SQVM * pVM)
{
	// Get the audio instance
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);

	// Is valid instance ?
	if (!pAudio)
	{
		sq_pushbool(pVM, false);
		return (1);
	}

	// Remove file
	pCore->GetAudioManager()->Remove(pAudio);
	sq_setinstance(pVM, NULL);
	// Return
	return (1);
}

SQInteger CAudioNatives::Play(SQVM * pVM)
{
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);
	if (!pAudio)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	sq_pushbool(pVM, pAudio->Play());
	return 1;
}

SQInteger CAudioNatives::Stop(SQVM * pVM)
{
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);
	if (!pAudio)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	pAudio->Stop();
	sq_pushbool(pVM, true);
	return 1;
}

SQInteger CAudioNatives::Pause(SQVM * pVM)
{
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);
	if (!pAudio)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	pAudio->Pause();
	sq_pushbool(pVM, true);
	return 1;
}

SQInteger CAudioNatives::GetVolume(SQVM * pVM)
{
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);
	if (!pAudio)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	sq_pushfloat(pVM, pAudio->GetVolume());
	return 1;
}

SQInteger CAudioNatives::ResetVolume(SQVM * pVM)
{
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);
	if (!pAudio)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	pAudio->Unmute();

	sq_pushbool(pVM, true);
	return 1;
}

SQInteger CAudioNatives::SetVolume(SQVM * pVM)
{
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);
	if (!pAudio)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	float fVolume;
	sq_getfloat(pVM, 2, &fVolume);

	pAudio->SetVolume(fVolume);

	sq_pushbool(pVM, true);
	return 1;
}

void CAudioNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("createAudio", Create, 3, "sbb");
	pScriptingManager->RegisterFunction("deleteAudio", Delete, 0, NULL);
	pScriptingManager->RegisterFunction("playAudio", Play, 0, NULL);
	pScriptingManager->RegisterFunction("stopAudio", Stop, 0, NULL);
	pScriptingManager->RegisterFunction("pauseAudio", Pause, 0, NULL);
	pScriptingManager->RegisterFunction("setVolume", SetVolume, 1, "f");
	pScriptingManager->RegisterFunction("getVolume", GetVolume, 0, NULL);
	pScriptingManager->RegisterFunction("resetVolume", ResetVolume, 0, NULL);
}