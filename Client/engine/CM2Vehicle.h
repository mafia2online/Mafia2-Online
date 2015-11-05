/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2Vehicle.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

enum eVehiclePartType
{
	PART_TYPE_WHEEL					= 2,
	PART_TYPE_DOOR					= 4,
	PART_TYPE_WINDOW				= 5,
	PART_TYPE_HOOD_BOOT				= 6,
	PART_TYPE_BUMPER				= 7,
	PART_TYPE_ENGINE				= 13,
	PART_TYPE_TYRE					= 14
};

class M2VehicleDataVFTable
{
public:
	PAD(M2VehicleDataVFTable, pad0, 0x88);		// 0000 - 0088
	DWORD SetSPZText;							// 0088 - 008C
	DWORD GetSPZText;							// 008C - 0090
	DWORD SetDirtLevel;							// 0090 - 0094
};

class M2VehiclePart
{
public:
	PAD(M2VehiclePart, pad0, 0x4);				// 0000 - 0004
	eVehiclePartType m_partType;				// 0004 - 0008
	BYTE m_byteFlags;							// 0008 - 0009
	PAD(M2VehiclePart, pad1, 0x13);				// 0009 - 001C
	float m_fRealHealth;						// 001C - 0020
	float m_fMaxHealth;							// 0020 - 0024
	float m_fHealth;							// 0024 - 0028
};

class M2VehicleData
{
public:
	M2VehicleDataVFTable * m_pVFTable;			// 0000 - 0004
	PAD(M2VehicleData, pad0, 0x1C);				// 0004 - 0020
	void * m_pVehicleParts;						// 0020 - 0024
};

class M2FuelTank
{
public:
	PAD(M2FuelTank, pad0, 0x1CC);				// 0000 - 01CC
	float m_fFuel;								// 01CC - 01D0
	float m_fFuelTankCapacity;					// 01D0 - 01D4
};

class M2VehicleUnk004
{
public:
	M2FuelTank * m_pFuelTank;					// 0000 - 0004
};

class M2VehicleUnk003
{
public:
	PAD(M2VehicleUnk002, pad0, 0x8);			// 0000 - 0008
	M2VehicleUnk004 * m_pUnknown;				// 0008 - 000C
};

class M2VehicleUnk002
{
public:
	PAD(M2VehicleUnk002, pad0, 0x8);			// 0000 - 0008
	BYTE m_byteUnknown;							// 0008 - 0009
};

class M2VehicleUnk001
{
public:
	PAD(M2VehicleUnk002, pad0, 0x4C);			// 0000 - 004C
	M2VehicleUnk002 * m_pUnknown;				// 004C - 0050
};

class M2VehicleSeat
{
public:
	void* m_pVFTable;							// 0000 - 0004
	M2Ped * m_pOccupant;						// 0004 - 0008
};

class M2VehicleSeats
{
public:
	M2VehicleSeat * m_pSeat[4];					// 0000 - 000C
};

class M2VehicleWheels
{
public:
	PAD(M2VehicleWheels, pad0, 0x10);			// 0000 - 0010
	char m_szWheelTexture[11];					// 0010 - 001B
};

class M2Vehicle : public M2Entity
{
public:
												// 0000 - 0064
	PAD(M2Vehicle, pad0, 0x14);					// 0064 - 0078
	int m_nSlotSDS;								// 0078 - 007C
	PAD(M2Vehicle, pad1, 0xC);					// 007C - 0088
	M2VehicleSeats * m_pSeats;					// 0088 - 008C
	PAD(M2Vehicle, pad2, 0x1C);					// 008C - 00A8
	M2VehicleData m_vehicleData;				// 00A8 - 00CC
	PAD(M2Vehicle, pad3, 0x1BC);				// 00CC - 0288
	CVector3 m_vecMoveSpeed;					// 0288 - 0294
	float m_fEngineSpeed;						// 0294 - 0298
	float m_fSpeed;								// 0298 - 029C
	PAD(M2Vehicle, pad4, 0x3C);					// 029C - 02D8
	M2VehicleWheels * m_pWheels;				// 02D8 - 02DC
	PAD(M2Vehicle, pad5, 0x10);					// 02DC - 02EC
	M2VehicleUnk003 * m_pFuelUnk;				// 02EC - 02F0
	PAD(M2Vehicle, pad6, 0x144);				// 02F0 - 0434
	float m_fBrake;								// 0434 - 0438
	PAD(M2Vehicle, pad7, 0x8);					// 0438 - 0440
	float m_fPower;								// 0440 - 0444
	PAD(M2Vehicle, pad8, 0x4C);					// 0444 - 0490
	float m_fSteer;								// 0490 - 0494
	float m_fAddedSteer;						// 0494 - 0498
	float m_fMaxSteerAngle;						// 0498 - 049C
	PAD(M2Vehicle, pad9, 0x264);				// 02F0 - 0700
	DWORD m_dwColourRefreshFlags;				// 0700 - 0704
	float m_fPrimaryRed;						// 0704 - 0708
	float m_fPrimaryGreen;						// 0708 - 070C
	float m_fPrimaryBlue;						// 070C - 0710
	float m_fSecondaryRed;						// 0710 - 0714
	float m_fSecondaryGreen;					// 0714 - 0718
	float m_fSecondaryBlue;						// 0718 - 071C
	float m_fDirtLevel;							// 071C - 0720
	PAD(M2Vehicle, pad10, 0x13C);				// 0720 - 085C
	BYTE m_byteHornFlags;						// 085C - 085D
	PAD(M2Vehicle, pad11, 0x83);				// 085D - 08E0
	char m_szPlateText[6];						// 08E0 - 08E6
	PAD(M2Vehicle, pad12, 0x3A6);				// 08E6 - 0C8C
	M2VehicleUnk001 * m_pUnknown;				// 0C8C - 0C90
	PAD(M2Vehicle, pad13, 0x18);				// 0C90 - 0CA8
	BYTE m_byteFlags1;							// 0CA8 - 0CA9
	BYTE m_byteFlags2;							// 0CA9 - 0CAA
	BYTE m_byteFlags3;							// 0CAA - 0CAB
	BYTE m_byteFlags4;							// 0CAB - 0CAC
	BYTE m_byteFlags5;							// 0CAC - 0CAD
	PAD(M2Vehicle, pad14, 0x3F);				// 0CA9 - 0CEC
	int m_iTuningTable;							// 0CEC - 0CF0
	PAD(M2Vehicle, pad15, 0x17C);				// 0CF0 - 0E6C
	float m_fSpentMoney;						// 0E6C - 0E70
	PAD(M2Vehicle, pad16, 0xC);					// 0E70 - 0E7C
	int m_iLightState;							// 0E7C - 0E80
	PAD(M2Vehicle, pad17, 0x13);				// 0E81 - 0E94
	float m_fSpeedLimiterSpeed;					// 0E94 - 0E98

