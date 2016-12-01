/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSync.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

// NOTE(zaklaus): Even though it sounds forced, Linux.h contains condition for non-win32 system.
#include "Linux.h"

#include "CColor.h"
#include "Math/CVector3.h"
#include "RakString.h"

// Sync data

struct OnFootSync
{
	CVector3		m_vecPosition;
	CVector3		m_vecRotation;
	CVector3		m_vecDirection;
	float			m_fHealth;
	DWORD			m_dwSelectedWeapon;
	int				m_iSelectedWeaponBullet;
	BYTE			m_bControlState;
	bool			m_bAiming;
	bool			m_bShooting;
	bool			m_bCrouching;
	unsigned int	m_uiModelIndex;
	CVector3		m_vecLookAt;
	int				m_iHandModel;
	int				m_iHand;

	OnFootSync()
		: m_vecPosition()
		, m_vecRotation()
		, m_vecDirection()
		, m_fHealth(720.0f)
		, m_dwSelectedWeapon(1)
		, m_iSelectedWeaponBullet(0)
		, m_bControlState(0)
		, m_bAiming(false)
		, m_bShooting(false)
		, m_bCrouching(false)
		, m_uiModelIndex(0)
		, m_vecLookAt()
		, m_iHandModel(0)
		, m_iHand(0)
	{
	}

	OnFootSync& operator=(const OnFootSync &rhs)
	{
		m_vecPosition = rhs.m_vecPosition;
		m_vecRotation = rhs.m_vecRotation;
		m_vecDirection = rhs.m_vecDirection;
		m_fHealth = rhs.m_fHealth;
		m_dwSelectedWeapon = rhs.m_dwSelectedWeapon;
		m_iSelectedWeaponBullet = rhs.m_iSelectedWeaponBullet;
		m_bControlState = rhs.m_bControlState;
		m_bAiming = rhs.m_bAiming;
		m_bShooting = rhs.m_bShooting;
		m_bCrouching = rhs.m_bCrouching;
		m_uiModelIndex = rhs.m_uiModelIndex;
		m_vecLookAt = rhs.m_vecLookAt;
		m_iHandModel = rhs.m_iHandModel;
		m_iHand = rhs.m_iHand;
		return *this;
	}
};

const size_t MAX_LICENSE_PLATE_LENGTH = 6;

struct InVehicleSync
{
	CVector3		m_vecPosition;
	CVector3		m_vecRotation;
	CVector3		m_vecVelocity;
	float			m_fDirtLevel;
	int				m_iTuningTable;
	bool			m_bHornState;
	bool			m_bSirenState;
	bool			m_bBeaconLightState;
	bool			m_bEngineState;
	float			m_fFuel;
	float			m_fTurnSpeed;
	float			m_fEngineDamage;
	char			m_szPlateText[MAX_LICENSE_PLATE_LENGTH + 1];
	int				m_model;
	CColor			m_primaryColour;
	CColor			m_secondaryColour;
	bool			m_bPower;
	bool			m_bBrake;
	BYTE			m_bWheelModels[3];
	bool			m_bHandbrake;
	bool			m_bLightState;
	bool			m_bPartState_Hood;
	bool			m_bPartState_Trunk;
	bool			m_bLeftIndicator;
	bool			m_bRightIndicator;
	bool			m_bTaxiLight;
};

struct InPassengerSync
{
	float			m_fHealth;
	DWORD			m_dwSelectedWeapon;
	int				m_iSelectedWeaponBullet;
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
