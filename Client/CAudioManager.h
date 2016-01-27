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

#include <list>

class CAudio;
class String;

class CAudioManager
{
private:
	std::list<CAudio *>	 m_audioList;
	bool				 m_bMuted;

public:
	CAudioManager();
	~CAudioManager();

	bool				Initialize(void);
	void				Add(CAudio * pAudio);
	void				Remove(CAudio * pAudio);
	void				RemoveAll(void);
	void				MuteAll(void);
	inline bool			IsMuted(void)  const { return m_bMuted; }
	void				UnmuteAll(void);
	void				Process(void);
	String				GetYoutubeStreamURL(const String& link);
};

#endif // CAUDIOMANAGER_H
