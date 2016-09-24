/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : CAudio.h
* Developers : Tyldar <tyldar@mafia2-online.com>
*
***************************************************************/

#ifndef CAUDIO_H
#define CAUDIO_H

#include "CString.h"
#include "Math/CVector3.h"

class String;
class CVector3;

class CAudio
{
private:
	String			m_strStreamName;
	bool			m_bReplay;
	bool			m_bIsOnlineStream;
	bool			m_bUsePosition;
	CVector3		m_vecPosition;
	float			m_fRange;
	float			m_fVolume;
	bool			m_bIsMuted;
	DWORD			m_dwChannel;

public:
	CAudio(const String& strStreamName, bool bReplay = false, bool bIsOnlineStream = false);
	~CAudio();

	bool			Load();
	inline bool		IsLoaded() const { return (m_dwChannel != 0); }
	void			Unload();
	bool			Play();
	void			Pause();
	bool			IsPlaying();
	bool			IsPaused();
	bool			IsStalled();
	void			Stop();
	void			SetUsePosition(bool bUsePosition);
	inline bool		GetUsePosition() { return m_bUsePosition; }
	void			SetPosition(const CVector3& vecPosition, float fRange);
	void			GetPosition(CVector3& vecPosition, float& fRange);
	void			SetVolume(float fVolume);
	float			GetVolume();
	void			Mute();
	inline bool		IsMuted() const { return m_bIsMuted; }
	void			Unmute();
	void			Process();
	int				GetLength();
	bool			SetAt(int iTime);
	int				GetAt();
};

#endif // CAUDIO_H
