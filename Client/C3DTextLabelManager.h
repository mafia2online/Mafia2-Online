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
	bool							IsActive(EntityId textID) const;
	bool							IsOnScreen(EntityId textID) const;
	void							Render(void);
	EntityId						FindFreeSlot(void) const;
	int								GetCount(void) const;

	C3DTextLabel					* Get(EntityId textId) const { return m_p3DTextLabels[textId]; }
};