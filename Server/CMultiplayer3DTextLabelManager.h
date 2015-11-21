/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Server
* File       : CMultiplayer3DTextLabelManager.h
* Developers : Tyldar <tyldar@mafia2-online.com>
*
***************************************************************/

#pragma once

class CMultiplayer3DTextLabelManager
{

public:

	virtual	unsigned int					Add(const char *text, int color, float X, float Y, float Z, float distance);
	virtual	bool							Remove(unsigned int uiBlipId) = 0;
	/*virtual	bool							IsActive(unsigned int uiBlipId) = 0;

	virtual CMultiplayerBlip				* Get(unsigned int uiBlipId) = 0;
	virtual	unsigned int					GetCount(void) = 0;*/

};