	// 0xD48 = m_szPainting
	// 0x49C = float m_fSteerAngle
	// 0x4A0 = float m_fMaxTurnAngle
	// 0x4A4 = float m_fTurnAngle
};

class CM2Vehicle : public CM2Entity
{

private:

	M2Vehicle				* m_pVehicle;

public:

							CM2Vehicle						( M2Vehicle * pVehicle );
							~CM2Vehicle						( void );

	void					SetVehicle						( M2Vehicle * pVehicle ) { m_pVehicle = pVehicle; }
	M2Vehicle				* GetVehicle					( void ) { return m_pVehicle; }

	void					SetEngineOn						( bool bEngine, bool bRevWhenStarted = true );
	bool					IsEngineOn						( void );

	void					OpenHood						( void );
	void					CloseHood						( void );
	
	void					OpenTrunk						( void );
	void					CloseTrunk						( void );

	void					SetEngineDamage					( float fEngineDamage );
	float					GetEngineDamage					( void );

	void					SetSpeed						( float fSpeed );
	float					GetSpeed						( void );

	void					SetMoveSpeed					( CVector3 vecMoveSpeed );
	void					GetMoveSpeed					( CVector3 * vecMoveSpeed );

	void					SetDirtLevel					( float fDirtLevel );
	float					GetDirtLevel					( void );

	void					SetSpeedLimited					( bool bSpeedLimited );
	bool					IsSpeedLimited					( void );

	void					SetSpeedLimiterSpeed			( float fSpeed );
	float					GetSpeedLimiterSpeed			( void );

	void					SetTuningTable					( int iTuningTable );
	int						GetTuningTable					( void );

	void					SetColour						( CColor primary, CColor secondary );
	void					GetColour						( CColor * primary, CColor * secondary );

	void					SetFuel							( float fFuel );
	float					GetFuel							( void );

	float					GetFuelTankCapacity				( void );

	void					SetHornOn						( bool bHornOn );
	bool					IsHornOn						( void );

	void					SetSirenOn						( bool bSirenOn );
	bool					IsSirenOn						( void );

	void					SetPlateText					( const char * szText );
	const char				* GetPlateText					( void );

	void					SetVehiclePart					( int iPart, bool bToggle );
	bool					GetVehiclePart					( int iPart );

	void					Repair							( void );
	void					Explode							( void );

	void					AddSteer						( float fSteer );
	float					GetAddedSteer					( void );

	float					GetSteer						( void );

	void					SetSpentMoney					( float fSpentMoney );
	float					GetSpentMoney					( void );

	void					SetPower						( bool bPower );
	bool					GetPower						( void );
	
	void					SetBrake						( bool bBrake );
	bool					GetBrake						( void );

	void					SetWheelsProtected				( bool bProtected );
	bool					AreWheelsProtected				( void );

	void					FixPosition						( bool bFix );
	bool					IsPositionFixed					( void );

	void					SetHandbrake					( bool bHandbrake );
	bool					IsHandbrakeOn					( void );

	int						GetTotalSeats					( void );
	int						GetPedOccupiedSeat				( M2Ped * pPed );
	M2Ped					* GetSeatOccupant				( int iSeat );

	void					SetWheelTexture					( int iWheelIndex, const char * szTexture );
	const char				* GetWheelTexture				( int iWheelIndex );

	void					SetLightState					( bool bLightState );
	bool					GetLightState					( void );

};