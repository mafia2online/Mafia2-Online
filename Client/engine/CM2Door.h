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

class CM2Door
{
public:
	void		Open			(CVector3 * pvPosition);
	void		Kick			(CVector3 * pvPosition);
	void		Close			(void);

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
