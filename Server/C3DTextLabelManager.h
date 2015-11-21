/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Server
* File       : C3DTextLabelManager.h
* Developers : Tyldar <tyldar@mafia2-online.com>
*
***************************************************************/

#pragma once

#include	"C3DTextLabel.h"

//class C3DTextLabelManager : public CMultiplayer3DTextLabelManager - BUGGED WHY ?
class C3DTextLabelManager 
{

private:

	C3DTextLabel													* m_3DText[MAX_3DTEXTS];

public:

	C3DTextLabelManager(void);
	~C3DTextLabelManager(void);

	EntityId						Add(const char *text, int color, float X, float Y, float Z, float distance);
	bool							Remove(EntityId textID);
	bool							IsActive(EntityId textID);
	EntityId						FindFreeSlot(void);

};