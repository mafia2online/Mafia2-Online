/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2SDSTables.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

class M2SDSPlayersTable
{
public:
	PAD(M2SDSPlayersTable, pad0, 0x8);		// 0000 - 0008
	int m_iStartModelOffset;				// 0008 - 000C
	int m_iEndModelOffset;					// 000C - 0010
};

class M2SDSTables // size: 0xB8
{
public:
	PAD(M2SDSTables, pad0, 0x1C);			// 0000 - 001C
	void* m_pPoliceOffences;				// 001C - 0020
	void* m_pWeapons;						// 0020 - 0024
	PAD(M2SDSTables, pad1, 0x4);			// 0024 - 0028
	void* m_pAttackParams;					// 0028 - 002C
	M2SDSPlayersTable* m_pPlayers;			// 002C - 0030
	void* m_pVehicles;						// 0030 - 0034
	PAD(M2SDSTables, pad2, 0x8);			// 0034 - 003C
	void* m_pVehicleColours;				// 003C - 0040
	PAD(M2SDSTables, pad3, 0x1C);			// 0040 - 005C
	void* m_pPhObjSounds2;					// 005C - 0060
	PAD(M2SDSTables, pad4, 0x4);			// 0060 - 0064
	void* m_pMaterialPhysics;				// 0064 - 0068
	void* m_pMaterialShots;					// 0068 - 006C
	void* m_pMusic;							// 006C - 0070
	void* m_pGlassBreaking;					// 0070 - 0074
	void* m_pGlassMatTemplates;				// 0074 - 0078
	PAD(M2SDSTables, pad5, 0x4);			// 0078 - 007C
	void* m_pHumanDamageZones;				// 007C - 0080
	void* m_pPinupsGalleries;				// 0080 - 0084
	void* m_pPinups;						// 0084	- 0088
	PAD(M2SDSTables, pad6, 0x2C);			// 0088 - 00B4
	void* m_pRamboActions;					// 00B4 - 00B8
};