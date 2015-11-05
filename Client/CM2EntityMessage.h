/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2EntityMessage.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
			   Hopkins
*
***************************************************************/

#pragma once

class M2EntityMessage
{
public:
	void * m_pVFTable;							// 0000 - 0004
	PAD(M2EntityMessage, pad0, 0x4);			// 0004 - 0008
	DWORD m_dwSenderGUID;						// 0008 - 000C
	DWORD m_dwReceiveGUID;						// 000C - 0010
	DWORD m_dwMessage;							// 0010 - 0014
	DWORD m_dwUnknown1;							// 0014 - 0018
	DWORD m_dwUnknown2;							// 0018 - 001C
	DWORD M2DamageMessage__dwEnemyGUID;         // 001C - 0020
	PAD(M2EntityMessage, pad1, 0x8);			// 0020 - 0028
	DWORD M2HumanDeathMessage__dwKillerGUID;    // 0028 - 002C
};

class M2ShotMessage
{
public:
	PAD(M2ShotMessage, pad0, 0x1C);				// 0000 - 001C
	CVector3 m_vecShotPos;						// 001C - 0024
};

class CM2EntityMessage
{

public:

	static bool		HandleEntityEvent( M2EntityMessage * pMessage );

};