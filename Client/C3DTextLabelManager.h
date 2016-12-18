/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : C3DTextLabelManager.h
* Developers : Tyldar <tyldar@m2-online.com>
*
***************************************************************/

#pragma once

#include "C3DTextLabel.h"

#define MAX_RENDER_DISTANCE 35.0

class C3DTextLabelManager
{

private:
	C3DTextLabel					* m_p3DTextLabels[MAX_3DTEXTS];
	int								m_count;


public:
									C3DTextLabelManager(void);
									~C3DTextLabelManager(void);

	bool							Add(float fX, float fY, float fZ, String text, int color, float distance);
	bool							Remove(EntityId textID);
	bool							IsActive(EntityId textID);
	bool							IsOnScreen(EntityId textID);
	void							Render(void);
	EntityId						FindFreeSlot(void);
	int								GetCount(void);

	C3DTextLabel					* Get(EntityId textId) { return m_p3DTextLabels[textId]; }
};