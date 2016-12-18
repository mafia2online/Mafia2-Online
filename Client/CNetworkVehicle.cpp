/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CNetworkVehicle.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "CString.h"
#include "Math/CMaths.h"
#include "CColor.h"
#include "Math/CQuaternion.h"
#include "Math/CVector3.h"

#include "Game/CGame.h"
#include "CMafia.h"

#include "CSync.h"

#include "CBlip.h"

#include "engine/CM2Entity.h"
#include "engine/CM2Ped.h"
#include "engine/CM2Vehicle.h"

#include "CModelManager.h"
#include "CNetworkModelManager.h"
#include "CIE.h"

#include "CNetworkModule.h"

#include "Network/NetRPCs.h"

#include "PacketPriority.h"

#include "SharedUtility.h"

#include "CPlayerManager.h"
#include "CLocalPlayer.h"
#include "CNetworkPlayer.h"
#include "CRemotePlayer.h"

#include "CClientScriptGUIManager.h"
#include "CClientScriptingManager.h"

#include "Scripting/CScriptingManager.h"
#include "Scripting/CSquirrelArguments.h"
#include "CEvents.h"

#include "CSync.h"

#include "CNetworkVehicle.h"

#include "CLogFile.h"

CNetworkVehicle::CNetworkVehicle( void )
{
	DEBUG_LOG("CNetworkVehicle::CNetworkVehicle");

	m_vehicleId = INVALID_ENTITY_ID;
	m_uiModelIndex = -1;
	m_pVehicle = nullptr;
	m_pVehicleModelManager = nullptr;
	m_bSpawned = false;
	m_bProcessSyncOnSpawn = false;
	m_bSpawnProcessed = false;
	m_pLastSyncer = nullptr;
	m_ulSpawnTime = 0;
	m_pAttachedBlip = nullptr;
	m_bBlipAttached = false;

	m_bPartState[VEHICLE_PART_HOOD] = 0;
	m_bPartState[VEHICLE_PART_TRUNK] = 0;

	m_pDriver = nullptr;
	memset( m_pPassenger, NULL, sizeof(m_pPassenger) );

	ResetInterpolation();
}

CNetworkVehicle::~CNetworkVehicle( void )
{
	DEBUG_LOG("CNetworkVehicle::~CNetworkVehicle");

	//CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->StopSyncVehicle( this );
	Destroy ();
}

void CNetworkVehicle::SetModel( unsigned int uiModelIndex, bool bRebuild )
{
	DEBUG_LOG("CNetworkVehicle::SetModel");

	m_uiModelIndex = uiModelIndex;
	if( bRebuild )
	{
	}
}

void CNetworkVehicle::Create( void )
{
	DEBUG_LOG("CNetworkVehicle::Create");

	if( m_pVehicle )
		Destroy ();

	String strModel;
	Game::GetVehicleModelFromId( m_uiModelIndex, &strModel );

	DEBUG_LOG ( "CNetworkVehicle< %d >::Create( %d ) - Building vehicle with model '%s'...", m_vehicleId, m_uiModelIndex, strModel.Get() );

	m_pVehicleModelManager = CNetworkModelManager::Load( SDS_LOAD_DIR_CARS, strModel.Get() );
	if( !m_pVehicleModelManager )
		return;

	m_pVehicle = IE::CreateWrapperVehicle(m_pVehicleModelManager);

	DEBUG_LOG ( "CNetworkVehicle< %d >::Create( ) - Wrapper Vehicle: 0x%p, Engine vehicle: 0x%p", m_vehicleId, m_pVehicle, (m_pVehicle ? m_pVehicle->GetVehicle () : NULL) );

	GetColour ( &m_primarySpawnColour, &m_secondarySpawnColour );

	DEBUG_LOG ( "CNetworkVehicle< %d >::Create( ) - 1", m_vehicleId );

	ResetInterpolation();

	DEBUG_LOG ( "CNetworkVehicle< %d >::Create( ) - 2", m_vehicleId );

	m_ulSpawnTime = SharedUtility::GetTime();
	m_bSpawnProcessed = true;

	DEBUG_LOG ( "CNetworkVehicle< %d >::Create( ) - Done", m_vehicleId );

	if ( m_pAttachedBlip && !m_bBlipAttached )
	{
		m_pAttachedBlip->AttachToVehicle ( this );
		m_bBlipAttached = true;
	}
}

