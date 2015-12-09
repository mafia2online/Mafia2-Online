/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : CNameTag.h
* Developers : Tyldar <tyldar@m2-online.com>
*
***************************************************************/

#pragma once

class CNameTag
{
private:
	CVector3						m_playerVectors[MAX_PLAYERS];
	CVector3						m_pedVectors[MAX_PEDS];


public:
	CNameTag();
	~CNameTag();

	void							PreAll(void);
	void							PrePlayer(void);
	void							PrePed(void);

	void							All(void);
	void							Player(void);
	void							Ped(void);
};