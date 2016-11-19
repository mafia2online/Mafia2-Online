/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2Core.h
* Developers : h0pk1nz <haha_fuck_u@hotmail.com>
*
***************************************************************/

#pragma once

#include "Math/CVector3.h"

class M2Model;

class M2Door : public M2Entity
{
public:
	PAD(M2Door, pad0, 0x320);		// 0000 - 0320
	int status;						// 0320 - ??

	void Open(CVector3 *position, bool sound, int unk);
	void Close();

	void Lock();
	void Unlock();

	void EnableAction();
	void DisableAction();
	
	void Kick(const CVector3 *);
};

class CM2Door : public CM2Entity
{

private:
	M2Door		* m_pDoor;

public:
	CM2Door(M2Door *);
	~CM2Door();

	void		SetDoor			(M2Door *door){ m_pDoor = door; }
	M2Door		*GetDoor		(void){ return m_pDoor; }

	void		Open			(CVector3 * pvPosition, bool sound);
	void		Close			(void);

	void		Kick(CVector3 * pvPosition);

	void		Lock			(void);
	void		Unlock			(void);

	void		EnableAction	(void);
	void		DisableAction	(void);

	void		RealLock		(void);
	void		RealUnlock		(void);

	__int64		GetHash			(void);

	const bool	IsClosed		(void) const;
	const bool	IsLocked		(void) const;

	M2Model *	GetModel		(void);
};