void CNetworkVehicle::Destroy( void )
{
	DEBUG_LOG("CNetworkVehicle::Destroy");

	if( !m_pVehicle )
		return;

	M2Ped * pSeatPed = NULL;
	CNetworkPlayer * pPlayer = NULL;
	for( int i = 0; i < GetVehicle()->GetTotalSeats(); i++ )
	{
		pSeatPed = m_pVehicle->GetSeatOccupant ( ( i + 1 ) );
		if( pSeatPed )
		{
			pPlayer = (pSeatPed == IE::GetGame()->m_pLocalPed ? (CNetworkPlayer *)CCore::Instance()->GetPlayerManager()->GetLocalPlayer() : (CNetworkPlayer *)CCore::Instance()->GetPlayerManager()->GetFromGameGUID ( pSeatPed->m_dwGUID ));
			if( pPlayer )
			{
				if ( pPlayer->IsLocalPlayer () )
				{
					((CLocalPlayer *)pPlayer)->SetRemoveForcefully ( true );
					((CLocalPlayer *)pPlayer)->FlagForFastExitVehicle ( true );
				}
				pPlayer->RemoveFromVehicle ( this );
			}
		}
	}

	m_pVehicle->Deactivate ();
	DetachBlip ();

	CNetworkModelManager::Unload ( m_pVehicleModelManager );

	SAFE_DELETE( m_pVehicle );

	SetSpawned ( false );
}

void CNetworkVehicle::Respawn( void )
{
	DEBUG_LOG("CNetworkVehicle::Respawn");

	if( m_pVehicle )
	{
		RakNet::BitStream bitStream;
		bitStream.WriteCompressed( m_vehicleId );
		CCore::Instance()->GetNetworkModule()->Call( RPC_RESPAWNVEHICLE, &bitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, true );
	}
}

void CNetworkVehicle::HandleRespawn( void )
{
	DEBUG_LOG("CNetworkVehicle::HandleRespawn");

	if( m_pVehicle )
	{
		SetSpawned(false);

		m_pVehicle->Deactivate();
		m_pVehicle->Activate();

		m_pVehicle->Repair();

		m_pVehicle->SetColour( m_primarySpawnColour, m_secondarySpawnColour );
		String plate;
		plate.Format("EB%d", m_vehicleId);
		m_pVehicle->SetPlateText( plate );
		m_pVehicle->SetSpeed( 0.0f );
		m_pVehicle->SetHornOn( false );
		m_pVehicle->SetSirenOn( false );
		m_pVehicle->SetBeaconLightOn( false );
		m_pVehicle->SetDirtLevel( 0.0f );
		m_pVehicle->SetTuningTable( 0 );
		m_pVehicle->SetPower( false );
		m_pVehicle->SetBrake( false );
		m_pVehicle->SetWheelsProtected( true );
		SetPosition( m_vecSpawnPosition );
		SetRotation( m_vecSpawnRotation );
		m_pVehicle->SetLightState ( false );

		SetSpawned(true);

		if( CCore::Instance()->GetClientScriptingManager() && CCore::Instance()->GetClientScriptingManager()->GetEvents() )
		{
			CSquirrelArguments args;
			args.push( m_vehicleId );
			CCore::Instance()->GetClientScriptingManager()->GetEvents()->Call( "onClientVehicleRespawn", &args );
		}
	}
}

