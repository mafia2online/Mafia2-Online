/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CCommon.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once
#include	"../Shared/CLogFile.h"

// Core defines

#ifdef _DEBUG
#define		MOD_NAME				"Mafia 2 Multiplayer - DEBUG MODE"
#else
#define		MOD_NAME				"Mafia 2 Multiplayer"
#endif
#define		MOD_VERS_STR			"0.1b-rc3"

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
#define		MAX_PEDS				500
#define		MAX_PLAYER_NAME			32

// Network
#define		NETWORK_TIMEOUT			3000
#define		NETWORK_VERSION			0x1B1
#define		NETWORK_TICKRATE		100

// Macros
#define		ARRAY_LENGTH(array)			(sizeof(array) / sizeof(array[0]))
#define		SAFE_DELETE(memory)			if(memory) { delete memory; memory = NULL; }
#define		SAFE_RELEASE(p)				{ if ( (p) ) { (p)->Release(); (p) = NULL; } }
#define		PAD(prefix, name, size)		BYTE prefix##_##name##[size]

// Bit manipulation macros
#define SET_BIT(a, b) a |= b
#define IS_BIT_SET(a, b) ((a & b) != 0)
#define UNSET_BIT(a, b) a &= ~(b)

#ifdef DEBUG_LOG
#undef DEBUG_LOG
#endif

// Debug
static void	OutputDebugLog ( char * szText )
{
#ifdef DEBUG_LOG
	CLogFile::Open( "logs\\debug.log", true );
	CLogFile::Print( szText );
	CLogFile::Close();
	CLogFile::Open( "logs\\core.log", true );
#endif
}
#define	DEBUG_TRACE(szText)				OutputDebugLog(szText);

// Defines
#define	UNBAN_NEVER						-1

// Entitys
typedef unsigned int EntityId;
#define	INVALID_ENTITY_ID				(MAX_PLAYERS + 1)
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