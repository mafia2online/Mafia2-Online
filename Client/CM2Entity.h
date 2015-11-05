/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2Entity.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
			   Hopkins
*
***************************************************************/

#pragma once

#include "CM2Enums.h"

class M2ScriptEntity
{
public:
	PAD(M2ScriptEntity, pad0, 0x64);			// 0000 - 0064
	int m_iScriptMachineNumber;					// 0064 - 0068
};

class M2EntityUnkClass002
{
public:
	PAD(M2EntityUnkClass002, pad0, 0xC);		// 0000 - 000C
	M2ScriptEntity * m_pScriptEntity;			// 000C - 0010
};

class M2EntityUnkClass001
{
public:
	PAD(M2EntitiyUnkClass001, pad0, 0x8);		// 0000 - 0008
	M2EntityUnkClass002 * pUnk002;				// 0008 - 000C
};

class M2EntityUnkClass003
{
public:
	PAD(M2EntityUnkClass003, pad0, 0x44);		// 0000 - 0044
	void * m_pUnknown;
};

class M2EntityData
{
public:
};

class M2EntityInventory
{
public:
	PAD(M2EntityInventory, pad0, 0x170);		// 0000 - 0170
	DWORD m_dwSelectedWeapon;					// 0170 - 0174
};

class M2WorldEntity // Actuall it's M2RenderEntity
{
public:
	PAD(M2WorldEntity, pad0, 0xC);				// 0000 - 000C
	DWORD m_dwFlags;							// 000C - 0010
	PAD(M2WorldEntity, pad1, 0x28);				// 0010 - 0038
	D3DMATRIX m_matWorld;						// 0038 - 0078
};

class M2HumanInventory
{
public:
	PAD(M2WeaponData, pad0, 0xE8);				// 0000 - 00E8
	bool m_bInfiniteAmmo;						// 00E8 - 00E9
};

// 110 functions
class M2EntityVFTable
{
public:
	DWORD Constructor;							// 0000 - 0004
	PAD(M2EntityVFTable, pad0, 0x78);			// 0004 - 007C
	DWORD SetPosition;							// 007C - 0080
	DWORD SetDirection;							// 0080 - 0084
	DWORD SetRotation;							// 0084 - 0088
	DWORD m88;									// 0088 - 008C
	DWORD GetPosition;							// 008C - 0090
	DWORD GetDirection;							// 0090 - 0094
	DWORD GetRotation;							// 0094 - 0098
	DWORD m98;									// 0098 - 009C
	DWORD m9C;									// 009C - 00A0
	DWORD GetWorldEntity;						// 00A0 - 00A4
	PAD(M2EntityVFTable, pad1, 0xC);			// 00A4 - 00B0
	DWORD SetWorldEntity;						// 00B0 - 00B4

	// 0x40 = IsVisibleFromCamera
};

// Move into CMatrix.h? (/Shared/Math/CMatrix.h)
class CMatrix4x3
{
public:
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
};

// 0x20 - 0x21 = byteFlags (1 byte)
// 0x33 - 0x3A = byteFlags (1 byte)
class M2Entity
{
public:
	M2EntityVFTable * m_pVFTable;				// 0000 - 0004
	// 0x4 = Render entity
	//  + 0x74 = path to model
	// 0x8 = something with EntityDataStorage
	// 0x10 = Unknown class
	PAD(M2Entity, pad0, 0x18);					// 0004 - 001C
	DWORD m_dwGUID;								// 001C - 0020
	M2EntityUnkClass001 * m_pUnk001;			// 0020 - 0024
	// 0x28 = ?
	// 0x2C = ?
	PAD(M2Entity, pad1, 0x3C);					// 0024 - 0060
	// 0x4C = SetTextId( )
	M2WorldEntity * m_pWorld;					// 0060 - 0064
	M2Vehicle * m_pVehicle;						// 0064 - 0068
	PAD(M2Entity, pad2, 0x38);					// 0064 - 009C
	M2HumanInventory * m_pHumanInventory;		// 00A0 - 00A4
	M2EntityData * m_pEntityData;				// 00A4 - 00A8
	//PAD(M2Entity, pad4, 0x4);					// 00A8 - 00AC void* m_pMeleeData;
	M2EntityUnkClass003 * m_pUnknown;			// 00A8 - 00AC
	M2EntityInventory * m_pInventory;			// 00AC - 00B0
	void * m_pThrowingInventory;				// 00B0 - 00B4
	PAD(M2Entity, pad5, 0x1C);					// 00B4 - 00D0
	float m_fHealth;							// 00D0 - 00D4
	float m_fMaxHealth;							// 00D4 - 00D8
	PAD(M2Entity, pad6, 0x10);					// 00D8 - 00E8
	bool m_bInvulnerable;						// 00E8 - 00E9
	PAD(M2Entity, pad8, 0x1F7);					// 00E9 - 02E0
	// 0x240 - MIGHT BE START OF CONTROLS
	DWORD m_dwState;							// 02E0 - 02E4 (this could be keystate (+0x4 = up/down key))
	PAD(M2Entity, pad9, 0x5C);					// 02E4 - 0340
	bool m_bCrouching;							// 0340 - 0341

	// 0x3C8 = (const char *) ControlStyleStr (Set to CS_NORMAL and doesn't change!)
	// 0x310 = State
	//  - 0 = Nothing
	//	- 1 = Moving
	//	- 4 = In cover
	//	- 5 = Opening/closing door
	//	- 10 = Using phone


	// 0x1D0 = ??
		// - 0 = String what we're walking on (IN CZECH)
		
