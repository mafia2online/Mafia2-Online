/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : CIcon.h
* Developers : Tyldar <tyldar@m2-online.com>
*
***************************************************************/

#pragma once

#include "Math/CVector3.h"

class CIcon
{
private:
	EntityId	iconId;
	int			type;
	CVector3	pos;
	float		renderDistance;
	bool		active;
public:
	CIcon(EntityId iconId, int type, CVector3 pos, float renderDistance);
	~CIcon();

	CIcon&	operator = (const CIcon& icon);

	void		SetIconId(EntityId newEnt){ iconId = newEnt; }
	EntityId	GetIconId() const { return iconId; }

	void		SetActive(bool act) { active = act; }
	bool		GetActive() const { return active; }

	void		SetType(int newT){ type = newT; }
	int			GetType() const { return type; }

	void		SetPosition(const CVector3& pos);
	CVector3&	GetPosition(CVector3& pos) const;

	void		SetRenderDistance(float dis){ renderDistance = dis; }
	float		GetRenderDistance() const { return renderDistance; }
};