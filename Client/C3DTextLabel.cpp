/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : C3DTextLabel.cpp
* Developers : Tyldar <tyldar@m2-online.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "CString.h"
#include "Math/CVector3.h"

#include "C3DTextLabel.h"

C3DTextLabel::C3DTextLabel(EntityId serverID, float fX, float fY, float fZ, String text, int color, float distance)
{
	m_serverID = serverID;
	m_fX = fX;
	m_fY = fY;
	m_fZ = fZ;
	m_text = text;
	m_color = color;
	m_distance = distance;
}

C3DTextLabel::~C3DTextLabel(void)
{
	this->SetActive(false);
}

CVector3	* C3DTextLabel::GetPosition(CVector3 * pos)
{
	pos->fX = m_fX;
	pos->fY = m_fY;
	pos->fZ = m_fZ;

	return (pos);
}

void	C3DTextLabel::SetPosition(CVector3 pos)
{
	this->m_fX = pos.fX;
	this->m_fY = pos.fY;
	this->m_fZ = pos.fZ;
}