/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSync.h
* Developers : AaronLad <aaron@m2-multiplayer.com>, MyU <myudev0@gmail.com>
*
***************************************************************/

#pragma once

#include	"CColor.h"
#include	"Math/CVector3.h"
#include	"../Libraries/RakNet/Source/RakString.h"

// Sync data

struct OnFootSync
{
	CVector3		m_vecPosition;
	CVector3		m_vecRotation;
	CVector3		m_vecDirection;
	float			m_fHealth;
	DWORD			m_dwSelectedWeapon;
	BYTE			m_bControlState;
	bool			m_bAiming;
	bool			m_bShooting;
	bool			m_bCrouching;
	unsigned int	m_uiModelIndex;
	CVector3		m_vecLookAt;
	int				m_iHandModel;
	int				m_iHand;
	/*const char		*m_styleName;
	const char		*m_styleDirectory;*/
};

struct MiscInfo {
	unsigned int bHornState : 1;
	unsigned int bSirenState : 1;
	unsigned int bBeaconLightState : 1;
	unsigned int bEngineState : 1;

	unsigned int bPower : 1;
	unsigned int bBrake : 1;
	unsigned int bHandbrake : 1;
	unsigned int bLightState : 1;
	unsigned int bPartState_Hood : 1;
	unsigned int bPartState_Trunk : 1;
}; // TODO: change it.. just do it there are some things that NOT need to be synced every freakin second - MyU


struct VehicleSpawnProperties {
	//#pragma pack(1) // TODO: think about the access impact - MyU
	float			m_fDirtLevel;
	int				m_iTuningTable;
	float			m_fFuel;
	char			m_szPlateText[6];
	unsigned int	m_uiModelIndex;
	DWORD			m_primaryColour; // cast to CColor on the client / server side.
	DWORD			m_secondaryColour;
	BYTE			m_bWheelModels[3];
};

struct InVehicleSync
{
	//#pragma pack(1) // TODO: think about the access impact - MyU
	CVector3		m_vecPosition;
	CVector3		m_vecRotation;
	CVector3		m_vecVelocity;

	float			m_fTurnSpeed;
	float			m_fEngineDamage;

	MiscInfo		m_Flags;
};

struct InPassengerSync
{
	float			m_fHealth;
	DWORD			m_dwSelectedWeapon;
};

struct UnoccupiedVehicleSync
{
	CVector3		m_vecPosition;
	CVector3		m_vecRotation;
};

struct MiscellaneousPlayerSync
{
	int				m_iMoney;
};