	// 0x240 = controls (size: 0xA8)
		// 0x19 = unknown
		// 0x1A = unknown
		// 0x1B = unknown
		// 0x1C = unknown
		// 0x28 = flags
		// 0xA0 = m_dwState (which i got at 0x2E0)
};

// 17 functions
class M2BaseEntityVFTable
{
public:
	DWORD Constructor;							// 0000 - 0004
	DWORD GetType;								// 0004 - 0008 (mov eax, 0Eh)
	DWORD Deconstructor;						// 0008 - 000C (xor eax, eax)
	DWORD m0C;									// 000C - 0010
	DWORD m10;									// 0010 - 0014 (Get lua wrapper)
	DWORD IsValid;								// 0014 - 0018
	DWORD IsInvalid;							// 0018 - 001C
	DWORD IsValid_2;							// 001C - 0020 (same code as 0x10)
	DWORD Activate;								// 0020 - 0024
	DWORD Deactivate;							// 0024 - 0028
	DWORD Restore;								// 0028 - 002C (I named it IsActive in IDA - maybe this at 0x2C are switched?)
	DWORD IsActive;								// 002C - 0030
	DWORD m30;									// 0030 - 0034
	DWORD m34;									// 0034 - 0038
	DWORD m38;									// 0038 - 003C
	DWORD m3C;									// 003C - 0040
	DWORD m40;									// 0040 - 0044
};

class M2BaseEntity
{
public:
	M2BaseEntityVFTable * m_pVFTable;			// 0000 - 0004
	DWORD m_dwFlags;							// 0004 - 0008
	M2Entity * m_pEntity;						// 0008 - 000C
};

class M2LocalPlayer
{
public:
	PAD(M2LocalPlayer, pad0, 0x10);				// 0000 - 0010
	M2Entity * m_pEntity;						// 0010 - 0014
	void * m_pDataStorage;
	// 0x74 = Path to model
};

class CM2Entity
{

private:

	M2Entity			* m_pEntity;
	M2BaseEntity		* m_pBaseEntity;
	CM2Entity			* m_pPosEntity;

public:

	CM2Entity( void );
	CM2Entity( M2Entity * pEntity );
	CM2Entity( M2BaseEntity * pBaseEntity, M2BaseEntity * pBasePosEntity );
	~CM2Entity( void );

	void						SetEntity( M2Entity * pEntity ) { m_pEntity = pEntity; }
	M2Entity					* GetEntity( void ) { return m_pEntity; }

	void						SetBaseEntity( M2BaseEntity * pBaseEntity ) { m_pBaseEntity = pBaseEntity; }
	M2BaseEntity				* GetBaseEntity( void ) { return m_pBaseEntity; }

	CM2Entity					* GetPosEntity( void ) { return m_pPosEntity; }

	void						Activate( void );
	void						Deactivate( void );
	int							GetScriptMachine( void );
	void						SetSelectedWeapon( DWORD dwWeapon, bool bUseAnimation );
	DWORD						GetSelectedWeapon( void );
	void						SetPosition( CVector3 vecPosition );
	void						GetPosition( CVector3 * vecPosition );
	void						SetDirection( CVector3 vecDirection );
	void						GetDirection( CVector3 * vecDirection );
	void						SetRotation( CVector3 vecRotation );
	void						GetRotation( CVector3 * vecRotation );
	void						SetHealth( float fHealth );
	float						GetHealth( void );
	float						GetMaxHealth( void );
	float						GetRealHealth( void );
	DWORD						GetGUID( void );
	bool						IsCrouching( void );
	void						GiveWeapon( DWORD dwWeapon, DWORD dwAmmo );
	void						RemoveWeapon( DWORD dwWeapon, DWORD dwAmmo = 0 );
	void						ReloadWeapon( void );
	M2SyncObject				* MoveVec( CVector3 vecPosition, M2Enums::eMoveType moveType, CVector3 vecEndDirection );
	M2SyncObject				* ShootAt( CVector3 vecPosition, float fTime );
	M2SyncObject				* AimAt( CVector3 vecPosition, bool bToggle = true );
	M2SyncObject				* LookAt( CVector3 vecPosition, float fTime );
	void						ShowHat( bool bShow );
	void						DropHat( bool bDrop );
	void						ToggleStealthKill( bool bToggle );
	void						EnablePushByPlayer( bool bEnable );
	void						SetReactions( bool bKillFromBehind, bool bBounce, bool bPunch );
	void						LockControls( bool bLock, bool bResetAnimation = true );
	bool						AreControlsLocked( void );
	bool						InventoryHaveItem( DWORD dwItem );
	void						SetInvulnerable( bool bInvulnerable );
	void						GiveMoney( int iAmount );
	void						RemoveMoney( int iAmount );
	int							GetMoney( void );
	int							GetControlStyle( void );
	void						SetControlStyle( int iStyle );
	void						SetUnlimitedAmmo( bool bToggle );
	bool						HasUnlimitedAmmo( void );
	bool						HaveThrowingWeaponInHand( void );
	bool						IsOnScreen( void );
	void						AnimationReset( void );
	void						SetState( DWORD dwState );
	DWORD						GetState( void );

	// Vehicle
	void						PutInVehicle( M2Vehicle * pVehicle, M2Enums::eVehicleSeat seat, bool bEnter = true, bool bAnimate = true );
	M2Vehicle					* GetVehicle( void );

	// Don't work/don't use(fri's playground :P)
	void						CreateMapIcon( char szUsername );
	void						DestroyMapIcon( char * szUsername );
	void						ChangeModel( int iFolder, String strModel );
	void						PlayAnimation( char strAnimation );
	void						StopAnimation( char strAnimation );
	bool						IsAnimFinished( char strAnimation );

};
