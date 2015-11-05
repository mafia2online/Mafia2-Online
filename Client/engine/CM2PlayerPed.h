/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2PlayerPed.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

class C_Entity // size: 0x60 (or 0x70 or 0x88) ???
{
public:
	PAD(C_Entity, pad0, 0x60);						// 0000 - 0060
};

class C_HumanEntity // size: 0x220
{
public:
	PAD(C_HumanEntity, pad0, 0x220);				// 0000 - 0220
};

class M2PlayerPed__VFTable
{
public:
	DWORD Deconstructor;							// 0000 - 0004
	DWORD GetType;									// 0004 - 0008
	DWORD m08;										// 0008 - 000C
	DWORD SetupHumanEntity;							// 000C - 0010
};

class M2PlayerPed // size: 0x3D0
{
public:
	M2PlayerPed__VFTable* m_pVFTable;				// 0000 - 0004
	PAD(M2PlayerPed, pad0, 0x3CC);					// 0004 - 03D0
};

//assert(sizeof(M2PlayerPed) == 0x3D0);
//assert(sizeof(C_HumanEntity) == 0xDC);