/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : StdInc.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma warning(disable: 4996)

// OS Dependant includes
#ifdef _WIN32
#include	"../Libraries/RakNet/Source/WindowsIncludes.h"
#include	<windowsx.h>
#include	<CrashRpt.h>
#else
#include	"../Shared/Linux.h"
#endif

// Core includes
#include	<stdio.h>
#include	<stdarg.h>
#include	<list>
#include	<map>
#include	<queue>
#include	<math.h>
#include	<ctime>
#include	<assert.h>

// Software Development Kit
#include	"Interfaces/CMultiplayerBlip.h"
#include	"Interfaces/CMultiplayerPlayer.h"
#include	"Interfaces/CMultiplayerVehicle.h"
#include	"Interfaces/CMultiplayerBlipManager.h"
#include	"Interfaces/CMultiplayerVehicleManager.h"
#include	"Interfaces/CMultiplayerPlayerManager.h"
#include	"Interfaces/CCoreInterface.h"

// Shared Library
#include	"../Shared/CCommon.h"
#include	"../Shared/CRC.h"
#include	"../Shared/SharedUtility.h"
#include	"../Shared/CLogFile.h"
#include	"../Shared/CString.h"
#include	"../Shared/CColor.h"
#include	"../Shared/Math/CVector3.h"
#include	"../Shared/Math/CMaths.h"
#include	"../Shared/CSync.h"
#include	"../Shared/CNetworkRPC.h"
#include	"../Shared/CSettings.h"
#include	"../Shared/md5/md5.h"
#include	"../Shared/Threading/CMutex.h"
#include	"../Shared/Threading/CThread.h"
#include	"../Shared/Scripting/CScriptingManager.h"
#include	"../Shared/Scripting/CSquirrel.h"
#include	"../Shared/Scripting/CSquirrelArguments.h"
#include	"../Shared/Scripting/CSquirrelCommon.h"
#include	"../Shared/CEvents.h"
#include	"../Shared/CCommands.h"
#include	"../Shared/Timers/CTimerManager.h"
#include	"../Shared/Network/CNetworkStats.h"
#include	"../Shared/CXML.h"

// RakNet
#include	"../Libraries/RakNet/Source/RakPeerInterface.h"
#include	"../Libraries/RakNet/Source/BitStream.h"
#include	"../Libraries/RakNet/Source/MessageIdentifiers.h"
#include	"../Libraries/RakNet/Source/RPC4Plugin.h"

// Shared Scripting Natives
#include	"../Shared/Scripting/Natives/CSystemNatives.h"
#include	"../Shared/Scripting/Natives/CEventNatives.h"
#include	"../Shared/Scripting/Natives/CTimerNatives.h"
#include	"../Shared/Scripting/Natives/CAreaNatives.h"
#include	"../Shared/Scripting/Natives/CBlipNatives.h"
#include	"../Shared/Scripting/Natives/CUtilNatives.h"
#include	"../Shared/Scripting/Natives/CSQLiteNatives.h"
#include	"../Shared/Scripting/Natives/CXMLNatives.h"
#include	"../Shared/Scripting/Natives/CSharedPlayerNatives.h"
#include	"../Shared/Scripting/Natives/CSharedVehicleNatives.h"
#include	"../Shared/Scripting/Natives/CHashNatives.h"

// Server Scripting Natives
#include	"CServerNatives.h"
#include	"CPlayerNatives.h"
#include	"CBanNatives.h"
#include	"CVehicleNatives.h"
#include	"CHttpNatives.h"

// General
#include	"CBanManager.h"
#include	"CModuleManager.h"
#include	"CResourceManager.h"
#include	"CWhoWas.h"

// Network
#include	"CPedManager.h"
#include	"CNetworkVehicle.h"
#include	"CVehicleManager.h"
#include	"CNetworkPlayer.h"
#include	"CPlayerManager.h"
#include	"CBlipManager.h"
#include	"CNetworkRPC.h"
#include	"CNetworkModule.h"
#include	"CMasterList.h"
#include	"CQuery.h"
#include	"CWebServer.h"
#include	"CClientScriptingManager.h"
#include	"CWebRequest.h"

// Project
#include	"CCore.h"