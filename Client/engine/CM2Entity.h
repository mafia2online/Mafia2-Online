/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2Entity.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#define	FUNC_CEntity__Activate				0x1189E20
#define	FUNC_CEntity__Deactivate			0x1189E70

class M2EntityVFTable
{
public:
	PAD(M2EntityVFTable, pad0, 0x40);			// 0000 - 0040
	DWORD IsVisibleFromCamera;					// 0040 - 0044
	PAD(M2EntityVFTable, pad1, 0x38);			// 0044 - 007C
	DWORD SetPosition;							// 007C - 0080
	DWORD SetDirection;							// 0080 - 0084
	DWORD SetRotation;							// 0084 - 0088
	DWORD SetScale;								// 0088 - 008C
	DWORD GetPosition;							// 008C - 0090
	DWORD GetDirection;							// 0090 - 0094
	DWORD GetRotation;							// 0094 - 0098
	DWORD GetScale;								// 0098 - 009C
	PAD(M2EntityVFTable, pad2, 0x4);			// 009C - 00A0
	DWORD GetModel;								// 00A0 - 00A4
	PAD(M2EntityVFTable, pad3, 0xC);			// 00A4 - 00B0
	DWORD SetModel;								// 00B0 - 00B4
	PAD(M2EntityVFTable, pad4, 0x30);			// 00B4 - 00E4
	DWORD GetTotalSeats;						// 00E4 - 00E8
	PAD(M2EntityVFTable, pad5, 0x38);			// 00E8 - 0120
	DWORD SetHornOn;							// 0120 - 0124
};

class C_RenderSkinnedVisual
{
public:
	PAD(C_RenderSkinnedVisual, pad0, 0x64);		// 0000 - 0064
	DWORD m_dwLastRenderTime;					// 0064 - 0068
};

class C_SingleMesh
{
public:
	PAD(C_SingleMesh, pad0, 0x6C);				// 0000 - 006C
	C_RenderSkinnedVisual* m_pRenderSkinnedVisual; // 006C - 0070
};

class M2Model
{
public:
	PAD(M2Model, pad0, 0xC);					// 0000 - 000C
	DWORD m_dwFlags;							// 000C - 0010
	PAD(M2Model, pad1, 0x14);					// 0010 - 0024
	C_SingleMesh* m_pSingleMesh;				// 0024 - 0028
	PAD(M2Model, pad2, 0xC);					// 0034 - 0038
	Matrix43 m_matPlaceable;					// 0038 - 0068
	PAD(M2Model, pad3, 0xB0);					// 0068 - 0118
	D3DMATRIX m_matWorld;						// 0118 - 0158
};

class M2Entity
{
public:
	M2EntityVFTable	* m_pVFTable;				// 0000 - 0004
	PAD(M2Entity, pad0, 0x18);					// 0004 - 001C
	// 0x15 = M2EntityMessage * pLastMessage;
	DWORD m_dwGUID;								// 001C - 0020
	DWORD m_dwFlags;							// 0020 - 0024
	PAD(M2Entity, pad2, 0x3C);					// 0024 - 0060
	M2Model * m_pModel;							// 0060 - 0064
};

class CM2Entity
{

private:

	M2Entity				* m_pEntity;

public:

							CM2Entity						( M2Entity * pEntity );
							~CM2Entity						( void );
							
	void					SetEntity						( M2Entity * pEntity ) { m_pEntity = pEntity; }
	M2Entity				* GetEntity						( void ) { return m_pEntity; }

	void					Activate						( void );
	void					Deactivate						( void );
	
	DWORD					GetGUID							( void );
	
	void					SetPosition						( CVector3 vecPosition );
	void					GetPosition						( CVector3 * vecPosition, bool bCheckVehicle = false );

	void					SetDirection					( CVector3 vecDirection );
	void					GetDirection					( CVector3 * vecDirection );

	void					SetRotation						( Quaternion quatRotation );
	void					GetRotation						( Quaternion * quatRotation );

	bool					IsOnScreen						( void );

	void					ShowModel						( bool bShow );
	bool					IsModelShowing					( void );
	
};