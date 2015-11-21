/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : CAudioManager.cpp
* Developers : Tyldar <tyldar@mafia2-online.com>
*
***************************************************************/

#include "StdInc.h"

extern CCore * pCore;

CAudioManager::CAudioManager() : m_bMuted(false)
{

}

CAudioManager::~CAudioManager()
{
	// Remove all audio
	RemoveAll();

	// Free BASS audio library
	BASS_Free();
}

bool CAudioManager::Initialize()
{
	// BASS version check
	if (HIWORD(BASS_GetVersion()) != BASSVERSION) {
		CLogFile::Printf("Invalid BASS version");
		return false;
	}

	// Initialize BASS audio library
	if(BASS_Init(-1, 44100, 0, 0, NULL) == TRUE)
		return true;
	else {
		CLogFile::Printf("Init error : %d", BASS_ErrorGetCode());
		return false;
	}
}

void CAudioManager::Add(CAudio * pAudio)
{
	m_audioList.push_back(pAudio);

	// Are we muted?
	if (m_bMuted)
		pAudio->Mute();
}

void CAudioManager::Remove(CAudio * pAudio)
{
	m_audioList.remove(pAudio);
	SAFE_DELETE(pAudio);
}

void CAudioManager::RemoveAll()
{
	for (auto pAudio : m_audioList)
		SAFE_DELETE(pAudio);

	m_audioList.clear();
}

void CAudioManager::MuteAll()
{
	if (m_bMuted)
		return;

	for (auto pAudio : m_audioList)
		pAudio->Mute();

	m_bMuted = true;
}

void CAudioManager::UnmuteAll()
{
	if (!m_bMuted)
		return;

	for (auto pAudio : m_audioList)
		pAudio->Unmute();

	m_bMuted = false;
}

void CAudioManager::Process()
{
	for (auto pAudio : m_audioList)
		pAudio->Process();
}

String CAudioManager::GetYoutubeStreamURL(String link)
{
	String idBuffer;
	if (SharedUtility::GetHTTPHeaderAndData("kingofmetin.com", String("/yt.php?url=%s", link.Get()).Get(), "", NULL, &idBuffer))
		return idBuffer;
	
	return String("");
}