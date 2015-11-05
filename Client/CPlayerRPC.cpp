/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CPlayerRPC.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"
#include	"CPlayerRPC.h"

extern	CCore			* pCore;
bool	CPlayerRPC::m_bRegistered = false;

void SetPlayerPosition( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the position
	CVector3 vecPosition;
	pBitStream->Read( vecPosition );

	// Is the localplayer valid?
	if( pCore->GetPlayerManager()->GetLocalPlayer() )
	{
		// Set the player position
		pCore->GetPlayerManager()->GetLocalPlayer()->Teleport( vecPosition );
	}
}

void SetPlayerRotation( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the rotation
	CVector3 vecRotation;
	pBitStream->Read( vecRotation );

	// Is the localplayer valid?
	if( pCore->GetPlayerManager()->GetLocalPlayer() )
	{
		// Set the player direction
		pCore->GetPlayerManager()->GetLocalPlayer()->SetRotation( vecRotation );
	}
}

void SendPlayerMessage( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the message
	RakNet::RakString strMessage;
	pBitStream->Read( strMessage );

	// Read the red value
	int red;
	pBitStream->ReadCompressed( red );

	// Read the green value
	int green;
	pBitStream->ReadCompressed( green );

	// Read the blue value
	int blue;
	pBitStream->ReadCompressed( blue );

	// Output the message
	pCore->GetChat()->AddInfoMessage( CColor( red, green, blue, 255 ), strMessage.C_String() );
}

void SetPlayerHealth( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the health
	float fHealth;
	pBitStream->Read( fHealth );

	// Is the localplayer valid?
	if( pCore->GetPlayerManager()->GetLocalPlayer() )
	{
		// Set the player health
		pCore->GetPlayerManager()->GetLocalPlayer()->SetHealth( fHealth );
	}
}

void GivePlayerWeapon( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the weapon
	int iWeapon;
	pBitStream->ReadCompressed( iWeapon );

	// Read the ammo
	int iAmmo;
	pBitStream->ReadCompressed( iAmmo );

	// Is the localplayer valid?
	if( pCore->GetPlayerManager()->GetLocalPlayer() )
	{
		// Give the player the weapon
		pCore->GetPlayerManager()->GetLocalPlayer()->GiveWeapon( iWeapon, iAmmo );
	}
}

void RemovePlayerWeapon( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the weapon id
	int iWeapon;
	pBitStream->ReadCompressed( iWeapon );

	// Read the ammo
	int iAmmo;
	pBitStream->ReadCompressed( iAmmo );

	// Is the localplayer valid?
	if( pCore->GetPlayerManager()->GetLocalPlayer() )
	{
		// Remove the weapon / ammo
		pCore->GetPlayerManager()->GetLocalPlayer()->RemoveWeapon( iWeapon, iAmmo );
	}
}

void SetPlayerColour( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the playerid
	EntityId playerId;
	pBitStream->ReadCompressed( playerId );

	// Read the colour
	unsigned int uiColour;
	pBitStream->Read( uiColour );

	// Is this the localplayer id?
	if( pCore->GetPlayerManager()->GetLocalPlayer()->GetId() == playerId )
	{
		// Set the localplayer colour
		pCore->GetPlayerManager()->GetLocalPlayer()->SetColour( uiColour );
	}
	else
	{
		// Get a pointer to the player
		CRemotePlayer * pRemotePlayer = pCore->GetPlayerManager()->Get(playerId);

		// Is the player pointer valid?
		if( pRemotePlayer )
		{
			// Set the player colour
			pRemotePlayer->SetColour( uiColour );
		}
	}
}

void SetPlayerWeather( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the weather string
	RakNet::RakString strWeather;
	pBitStream->Read( strWeather );

	// Change the weather
	pCore->GetGame()->ChangeWeather( strWeather.C_String() );
}

void TogglePlayerHud( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the toggle
	bool bToggle;
	pBitStream->Read( bToggle );

	// Toggle the hud
	if( pCore->GetHud() )
		pCore->GetHud()->Show( bToggle );
}

