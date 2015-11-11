/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : CAudioManager.cpp
* Developers : Tyldar <tyldar@mafia2-online.com>
*
***************************************************************/

#ifndef CAUDIOMANAGER_H
#define CAUDIOMANAGER_H

#include "CAudio.h"

class CAudioManager
{
private:
	std::list<CAudio *>	 m_audioList;
	bool				 m_bMuted;

public:
	CAudioManager();
	~CAudioManager();

	bool				Initialize();
	void				Add(CAudio * pAudio);
	void				Remove(CAudio * pAudio);
	void				RemoveAll();
	void				MuteAll();
	inline bool			IsMuted() { return m_bMuted; }
	void				UnmuteAll();
	void				Process();
	String				GetYoutubeStreamURL(String link);
};

#endif // CAUDIOMANAGER_H