void CNetworkVehicle::StoreVehicleSync( const InVehicleSync &vehicleSync, bool bInterpolate, bool bSpawn )
{
	DEBUG_LOG("CNetworkVehicle::StoreVehicleSync");

	m_lastSyncData = vehicleSync;

	CVector3 vecLocalPos;
	CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetPosition( &vecLocalPos );

	if ( !Math::IsValidVector ( m_lastSyncData.m_vecPosition ) || !Math::IsValidVector ( m_lastSyncData.m_vecRotation ) )
	{
		m_lastSyncData.m_vecPosition = m_vecSpawnPosition;
		m_lastSyncData.m_vecRotation = m_vecSpawnRotation;
		m_vecLastGoodPosition = m_vecSpawnPosition;
		m_vecLastGoodRotation = m_vecSpawnRotation;
	}

	if( bSpawn || Math::IsDistanceBetweenPointsLessThen(vecLocalPos, m_lastSyncData.m_vecPosition, 300.0f) )
	{
		CVector3 vecVelocity;
		GetSpeedVec(&vecVelocity);

		CColor primary, secondary;
		GetColour(&primary, &secondary);

		if( bInterpolate )
		{
			SetTargetPosition( m_lastSyncData.m_vecPosition );
			SetTargetRotation( m_lastSyncData.m_vecRotation );
		}
		else
		{
			SetPosition( m_lastSyncData.m_vecPosition );
			SetRotation( m_lastSyncData.m_vecRotation );

			DEBUG_LOG ( "Not using interpolation this frame for vehicle %d.", m_vehicleId );
		}

		if( GetDirtLevel() != m_lastSyncData.m_fDirtLevel )
			SetDirtLevel( m_lastSyncData.m_fDirtLevel );
		if( GetTuningTable() != m_lastSyncData.m_iTuningTable )
			SetTuningTable( m_lastSyncData.m_iTuningTable );
		if( GetHornState() != m_lastSyncData.m_bHornState )
			SetHornState( m_lastSyncData.m_bHornState );
		if( m_pVehicle->IsSirenOn() != m_lastSyncData.m_bSirenState )
			m_pVehicle->SetSirenOn( m_lastSyncData.m_bSirenState );
		if (m_pVehicle->IsBeaconLightOn() != m_lastSyncData.m_bBeaconLightState)
			m_pVehicle->SetBeaconLightOn(m_lastSyncData.m_bBeaconLightState);
		/*if ( m_pVehicle->GetFuel () != vehicleSync.m_fFuel )
			m_pVehicle->SetFuel ( vehicleSync.m_fFuel );*/
		if( Math::IsValidVector ( m_lastSyncData.m_vecVelocity ) && vecVelocity != m_lastSyncData.m_vecVelocity )
			SetTargetSpeed ( m_lastSyncData.m_vecVelocity ); //SetSpeedVec( vehicleSync.m_vecVelocity );
		if( GetSteer() != m_lastSyncData.m_fTurnSpeed )
			SetTargetSteer ( m_lastSyncData.m_fTurnSpeed );
		if( m_pVehicle->GetEngineDamage() != m_lastSyncData.m_fEngineDamage )
			m_pVehicle->SetEngineDamage( m_lastSyncData.m_fEngineDamage );
		if( strcmp( m_lastSyncData.m_szPlateText, GetPlateText() ) )
			SetPlateText( m_lastSyncData.m_szPlateText );
		if( primary != m_lastSyncData.m_primaryColour || secondary != m_lastSyncData.m_secondaryColour )
			SetColour( m_lastSyncData.m_primaryColour, m_lastSyncData.m_secondaryColour );
		if( m_pVehicle->GetPower() != m_lastSyncData.m_bPower )
			m_pVehicle->SetPower( m_lastSyncData.m_bPower );
		if( m_pVehicle->GetBrake() != m_lastSyncData.m_bBrake )
			m_pVehicle->SetBrake( m_lastSyncData.m_bBrake );
		/*if ( vehicleSync.m_bWheelModels[ 0 ] != 0xFF && Game::GetIdFromVehicleWheelModel ( m_pVehicle->GetWheelTexture ( 0 ) ) != vehicleSync.m_bWheelModels[ 0 ] )
			m_pVehicle->SetWheelTexture ( 0, Game::GetVehicleWheelModelFromId ( vehicleSync.m_bWheelModels[ 0 ] ).Get () );
		if ( vehicleSync.m_bWheelModels[ 1 ] != 0xFF && Game::GetIdFromVehicleWheelModel ( m_pVehicle->GetWheelTexture ( 1 ) ) != vehicleSync.m_bWheelModels[ 1 ] )
			m_pVehicle->SetWheelTexture ( 1, Game::GetVehicleWheelModelFromId ( vehicleSync.m_bWheelModels[ 1 ] ).Get () );
		if ( vehicleSync.m_bWheelModels[ 2 ] != 0xFF && Game::GetIdFromVehicleWheelModel ( m_pVehicle->GetWheelTexture ( 2 ) ) != vehicleSync.m_bWheelModels[ 2 ] )
			m_pVehicle->SetWheelTexture ( 2, Game::GetVehicleWheelModelFromId ( vehicleSync.m_bWheelModels[ 2 ] ).Get () );*/
		if ( m_pVehicle->IsHandbrakeOn () != m_lastSyncData.m_bHandbrake )
			m_pVehicle->SetHandbrake ( m_lastSyncData.m_bHandbrake );
		if ( m_pVehicle->GetLightState () != m_lastSyncData.m_bLightState )
			m_pVehicle->SetLightState ( m_lastSyncData.m_bLightState );
		if (m_pVehicle->IsTaxiLightOn() != m_lastSyncData.m_bTaxiLight)
			m_pVehicle->SetTaxiLightOn(m_lastSyncData.m_bTaxiLight);
		if (m_pVehicle->IsIndicatorLightsOn(0) != m_lastSyncData.m_bLeftIndicator)
			m_pVehicle->SetIndicatorLightsOn(0, m_lastSyncData.m_bLeftIndicator);
		if (m_pVehicle->IsIndicatorLightsOn(1) != m_lastSyncData.m_bRightIndicator)
			m_pVehicle->SetIndicatorLightsOn(1, m_lastSyncData.m_bRightIndicator);
	}
	else
	{
		SetPosition( m_lastSyncData.m_vecPosition );
		SetRotation( m_lastSyncData.m_vecRotation );
	}
}