void SetPlayerMoney( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the money
	int iMoney;
	pBitStream->Read( iMoney );

	// Is the localplayer valid?
	if( pCore->GetPlayerManager()->GetLocalPlayer() )
	{
		// Get the player money
		int iCurrentMoney = pCore->GetPlayerManager()->GetLocalPlayer()->GetMoney( );

		// Remove the current money
		pCore->GetPlayerManager()->GetLocalPlayer()->RemoveMoney( iCurrentMoney );

		// Give the player money
		pCore->GetPlayerManager()->GetLocalPlayer()->GiveMoney( iMoney );
	}
}

void GivePlayerMoney( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the money
	int iMoney;
	pBitStream->Read( iMoney );

	// Is the localplayer valid?
	if( pCore->GetPlayerManager()->GetLocalPlayer() )
	{
		// Set the player money
		pCore->GetPlayerManager()->GetLocalPlayer()->GiveMoney( iMoney );
	}
}

void TakePlayerMoney( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the money
	int iMoney;
	pBitStream->Read( iMoney );

	// Is the localplayer valid?
	if( pCore->GetPlayerManager()->GetLocalPlayer() )
	{
		// Set the player money
		pCore->GetPlayerManager()->GetLocalPlayer()->RemoveMoney( iMoney );
	}
}

void TogglePlayerControls( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the toggle
	bool bToggle;
	pBitStream->Read( bToggle );

	// Is the localplayer valid?
	if( pCore->GetPlayerManager()->GetLocalPlayer() )
	{
		// Toggle the controls
		pCore->GetPlayerManager()->GetLocalPlayer()->LockControls( !bToggle );
	}
}

void PlaySound( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the input
	RakNet::RakString strMusic;
	pBitStream->Read( strMusic );

	// Play the music
	pCore->GetGame()->PlayMusic( strMusic.C_String() );
}

void StopSound( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the input
	RakNet::RakString strAnimation;
	pBitStream->Read( strAnimation );

	// Stop the music
	pCore->GetGame()->StopMusic();
}

void SetPlayerModel( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the model id
	unsigned int uiModel;
	pBitStream->ReadCompressed( uiModel );

	// Set the localplayer model
	pCore->GetPlayerManager()->GetLocalPlayer()->SetModel( uiModel, pCore->GetPlayerManager()->GetLocalPlayer()->IsSpawned() );
}

void PlayerPing( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Pass this to the localplayer
	pCore->GetPlayerManager()->GetLocalPlayer()->PingReturn ( pBitStream );
}

void PutPlayerInVehicle( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	CLogFile::Print ( "1" );

	// Read the player id
	EntityId playerId;
	pBitStream->ReadCompressed ( playerId );

	CLogFile::Print ( "2" );

	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed ( vehicleId );

	CLogFile::Print ( "3" );

	// Read the seat id
	EntityId seatId;
	pBitStream->ReadCompressed ( seatId );

	CLogFile::Print ( "4" );

	// Get the player instance
	CNetworkPlayer * pPlayer = NULL;

	CLogFile::Print ( "5" );

	// Is this the localplayer?
	if ( playerId == pCore->GetPlayerManager()->GetLocalPlayer()->GetId () )
		pPlayer = pCore->GetPlayerManager()->GetLocalPlayer();
	else
		pCore->GetPlayerManager()->Get ( playerId );

	CLogFile::Printf ( "PutPlayerInVehicle - %d, %d, %d", playerId, vehicleId, seatId );

	// Is the player instance valid?
	if ( pPlayer )
	{
		pPlayer->PutInVehicle ( pCore->GetVehicleManager()->Get( vehicleId ), seatId );
	}
}

void RemovePlayerFromVehicle( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the player id
	EntityId playerId;
	pBitStream->ReadCompressed ( playerId );

	// Get the player instance
	CNetworkPlayer * pPlayer = NULL;

	// Is this the localplayer?
	if ( playerId == pCore->GetPlayerManager()->GetLocalPlayer()->GetId () )
		pPlayer = pCore->GetPlayerManager()->GetLocalPlayer();
	else
		pCore->GetPlayerManager()->Get ( playerId );

	CLogFile::Printf ( "RemovePlayerFromVehicle - %d", playerId );

	// Is the player instance valid?
	if ( pPlayer )
	{
		// Remove the player from the vehicle
		pPlayer->RemoveFromVehicle ( pPlayer->GetVehicle() );
	}
}

