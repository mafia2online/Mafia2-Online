/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : C3DTextLabel.cpp
* Developers : Tyldar <tyldar@m2-online.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CIcon.h"

CIcon::CIcon(EntityId iconId, int type, CVector3 pos, float renderDistance)
{
	this->iconId = iconId;
	this->type = type;
	this->pos = pos;
	this->renderDistance = renderDistance;
	this->active = false;
}

CIcon::~CIcon()
{
	this->active = false;
}

CIcon&	CIcon::operator	= (const CIcon& icon)
{
	this->iconId = icon.GetIconId();
	this->type = icon.GetType();
	icon.GetPosition(this->pos);
	this->renderDistance = icon.GetRenderDistance();
	this->active = icon.GetActive();
	return (*this);
}

CVector3& CIcon::GetPosition(CVector3& pos) const
{
	pos.fX = this->pos.fX;
	pos.fY = this->pos.fY;
	pos.fZ = this->pos.fZ;

	return (pos);
}

void	CIcon::SetPosition(const CVector3& pos)
{
	this->pos = pos;
}