void CNetworkVehicle::Pulse( void )
{
	if( !CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->IsSpawned() )
		return;

	if( IsSpawned() )
	{
		CVector3 vecPosition, vecRotation;
		GetPosition( &vecPosition );
		GetRotation( &vecRotation );

		if ( !Math::IsValidVector ( vecPosition ) || !Math::IsValidVector ( vecRotation ) )
		{
			m_pVehicle->Deactivate ();
			SetPosition ( m_vecLastGoodPosition );
			SetRotation ( m_vecLastGoodRotation );
			if ( !Math::IsValidVector ( m_vecLastGoodPosition ) || !Math::IsValidVector ( m_vecLastGoodRotation ) )
			{
				SetPosition ( m_vecSpawnPosition );
				SetRotation ( m_vecSpawnRotation );
			}
			m_pVehicle->Activate ();

			// Here we return so we don't override the last good position
			return;
		}
		memcpy ( &m_vecLastGoodPosition, &vecPosition, sizeof(CVector3) );
		memcpy ( &m_vecLastGoodRotation, &vecRotation, sizeof(CVector3) );

		Interpolate ();

		if( !m_pDriver )
		{
			if( vecPosition.fZ <= -50.0f )
			{
				m_pVehicle->Deactivate();

				SetPosition( m_vecSpawnPosition );
				SetRotation( m_vecSpawnRotation );
				m_pVehicle->Activate();
				StoreVehicleSync( m_lastSyncData, false, true );
			}
		}
	}
	else
	{
		unsigned long ulCurrentTime = SharedUtility::GetTime ();
		if( m_bSpawnProcessed/* && ((ulCurrentTime - m_ulSpawnTime) >= 4000 || ((ulCurrentTime - CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetSpawnTime()) >= 4000))*/ )
		{
			m_ulSpawnTime = ulCurrentTime;
			m_bSpawnProcessed = false;

			m_pVehicle->Activate();
			if( m_bProcessSyncOnSpawn )
			{
				StoreVehicleSync( m_lastSyncData, false, true );
			}
			else
			{
				SetPosition( m_vecSpawnPosition );
				SetRotation( m_vecSpawnRotation );
			}
			if( m_pDriver )
			{
				m_pDriver->PutInVehicle( this, 1 );
			}
			for( int i = 0; i < (MAX_SEATS - 1); i++ )
			{
				if( m_pPassenger[ i ] )
				{
					m_pPassenger[ i ]->PutInVehicle( this, (i + 2) );
				}
			}
			SetSpawned( true );
		}
	}
}

void CNetworkVehicle::SetPosition( CVector3 vecPosition )
{
	if( m_pVehicle )
		m_pVehicle->SetPosition( vecPosition );
}

void CNetworkVehicle::GetPosition( CVector3 * vecPosition )
{
	if( m_pVehicle )
		m_pVehicle->GetPosition( vecPosition );
}

void CNetworkVehicle::SetRotation( CVector3 vecRotation )
{
	if( m_pVehicle )
	{
		vecRotation.ToRadians();
		m_pVehicle->SetRotation( Quaternion( vecRotation ) );
	}
}

void CNetworkVehicle::GetRotation( CVector3 * vecRotation )
{
	if( m_pVehicle )
	{
		Quaternion quatRotation;
		m_pVehicle->GetRotation( &quatRotation );

		memcpy( vecRotation, &quatRotation.toEularAngles(), sizeof(CVector3) );

		vecRotation->FromRadians();
	}
}

void CNetworkVehicle::SetColour( CColor primary, CColor secondary )
{
	if( m_pVehicle )
		m_pVehicle->SetColour( primary, secondary );

	m_lastSyncData.m_primaryColour = primary;
	m_lastSyncData.m_secondaryColour = secondary;
}

void CNetworkVehicle::GetColour( CColor * primary, CColor * secondary )
{
	if( m_pVehicle )
		m_pVehicle->GetColour( primary, secondary );
}

bool CNetworkVehicle::SetPlateText( const char * szPlateText )
{
	if (!szPlateText || strlen(szPlateText) > MAX_LICENSE_PLATE_LENGTH)
		return false;

	if( m_pVehicle )
		m_pVehicle->SetPlateText( szPlateText );

	strcpy(m_lastSyncData.m_szPlateText, szPlateText);
	return true;
}

const char * CNetworkVehicle::GetPlateText( void ) const
{
	if( m_pVehicle )
		return m_pVehicle->GetPlateText();

	return "UNKPLT";
}

void CNetworkVehicle::Repair( void )
{
	if( m_pVehicle )
		m_pVehicle->Repair();
}

void CNetworkVehicle::Explode( void )
{
	if( m_pVehicle )
		m_pVehicle->Explode();
}

void CNetworkVehicle::SetDirtLevel( float fDirtLevel )
{
	if( m_pVehicle )
		m_pVehicle->SetDirtLevel( fDirtLevel );

	m_lastSyncData.m_fDirtLevel = fDirtLevel;
}

float CNetworkVehicle::GetDirtLevel( void )
{
	if( m_pVehicle )
		return m_pVehicle->GetDirtLevel();

	return 0.0f;
}

void CNetworkVehicle::SetEngineState( bool bState )
{
	if( m_pVehicle )
		m_pVehicle->SetEngineOn( bState );

	m_lastSyncData.m_bEngineState = bState;
}

bool CNetworkVehicle::GetEngineState( void )
{
	if( m_pVehicle )
		return m_pVehicle->IsEngineOn();

	return false;
}

void CNetworkVehicle::SetPartOpen( int iPart, bool bOpen )
{
	if( m_pVehicle )
	{
		if (iPart == VEHICLE_PART_HOOD) {
			bOpen ? m_pVehicle->OpenHood() : m_pVehicle->CloseHood();
			m_bPartState[VEHICLE_PART_HOOD] = bOpen;

			m_lastSyncData.m_bPartState_Hood = bOpen;
		}
		else {
			bOpen ? m_pVehicle->OpenTrunk() : m_pVehicle->CloseTrunk();
			m_bPartState[VEHICLE_PART_TRUNK] = bOpen;
			m_lastSyncData.m_bPartState_Trunk = bOpen;
		}
	}
}