void CPlayerRPC::Register( RakNet::RPC4 * pRPC )
{
	// Are we already registered?
	if( m_bRegistered )
		return;

	// Player functions
	pRPC->RegisterFunction( RPC_SETPLAYERPOS, SetPlayerPosition );
	pRPC->RegisterFunction( RPC_SETPLAYERROT, SetPlayerRotation );
	pRPC->RegisterFunction( RPC_SENDPLAYERMESSAGE, SendPlayerMessage );
	pRPC->RegisterFunction( RPC_SETPLAYERHEALTH, SetPlayerHealth );
	pRPC->RegisterFunction( RPC_GIVEPLAYERWEAPON, GivePlayerWeapon );
	pRPC->RegisterFunction( RPC_REMOVEPLAYERWEAPON, RemovePlayerWeapon );
	pRPC->RegisterFunction( RPC_SETPLAYERCOLOUR, SetPlayerColour );
	pRPC->RegisterFunction( RPC_SETPLAYERWEATHER, SetPlayerWeather );
	pRPC->RegisterFunction( RPC_TOGGLEPLAYERHUD, TogglePlayerHud );
	pRPC->RegisterFunction( RPC_SETPLAYERMONEY, SetPlayerMoney );
	pRPC->RegisterFunction( RPC_GIVEPLAYERMONEY, GivePlayerMoney );
	pRPC->RegisterFunction( RPC_TAKEPLAYERMONEY, TakePlayerMoney );
	pRPC->RegisterFunction( RPC_TOGGLEPLAYERCONTROLS, TogglePlayerControls );
	pRPC->RegisterFunction( RPC_PLAYSOUND, PlaySound );
	pRPC->RegisterFunction( RPC_STOPSOUND, StopSound );
	pRPC->RegisterFunction( RPC_SETPLAYERMODEL, SetPlayerModel );
	pRPC->RegisterFunction( RPC_PLAYERPING, PlayerPing );
	pRPC->RegisterFunction( RPC_PUTINVEHICLE, PutPlayerInVehicle );
	pRPC->RegisterFunction( RPC_REMOVEFROMVEHICLE, RemovePlayerFromVehicle );
}

void CPlayerRPC::Unregister( RakNet::RPC4 * pRPC )
{
	// Are we not registered?
	if( !m_bRegistered )
		return;

	// Player functions
	pRPC->UnregisterFunction( RPC_SETPLAYERPOS );
	pRPC->UnregisterFunction( RPC_SETPLAYERROT );
	pRPC->UnregisterFunction( RPC_SENDPLAYERMESSAGE );
	pRPC->UnregisterFunction( RPC_SETPLAYERHEALTH );
	pRPC->UnregisterFunction( RPC_GIVEPLAYERWEAPON );
	pRPC->UnregisterFunction( RPC_REMOVEPLAYERWEAPON );
	pRPC->UnregisterFunction( RPC_SETPLAYERCOLOUR );
	pRPC->UnregisterFunction( RPC_SETPLAYERWEATHER );
	pRPC->UnregisterFunction( RPC_TOGGLEPLAYERHUD );
	pRPC->UnregisterFunction( RPC_SETPLAYERMONEY );
	pRPC->UnregisterFunction( RPC_GIVEPLAYERMONEY );
	pRPC->UnregisterFunction( RPC_TAKEPLAYERMONEY );
	pRPC->UnregisterFunction( RPC_TOGGLEPLAYERCONTROLS );
	pRPC->UnregisterFunction( RPC_PLAYSOUND );
	pRPC->UnregisterFunction( RPC_STOPSOUND );
	pRPC->UnregisterFunction( RPC_SETPLAYERMODEL );
	pRPC->UnregisterFunction( RPC_PLAYERPING );
	pRPC->UnregisterFunction( RPC_PUTINVEHICLE );
	pRPC->UnregisterFunction( RPC_REMOVEFROMVEHICLE );
}