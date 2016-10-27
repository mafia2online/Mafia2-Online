/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : CAudioManager.cpp
* Developers : Tyldar <tyldar@mafia2-online.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCommon.h"

#include "CAudio.h"
#include "CAudioManager.h"

#include "CLogFile.h"
#include "CString.h"
#include "SharedUtility.h"

#include "bass.h"

CAudioManager::CAudioManager()
	: m_audioList()
	, m_bMuted(false)
{
}

CAudioManager::~CAudioManager()
{
	RemoveAll();
	BASS_Free();
}

bool CAudioManager::Initialize()
{
	if (HIWORD(BASS_GetVersion()) != BASSVERSION)
	{
		CLogFile::Printf("Invalid BASS version");
		return false;
	}

	if (BASS_Init(-1, 44100, 0, 0, NULL) == TRUE)
	{
		return true;
	}
	else
	{
		CLogFile::Printf("BASS Init error: %d", BASS_ErrorGetCode());
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

String CAudioManager::GetYoutubeStreamURL(const String& link)
{
	String idBuffer;

	// NOTE(zaklaus): Seriously, why?
	/*
	 *if (SharedUtility::GetHTTPHeaderAndData("kingofmetin.com", String("/yt.php?url=%s", link.Get()).Get(), "", NULL, &idBuffer))
		//return idBuffer;
*	 */

	return String("");
}