bool CNetworkVehicle::IsPartOpen( int iPart )
{
	if (m_pVehicle){
		if (iPart == VEHICLE_PART_HOOD)
			return (m_bPartState[VEHICLE_PART_HOOD]);
		else if (iPart == VEHICLE_PART_TRUNK)
			return (0);
		else
			return (m_bPartState[VEHICLE_PART_TRUNK]);
	}
	return (false);
}

void CNetworkVehicle::SetSirenState( bool bState )
{
	if( m_pVehicle )
		m_pVehicle->SetSirenOn( bState );

	m_lastSyncData.m_bSirenState = bState;
}

bool CNetworkVehicle::GetSirenState( void )
{
	if( m_pVehicle )
		return m_pVehicle->IsSirenOn();

	return false;
}

void CNetworkVehicle::SetBeaconLightState(bool bState)
{
	if (m_pVehicle)
		m_pVehicle->SetBeaconLightOn(bState);

	m_lastSyncData.m_bBeaconLightState = bState;
}

bool CNetworkVehicle::GetBeaconLightState(void)
{
	if (m_pVehicle)
		return m_pVehicle->IsBeaconLightOn();

	return false;
}

void CNetworkVehicle::SetHornState( bool bState )
{
	if( m_pVehicle )
		m_pVehicle->SetHornOn( bState );

	m_lastSyncData.m_bHornState = bState;
}

bool CNetworkVehicle::GetHornState( void )
{
	if( m_pVehicle )
		return m_pVehicle->IsHornOn();

	return false;
}

void CNetworkVehicle::SetWindowOpen( int iSeat, bool bOpen )
{
	if (m_pVehicle)
		m_pVehicle->SetWindowOpen(iSeat, bOpen);
}

bool CNetworkVehicle::IsWindowOpen( int iSeat )
{
	// todo
	return false;
}

void CNetworkVehicle::SetTuningTable( int iTable )
{
	if( m_pVehicle )
		m_pVehicle->SetTuningTable( iTable );

	m_lastSyncData.m_iTuningTable = iTable;
}

int CNetworkVehicle::GetTuningTable( void )
{
	if( m_pVehicle )
		return m_pVehicle->GetTuningTable();

	return 0;
}

void CNetworkVehicle::SetWheelTexture( int iWheelIndex, int iTexture )
{
	if( m_pVehicle )
		m_pVehicle->SetWheelTexture( iWheelIndex, Game::GetVehicleWheelModelFromId( iTexture ) );

	m_lastSyncData.m_bWheelModels[iWheelIndex] = static_cast<BYTE>(iTexture);
}

int CNetworkVehicle::GetWheelTexture( int iWheelIndex )
{
	if( m_pVehicle )
		return Game::GetIdFromVehicleWheelModel( m_pVehicle->GetWheelTexture( iWheelIndex ) );

	return 0xFF;
}

void CNetworkVehicle::SetSteer ( float fSteer )
{
	if ( m_pVehicle )
		m_pVehicle->AddSteer ( (fSteer * (D3DX_PI / 180.0f)) );

	m_lastSyncData.m_fTurnSpeed = fSteer;
}

float CNetworkVehicle::GetSteer ( void )
{
	if ( m_pVehicle )
		return (m_pVehicle->GetSteer() * (180.0f / D3DX_PI));

	return 0.0f;
}

void CNetworkVehicle::SetSpeed ( float fSpeed )
{
	if ( m_pVehicle )
		m_pVehicle->SetSpeed ( fSpeed );
}

float CNetworkVehicle::GetSpeed ( void )
{
	if ( m_pVehicle )
		return m_pVehicle->GetSpeed ();

	return 0.0f;
}

void CNetworkVehicle::SetSpeedVec ( CVector3 vecSpeed )
{
	if ( m_pVehicle )
		m_pVehicle->SetMoveSpeed ( vecSpeed );

	m_lastSyncData.m_vecVelocity = vecSpeed;
}

void CNetworkVehicle::GetSpeedVec ( CVector3 * vecSpeed )
{
	if ( m_pVehicle )
		m_pVehicle->GetMoveSpeed ( vecSpeed );
}

void CNetworkVehicle::SetFuel ( float fFuel )
{
	if ( m_pVehicle )
		m_pVehicle->SetFuel ( fFuel );

	m_lastSyncData.m_fFuel = fFuel;
}

float CNetworkVehicle::GetFuel ( void )
{
	if ( m_pVehicle )
		return m_pVehicle->GetFuel ();

	return 0.0f;
}

void CNetworkVehicle::SetLightState ( bool bLightState )
{
	if ( m_pVehicle )
		m_pVehicle->SetLightState ( bLightState );

	m_lastSyncData.m_bLightState = bLightState;
}

bool CNetworkVehicle::GetLightState ( void )
{
	if ( m_pVehicle )
		return m_pVehicle->GetLightState ();

	return false;
}

