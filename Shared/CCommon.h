/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CCommon.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

// Core defines

#ifdef _DEBUG
#define		MOD_NAME				"Mafia2-Online - DEBUG MODE"
#else
#define		MOD_NAME				"Mafia2-Online"
#endif
#define		MOD_VERS_STR			"0.1 RC4"

// MasterList stuff

#define MASTERLIST_HOST				"mafia2-online.com"
#define MASTERLIST_NONE_URL_PATH	"/api/v1/server?ordered=true"
#define MASTERLIST_HOSTED_URL_PATH	"/api/v1/server?ordered=true&hosted=true"

// Server stuff
#define		SERVER_TITLE			MOD_NAME " - " MOD_OS_STRING " Dedicated Server"
#ifdef _WIN32
#define		MOD_OS_STRING			"Windows"
#define		LIB_EXTENSION			".dll"
#else
#define		MOD_OS_STRING			"Linux"
#define		LIB_EXTENSION			".so"
#endif

// Limits
#define		MAX_PLAYERS				1000
#define		MAX_VEHICLES			1000
#define		MAX_BLIPS				256
#define		MAX_PEDS				32
#define		MAX_3DTEXTS				32
#define		MAX_PLAYER_NAME			32

// Network
#define		NETWORK_TIMEOUT			3000
#define		NETWORK_VERSION			0x1B4
#define		NETWORK_TICKRATE		100

/** The default server listen port */
const unsigned DEFAULT_PORT			= 27015;

// Macros
#define		ARRAY_LENGTH(array)			(sizeof(array) / sizeof(array[0]))
#define		SAFE_DELETE(memory)			if(memory) { delete memory; memory = NULL; }
#define		SAFE_RELEASE(p)				{ if ( (p) ) { (p)->Release(); (p) = NULL; } }
#define		PAD(prefix, name, size)		BYTE prefix##_##name##[size]

// Bit manipulation macros
#define SET_BIT(a, b) a |= b
#define IS_BIT_SET(a, b) ((a & b) != 0)
#define UNSET_BIT(a, b) a &= ~(b)

// Defines
#define	UNBAN_NEVER						-1

// Entitys
typedef unsigned int EntityId;

/**
 * The invalid entity id.
 *
 * The maximum value of unsigned int seems to be reasonable as our limits are much smaller
 * and definitely we will not have any entity type which can be spawned in that amount.
 */
const unsigned INVALID_ENTITY_ID		= 0xFFFFFFFF;

#define	ENTITY_ID_CONSOLE				INVALID_ENTITY_ID

typedef EntityId ElementId;
#define	INVALID_ELEMENT_ID				INVALID_ENTITY_ID

// Vehicle stuff
#define	MAX_SEATS						4

// Enums
enum eNetworkState
{
	NETSTATE_NONE = 0,
	NETSTATE_TIMEOUT,

	/** Network state set when network client has postponed disconnecting operation */
	NETSTATE_DISCONNECTING,

	NETSTATE_DISCONNECTED,
	NETSTATE_CONNECTING,
	NETSTATE_CONNECTED
};

enum eDisconnectReason
{
	DISCONNECT_TIMEOUT = 0,
	DISCONNECT_QUIT,
	DISCONNECT_KICKED,
	DISCONNECT_BANNED
};

enum eRejectReason
{
	REJECT_REASON_VERSION = 0,
	REJECT_REASON_NICKNAME,
	REJECT_REASON_BANNED
};

enum ePlayerState
{
	PLAYERSTATE_UNKNOWN = 0,
	PLAYERSTATE_ONFOOT,
	PLAYERSTATE_DRIVER,
	PLAYERSTATE_PASSENGER,
	PLAYERSTATE_DEAD
};

enum eWebRequestType
{
	HTTP_TYPE_GET = 0,
	HTTP_TYPE_POST
};

enum eBlipType {
	BLIP_TYPE_STANDALONE = 0,
	BLIP_TYPE_PLAYER,
	BLIP_TYPE_VEHICLE,
	BLIP_TYPE_PED
};

enum eVehiclePart {
	VEHICLE_PART_HOOD = 0,
	VEHICLE_PART_TRUNK = 1
};