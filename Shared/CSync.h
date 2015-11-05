/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSync.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

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
	unsigned int	m_uiModelIndex;
	CVector3		m_vecLookAt;
};

struct InVehicleSync
{
	CVector3		m_vecPosition;
	CVector3		m_vecRotation;
	CVector3		m_vecVelocity;
	float			m_fDirtLevel;
	int				m_iTuningTable;
	bool			m_bHornState;
	bool			m_bSirenState;
	bool			m_bEngineState;
	float			m_fFuel;
	float			m_fTurnSpeed;
	float			m_fEngineDamage;
	char			m_szPlateText[6];
	CColor			m_primaryColour;
	CColor			m_secondaryColour;
	bool			m_bPower;
	bool			m_bBrake;
	BYTE			m_bWheelModels[3];
	bool			m_bHandbrake;
	bool			m_bLightState;
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