void CNetworkVehicle::SetTaxiLightState(bool state)
{
	if (m_pVehicle)
		m_pVehicle->SetTaxiLightOn(state);

	m_lastSyncData.m_bTaxiLight = state;
}

bool CNetworkVehicle::GetTaxiLightState(void)
{
	if (m_pVehicle)
		return m_pVehicle->IsTaxiLightOn();

	return false;
}

void CNetworkVehicle::SetIndicatorLightState(int indicator, bool state)
{
	if (m_pVehicle)
		m_pVehicle->SetIndicatorLightsOn(indicator, state);

	if (indicator == 0)
		m_lastSyncData.m_bLeftIndicator = state;
	else if (indicator == 1)
		m_lastSyncData.m_bRightIndicator = state;
}

bool CNetworkVehicle::GetIndicatorLightState(int indicator)
{
	if (m_pVehicle)
		return m_pVehicle->IsIndicatorLightsOn(indicator);

	return false;
}

void CNetworkVehicle::SetHandbrake(bool bHandbrake)
{
	if (m_pVehicle)
		m_pVehicle->SetHandbrake(bHandbrake);

	m_lastSyncData.m_bHandbrake = bHandbrake;
}

bool CNetworkVehicle::GetHandbrake(void)
{
	return m_lastSyncData.m_bHandbrake;
}

void CNetworkVehicle::HandlePlayerEnter( CNetworkPlayer * pNetworkPlayer, EntityId seatId )
{
	if( seatId == 0 )
	{
		m_pDriver = pNetworkPlayer;
		SetLastSyncer( NULL );
		m_pVehicle->SetHandbrake ( false );
	}
	else
	{
		m_pPassenger[ seatId ] = pNetworkPlayer;
	}
}

void CNetworkVehicle::HandlePlayerExit( CNetworkPlayer * pNetworkPlayer, EntityId seatId, bool bResetInterpolation )
{
	DEBUG_LOG("CNetworkVehicle::HandlePlayerExit");

	if( seatId == 0 )
	{
		m_pDriver = NULL;
		SetLastSyncer( pNetworkPlayer );
		m_pVehicle->SetHandbrake ( true );
		if( pNetworkPlayer != CCore::Instance()->GetPlayerManager()->GetLocalPlayer() && bResetInterpolation )
			ResetInterpolation ();
	}
	else
	{
		m_pPassenger[ seatId ] = NULL;
	}
}

void CNetworkVehicle::ResetInterpolation( void )
{
	if ( IsSpawned() )
	{
		if( HasTargetPosition () )
			SetPosition ( m_Interpolation.position.vecTarget );

		if( HasTargetRotation () )
			SetRotation ( m_Interpolation.rotation.vecTarget );

		if ( HasTargetSteer () )
			SetSteer ( m_Interpolation.steer.fTarget );

		if ( HasTargetSpeed () )
			SetSpeedVec ( m_Interpolation.speed.vecTarget );
	}

	RemoveTargetPosition ();
	m_Interpolation.position.fLastAlpha = 0;
	m_Interpolation.position.ulStartTime = 0;
	m_Interpolation.position.vecError = CVector3();
	m_Interpolation.position.vecTarget = CVector3();

	RemoveTargetRotation ();
	m_Interpolation.rotation.fLastAlpha = 0;
	m_Interpolation.rotation.ulStartTime = 0;
	m_Interpolation.rotation.vecError = CVector3();
	m_Interpolation.rotation.vecTarget = CVector3();

	RemoveTargetSteer ();
	m_Interpolation.steer.fLastAlpha = 0;
	m_Interpolation.steer.ulStartTime = 0;
	m_Interpolation.steer.fError = 0.0f;
	m_Interpolation.steer.fTarget = 0.0f;

	RemoveTargetSpeed ();
	m_Interpolation.speed.fLastAlpha = 0;
	m_Interpolation.speed.ulStartTime = 0;
	m_Interpolation.speed.vecError = CVector3();
	m_Interpolation.speed.vecTarget = CVector3();
}

void CNetworkVehicle::Interpolate( void )
{
	if( m_pDriver && m_pDriver != CCore::Instance()->GetPlayerManager()->GetLocalPlayer () )
	{
		UpdateTargetPosition ();
		UpdateTargetRotation ();
		UpdateTargetSteer ();
		UpdateTargetSpeed ();
	}
}

void CNetworkVehicle::SetTargetPosition( CVector3 vecPosition )
{
	if ( IsSpawned () )
	{
		UpdateTargetPosition ();
		unsigned long ulCurrentTime = SharedUtility::GetTime ();
		CVector3 vecCurrentPosition;
		GetPosition ( &vecCurrentPosition );
		m_Interpolation.position.vecTarget = vecPosition;
		m_Interpolation.position.vecError = (vecPosition - vecCurrentPosition);
		m_Interpolation.position.vecError *= Math::Lerp < const float > ( 0.25f, Math::UnlerpClamped ( 100, NETWORK_TICKRATE, 400 ), 1.0f );
		m_Interpolation.position.ulStartTime = ulCurrentTime;
		m_Interpolation.position.ulFinishTime = (ulCurrentTime + NETWORK_TICKRATE);
		m_Interpolation.position.fLastAlpha = 0.0f;
	}
}

