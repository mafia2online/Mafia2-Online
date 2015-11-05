/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CNetworkRPC.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

// Default RPC's
#define		RPC_INITIAL_DATA			"RPC_InitialData"
#define		RPC_CONNECTION_REJECTED		"RPC_ConnectionRejected"
#define		RPC_NEW_PLAYER				"RPC_NewPlayer"
#define		RPC_REMOVE_PLAYER			"RPC_RemovePlayer"
#define		RPC_PLAYER_SYNC				"RPC_PlayerSync"
#define		RPC_PLAYER_CHAT				"RPC_PlayerChat"
#define		RPC_PLAYER_DEATH			"RPC_PlayerDeath"
#define		RPC_PLAYER_SPAWN			"RPC_PlayerSpawn"
#define		RPC_PLAYER_RESPAWN			"RPC_PlayerRespawn"
#define		RPC_NEWBLIP					"RPC_NewBlip"
#define		RPC_REMOVEBLIP				"RPC_RemoveBlip"
#define		RPC_KICKPLAYER				"RPC_KickPlayer"
#define		RPC_PLAYERCHANGENICK		"RPC_PlayerChangeNick"
#define		RPC_PLAYERSYNCVEHICLE		"RPC_PlayerSyncVehicle"
#define		RPC_PLAYERSTOPSYNCVEHICLE	"RPC_PlayerStopSyncVehicle"
#define		RPC_PLAYERPING				"RPC_PlayerPing"
#define		RPC_PLAYERDAMAGE			"RPC_PlayerDamage"

// Vehicle RPC's
#define		RPC_NEW_VEHICLE				"RPC_NewVehicle"
#define		RPC_REMOVE_VEHICLE			"RPC_RemoveVehicle"
#define		RPC_VEHICLE_SYNC			"RPC_VehicleSync"
#define		RPC_PASSENGER_SYNC			"RPC_PassengerSync"
#define		RPC_UNOCCUPIED_SYNC			"RPC_UnoccupiedSync"
#define		RPC_ENTER_VEHICLE			"RPC_EnterVehicle"
#define		RPC_ENTER_VEHICLE_DONE		"RPC_EnterVehicleDone"
#define		RPC_EXIT_VEHICLE			"RPC_ExitVehicle"
#define		RPC_SPAWNVEHICLE			"RPC_SpawnVehicle"
#define		RPC_RESPAWNVEHICLE			"RPC_RespawnVehicle"
#define		RPC_MOVETODRIVER			"RPC_MoveToDriver"

// Clientscript RPC's
#define		RPC_NEWFILE					"RPC_NewFile"
#define		RPC_DELETEFILE				"RPC_DeleteFile"

// Ped RPC's
#define		RPC_NEWPED					"RPC_NewPed"
#define		RPC_REMOVEPED				"RPC_RemovePed"
#define		RPC_SETPEDMODEL				"RPC_Script_SetPedModel"

// Player scripting RPC's
#define		RPC_SETPLAYERPOS			"RPC_Script_SetPlayerPosition"
#define		RPC_SETPLAYERROT			"RPC_Script_SetPlayerRotation"
#define		RPC_SENDPLAYERMESSAGE		"RPC_Script_SendPlayerMessage"
#define		RPC_SETPLAYERHEALTH			"RPC_Script_SetPlayerHealth"
#define		RPC_GIVEPLAYERWEAPON		"RPC_Script_GivePlayerWeapon"
#define		RPC_REMOVEPLAYERWEAPON		"RPC_Script_RemovePlayerWeapon"
#define		RPC_SETPLAYERCOLOUR			"RPC_Script_SetPlayerColour"
#define		RPC_SETPLAYERWEATHER		"RPC_Script_SetPlayerWeather"
#define		RPC_TOGGLEPLAYERHUD			"RPC_Script_TogglePlayerHud"
#define		RPC_SETPLAYERMONEY			"RPC_Script_SetPlayerMoney"
#define		RPC_GIVEPLAYERMONEY			"RPC_Script_GivePlayerMoney"
#define		RPC_TAKEPLAYERMONEY			"RPC_Script_TakePlayerMoney"
#define		RPC_TOGGLEPLAYERCONTROLS	"RPC_Script_TogglePlayerControls"
#define		RPC_PLAYSOUND				"RPC_Script_PlaySound"
#define		RPC_STOPSOUND				"RPC_Script_StopSound"
#define		RPC_SETPLAYERMODEL			"RPC_Script_SetPlayerModel"
#define		RPC_PUTINVEHICLE			"RPC_Script_PutPlayerInVehicle"
#define		RPC_REMOVEFROMVEHICLE		"RPC_Script_RemovePlayerFromVehicle"

// Vehicle scripting RPC's
#define		RPC_SETVEHICLEPOS			"RPC_Script_SetVehiclePosition"
#define		RPC_SETVEHICLEDIR			"RPC_Script_SetVehicleDirection"
#define		RPC_SETVEHICLECOLOUR		"RPC_Script_SetVehicleColour"
#define		RPC_TRIGGEREVENT			"RPC_Script_EventCall"
#define		RPC_SETVEHICLEPLATETEXT		"RPC_Script_SetVehiclePlateText"
#define		RPC_REPAIRVEHICLE			"RPC_Script_RepairVehicle"
#define		RPC_SETVEHICLEDIRTLEVEL		"RPC_Script_SetVehicleDirtLevel"
#define		RPC_SETVEHICLEENGINESTATE	"RPC_Script_SetVehicleEngineState"
#define		RPC_EXPLODEVEHICLE			"RPC_Script_ExplodeVehicle"
#define		RPC_SETVEHICLEPARTOPEN		"RPC_Script_SetVehiclePartOpen"
#define		RPC_SETVEHICLESIRENSTATE	"RPC_Script_SetVehicleSirenState"
#define		RPC_SETVEHICLEHORNSTATE		"RPC_Script_SetVehicleHornState"
#define		RPC_SETVEHICLEWINDOWOPEN	"RPC_Script_SetVehicleWindowOpen"
#define		RPC_SETVEHICLETUNINGTABLE	"RPC_Script_SetVehicleTuningTable"
#define		RPC_SETVEHICLEWHEELTEXTURE	"RPC_Script_SetVehicleWheelTexture"
#define		RPC_SETVEHICLESPEED			"RPC_Script_SetVehicleSpeed"
#define		RPC_SETVEHICLEFUEL			"RPC_Script_SetVehicleFuel"
#define		RPC_SETVEHICLELIGHTSTATE	"RPC_Script_SetVehicleLightState"

// Blip scripting RPC's
#define		RPC_ATTACHBLIPTOPLAYER		"RPC_Script_AttachBlipToPlayer"
#define		RPC_ATTACHBLIPTOVEHICLE		"RPC_Script_AttachBlipToVehicle"
#define		RPC_ATTACHBLIPTOPED			"RPC_Script_AttachBlipToPed"