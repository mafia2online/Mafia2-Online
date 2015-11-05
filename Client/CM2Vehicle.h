/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2Vehicle.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"CM2Enums.h"

class M2Entity;
class CM2Entity;
class M2WorldEntity;

class M2VehicleDataVFTable
{
public:
	PAD(M2VehicleDataVFTable, pad0, 0x88);		// 0000 - 0088
	DWORD SetSPZText;							// 0088 - 008C
	DWORD GetSPZText;							// 008C - 0090
};

class M2VehicleData
{
public:
	M2VehicleDataVFTable * m_pVFTable;			// 0000 - 0004
};

class M2VehicleSeat
{
public:
	void * m_pVFTable;							// 0000 - 0004
	M2Entity * m_pOccupant;						// 0004 - 0008
};

class M2VehicleVFTable
{
public:
	PAD(M2VehicleVFTable, pad0, 0xA0);			// 0000 - 00A0
	DWORD GetWorldEntity;						// 00A0 - 00A4
	PAD(M2VehicleVFTable, pad1, 0x40);			// 00A4 - 00E4
	DWORD GetTotalSeats;						// 00E4 - 00E8
	PAD(M2VehicleVFTable, pad2, 0x4);			// 00E8 - 00EC
	DWORD GetSeatAtIndex;						// 00EC - 00F0
	PAD(M2VehicleVFTable, pad3, 0x30);			// 00F0 - 0120
	DWORD SetHornOn;							// 0120 - 0124
};

class M2Vehicle
{
public:
	M2VehicleVFTable * m_pVFTable;				// 0000 - 0004
	PAD(M2Vehicle, pad0, 0x18);					// 0004 - 001C
	DWORD m_dwGUID;								// 001C - 0020
	PAD(M2Vehicle, pad1, 0x40);					// 0020 - 0060
	M2WorldEntity * m_pWorld;					// 0060 - 0064
	PAD(M2Vehicle, pad2, 0x44);					// 0064 - 00A0
	M2VehicleData * m_pVehicleData;				// 00A8 - 00AC
	PAD(M2Vehicle, pad3, 0x1DC);				// 00AC - 0288
	CVector3 m_vecMoveSpeed;					// 0288 - 0290
	PAD(M2Vehicle, pad4, 0x4);					// 0290 - 0294
	float m_fSpeed;								// 0294 - 029C
	PAD(M2Vehicle, pad5, 0x484);				// 029C - 071C
	float m_fDirtLevel;							// 071C - 0720
	PAD(M2Vehicle, pad6, 0x588);				// 0720 - 0CA8
	BYTE m_byteFlags1;							// 0CA8 - 0CA9 (& 0x10 = has seats?)
	BYTE m_byteFlags2;							// 0CA9 - 0CAA
	BYTE m_byteFlags3;							// 0CAA - 0CAB
	BYTE m_byteFlags4;							// 0CAB - 0CAC
	BYTE m_byteFlags5;							// 0CAC - 0CAD
	PAD(M2Vehicle, pad7, 0x3F);					// 0CAD - 0CEC
	int m_iTuningTable;							// 0CEC - 0CF0

	// 0xFC = resistance flags
	// 0xD48 = painting (const char or int)
	// 0xE6C = spent money (how much was spend on upgrades)
	// 0xE94 = speed limiter speed (float * 3.599999904632568)
	// 0xE98 = resistance (float * 100.0)
	// 0xE9C = respond to events (bool) - called from CVehicle__RegisterEvents
	// 0x918 = damage flags (& 0x10000 = godmode)
	// 0x85C = horn flags (|= 1u; - on, &= 0xFFFFFFFEu; - off)
	// 0x838 = get spz text - plate text (const char *)
	// 0x8E0 = Plate text
};

class M2VehicleBaseEntityVFTable
{
public:
};

class M2VehicleBaseEntity
{
public:
	M2VehicleBaseEntityVFTable * m_pVFTable;	// 0000 - 0004
	PAD(M2VehicleBaseEntity, pad0, 0x4);		// 0004 - 0008
	M2Vehicle * m_pVehicle;						// 0008 - 000C
};

class CM2Vehicle
{

private:

	M2VehicleBaseEntity * m_pBaseVehicle;
	M2Vehicle * m_pVehicle;
	
public:

	CM2Vehicle( M2VehicleBaseEntity * pBaseVehicle );
	CM2Vehicle( M2Vehicle * pVehicle );
	~CM2Vehicle( void );

	void						SetBaseVehicle( M2VehicleBaseEntity * pBaseVehicle ) { m_pBaseVehicle = pBaseVehicle; }
	M2VehicleBaseEntity			* GetBaseVehicle( void ) { return m_pBaseVehicle; }

	void						SetVehicle( M2Vehicle * pVehicle ) { m_pVehicle = pVehicle; }
	M2Vehicle					* GetVehicle( void ) { return m_pVehicle; }

	DWORD						GetGUID( void );
	void						SetPosition( CVector3 vecPosition );
	void						GetPosition( CVector3 * vecPosition );
	void						SetDirection( CVector3 vecDirection );
	void						GetDirection( CVector3 * vecDirection );
	void						SetMoveSpeed( CVector3 vecMoveSpeed );
	void						GetMoveSpeed( CVector3 * vecMoveSpeed );
	void						SetSpeed( float fSpeed );
	float						GetSpeed( void );
	bool						IsSpeedLimited( void );
	void						OpenHood( void );
	void						CloseHood( void );
	void						OpenTrunk( void );
	void						CloseTrunk( void );
	void						SetDirtLevel( float fDirtLevel );
	float						GetDirtLevel( void );
	void						SetHandbrake( bool bToggle );
	void						ToggleEngine( bool bToggle, bool bRevOnStart = true );
	void						SetTuningTable( int iTable );
	int							GetTuningTable( void );
	void						SetPlateText( const char * szText );
	const char					* GetPlateText( void );
	void						SetPainting( const char * szPainting );
	void						DisableExplosions( bool bDisable );
	void						SetBeaconLightOn( bool bToggle );
	void						SetHornOn( bool bToggle );
	void						SetWheelsProtected( bool bProtected );
	bool						AreWheelsProtected( void );
	void						OpenSeatWindow( M2Enums::eVehicleSeat seat, bool bOpen );
	M2VehicleSeat				* GetSeat( int iSeat );
	int							GetSeat( CM2Entity * pEntity );

	void						Repair( float fDirtLevel );
	float						GetEngineDamage();
	void						SetEngineDamage( float fDamage );
	int							GetDamage();
	void						SetDamage( int iDamage );

};