void CNetworkVehicle::UpdateTargetPosition( void )
{
	if ( IsSpawned () && HasTargetPosition () )
	{
		CVector3 vecCurrentPosition;
		GetPosition ( &vecCurrentPosition );

		unsigned long ulCurrentTime = SharedUtility::GetTime ();
		float fAlpha = Math::Clamp ( 0.0f, Math::Unlerp ( m_Interpolation.position.ulStartTime, ulCurrentTime, m_Interpolation.position.ulFinishTime ), 1.5f );
		float fCurrentAlpha = (fAlpha - m_Interpolation.position.fLastAlpha);

		m_Interpolation.position.fLastAlpha = fAlpha;

		CVector3 vecErrorCompensation = Math::Lerp ( CVector3(), fCurrentAlpha, m_Interpolation.position.vecError );

		if ( fAlpha == 1.5f )
			m_Interpolation.position.ulFinishTime = 0;

		CVector3 vecTarget = (vecCurrentPosition + vecErrorCompensation);

		vecTarget.fZ = m_Interpolation.position.vecTarget.fZ;

		SetPosition ( vecTarget );
	}
}

void CNetworkVehicle::SetTargetRotation( CVector3 vecRotation )
{
	if( IsSpawned() )
	{
		UpdateTargetRotation ();
		unsigned long ulCurrentTime = SharedUtility::GetTime ();
		CVector3 vecCurrentRotation;
		GetRotation ( &vecCurrentRotation );
		m_Interpolation.rotation.vecTarget = vecRotation;
		m_Interpolation.rotation.vecError = Math::GetOffsetDegrees ( vecCurrentRotation, vecRotation );
		m_Interpolation.rotation.vecError *= Math::Lerp < const float > ( 0.25f, Math::UnlerpClamped ( 100, NETWORK_TICKRATE, 400 ), 1.0f ); // 250ms -> 0.40f
		m_Interpolation.rotation.ulStartTime = ulCurrentTime;
		m_Interpolation.rotation.ulFinishTime = (ulCurrentTime + NETWORK_TICKRATE);
		m_Interpolation.rotation.fLastAlpha = 0.0f;
	}
}

void CNetworkVehicle::UpdateTargetRotation( void )
{
	if( HasTargetRotation() )
	{
		CVector3 vecCurrentRotation;
		GetRotation ( &vecCurrentRotation );

		unsigned long ulCurrentTime = SharedUtility::GetTime ();
		float fAlpha = Math::Clamp ( 0.0f, Math::Unlerp ( m_Interpolation.rotation.ulStartTime, ulCurrentTime, m_Interpolation.rotation.ulFinishTime ), 1.0f );
		float fCurrentAlpha = (fAlpha - m_Interpolation.rotation.fLastAlpha);

		m_Interpolation.rotation.fLastAlpha = fAlpha;

		CVector3 vecErrorCompensation = Math::Lerp ( CVector3(), fCurrentAlpha, m_Interpolation.rotation.vecError );

		if ( fAlpha == 1.0f )
			m_Interpolation.rotation.ulFinishTime = 0;

		SetRotation ( (vecCurrentRotation + vecErrorCompensation) );
	}
}

void CNetworkVehicle::SetTargetSteer( float fSteer )
{
	if( IsSpawned() )
	{
		UpdateTargetSteer ();
		unsigned long ulCurrentTime = SharedUtility::GetTime ();
		float fCurrentSteer = GetSteer ();

		m_Interpolation.steer.fTarget = fSteer;
		m_Interpolation.steer.fError = (fSteer - fCurrentSteer);
		m_Interpolation.steer.fError *= Math::Lerp < const float > ( 0.25f, Math::UnlerpClamped ( 100, NETWORK_TICKRATE, 400 ), 1.0f );
		m_Interpolation.steer.ulStartTime = ulCurrentTime;
		m_Interpolation.steer.ulFinishTime = (ulCurrentTime + NETWORK_TICKRATE);
		m_Interpolation.steer.fLastAlpha = 0.0f;
	}
}

void CNetworkVehicle::UpdateTargetSteer( void )
{
	if( HasTargetSteer () )
	{
		float fCurrentSteer = GetSteer ();
		unsigned long ulCurrentTime = SharedUtility::GetTime ();
		float fAlpha = Math::Clamp ( 0.0f, Math::Unlerp ( m_Interpolation.steer.ulStartTime, ulCurrentTime, m_Interpolation.steer.ulFinishTime ), 1.0f );
		float fCurrentAlpha = (fAlpha - m_Interpolation.steer.fLastAlpha);

		m_Interpolation.steer.fLastAlpha = fAlpha;
		float fErrorCompensation = Math::Lerp ( 0.0f, fCurrentAlpha, m_Interpolation.steer.fError );
		if ( fAlpha == 1.0f )
			m_Interpolation.steer.ulFinishTime = 0;

		SetSteer ( (fCurrentSteer + fErrorCompensation) );
	}
}

void CNetworkVehicle::SetTargetSpeed( CVector3 vecSpeed )
{
	if( IsSpawned() )
	{
		UpdateTargetSpeed ();
		unsigned long ulCurrentTime = SharedUtility::GetTime ();
		CVector3 vecCurrentSpeed;
		GetSpeedVec ( &vecCurrentSpeed );

		m_Interpolation.speed.vecTarget = vecSpeed;
		m_Interpolation.speed.vecError = (vecSpeed - vecCurrentSpeed);
		m_Interpolation.speed.vecError *= Math::Lerp < const float > ( 0.25f, Math::UnlerpClamped ( 100, NETWORK_TICKRATE, 400 ), 1.0f );
		m_Interpolation.speed.ulStartTime = ulCurrentTime;
		m_Interpolation.speed.ulFinishTime = (ulCurrentTime + NETWORK_TICKRATE);
		m_Interpolation.speed.fLastAlpha = 0.0f;
	}
}

void CNetworkVehicle::UpdateTargetSpeed( void )
{
	if( HasTargetSpeed() )
	{
		CVector3 vecCurrentSpeed;
		GetSpeedVec ( &vecCurrentSpeed );

		unsigned long ulCurrentTime = SharedUtility::GetTime ();
		float fAlpha = Math::Clamp ( 0.0f, Math::Unlerp ( m_Interpolation.speed.ulStartTime, ulCurrentTime, m_Interpolation.speed.ulFinishTime ), 1.0f );
		float fCurrentAlpha = (fAlpha - m_Interpolation.speed.fLastAlpha);

		m_Interpolation.speed.fLastAlpha = fAlpha;

		CVector3 vecErrorCompensation = Math::Lerp ( CVector3(), fCurrentAlpha, m_Interpolation.speed.vecError );

		if ( fAlpha == 1.0f )
			m_Interpolation.speed.ulFinishTime = 0;

		SetSpeedVec ( (vecCurrentSpeed + vecErrorCompensation) );
	}
}

bool CNetworkVehicle::GetClosestPlayer( CNetworkPlayer ** pNetworkPlayer )
{
	if( IsSpawned() )
	{
		CVector3 vecPosition;
		GetPosition( &vecPosition );

		CNetworkPlayer * pClosestPlayer = NULL;
		float fCurrentDistance = 100.0f;

		for( EntityId i = 0; i < MAX_PLAYERS; i++ )
		{
			if( CCore::Instance()->GetPlayerManager()->IsActive( i ) || i == CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetId() )
			{
				bool bLocalPlayer = (i == CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetId());

				CVector3 vecCurrentPosition;
				if( bLocalPlayer )
					CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetPosition( &vecCurrentPosition );
				else
					CCore::Instance()->GetPlayerManager()->Get( i )->GetPosition( &vecCurrentPosition );

				float fDistance = Math::GetDistanceBetweenPoints( vecCurrentPosition, vecPosition );

				if( fDistance < fCurrentDistance )
				{
					fCurrentDistance = fDistance;
					if( bLocalPlayer )
						pClosestPlayer = CCore::Instance()->GetPlayerManager()->GetLocalPlayer();
					else
						pClosestPlayer = CCore::Instance()->GetPlayerManager()->Get( i );
				}
			}
		}
		if( pClosestPlayer )
		{
			*pNetworkPlayer = pClosestPlayer;
			return true;
		}
	}

	return false;
}

void CNetworkVehicle::ProcessUnoccupiedSync( RakNet::BitStream * pBitStream )
{
	UnoccupiedVehicleSync unoccupiedVehicleSync;
	pBitStream->Read( (char *)&unoccupiedVehicleSync, sizeof(UnoccupiedVehicleSync) );

	/*
	if( !m_pVehicle )
		return;

	if( !CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->IsSpawned() )
		return;

	SetPosition( unoccupiedVehicleSync.m_vecPosition ); // todo: ::Interpolate() only works if there's a driver! (check if there's a syncer instead!)
	SetDirection( unoccupiedVehicleSync.m_vecDirection );
	SetRotation( unoccupiedVehicleSync.m_quatRotation );
	*/
}

void CNetworkVehicle::AttachBlip ( CBlip * pBlip )
{
	if ( pBlip )
	{
		m_pAttachedBlip = pBlip;
		m_bBlipAttached = pBlip->IsCreated ();
		DEBUG_LOG ( "CNetworkVehicle::AttachBlip () - 0x%p, %s", m_pAttachedBlip, (m_bBlipAttached ? "true" : "false") );
	}
}

void CNetworkVehicle::DetachBlip ( void )
{
	if ( m_pAttachedBlip )
	{
		m_pAttachedBlip->Detach ();
		m_pAttachedBlip = NULL;
		m_bBlipAttached = false;
		DEBUG_LOG ( "CNetworkVehicle::DetachBlip ()" );
	}
}
