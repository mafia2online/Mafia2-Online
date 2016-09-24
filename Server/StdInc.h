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
#include "WindowsIncludes.h"
#include	<windowsx.h>
#else
#include "Linux.h"
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
#include "Interfaces/CMultiplayerBlip.h"
#include "Interfaces/CMultiplayerPlayer.h"
#include "Interfaces/CMultiplayerVehicle.h"
#include "Interfaces/CMultiplayerBlipManager.h"
#include "Interfaces/CMultiplayerVehicleManager.h"
#include "Interfaces/CMultiplayerPlayerManager.h"
#include "Interfaces/CCoreInterface.h"

// Shared Library
#include "CCommon.h"
#include "CRC.h"
#include "SharedUtility.h"
#include "CLogFile.h"
#include "CString.h"
#include "CColor.h"
#include "Math/CVector3.h"
#include "Math/CMaths.h"
#include "CSync.h"
#include "CNetworkRPC.h"
#include "CSettings.h"
#include "md5/md5.h"
#include "Threading/CMutex.h"
#include "Threading/CThread.h"
#include "Scripting/CScriptingManager.h"
#include "Scripting/CSquirrel.h"
#include "Scripting/CSquirrelArguments.h"
#include "Scripting/CSquirrelCommon.h"
#include "CEvents.h"
#include "CCommands.h"
#include "Timers/CTimerManager.h"
#include "Network/CNetworkStats.h"
#include "CXML.h"
#include "Game/CGame.h"

// RakNet
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "MessageIdentifiers.h"
#include "RPC4Plugin.h"

// Shared Scripting Natives
#include "Scripting/Natives/CSystemNatives.h"
#include "Scripting/Natives/CEventNatives.h"
#include "Scripting/Natives/CTimerNatives.h"
#include "Scripting/Natives/CAreaNatives.h"
#include "Scripting/Natives/CBlipNatives.h"
#include "Scripting/Natives/CUtilNatives.h"
#include "Scripting/Natives/CSQLiteNatives.h"
#include "Scripting/Natives/CXMLNatives.h"
#include "Scripting/Natives/CSharedPlayerNatives.h"
#include "Scripting/Natives/CSharedVehicleNatives.h"
#include "Scripting/Natives/CHashNatives.h"

// Server Scripting Natives
#include "CServerNatives.h"
#include "CPlayerNatives.h"
#include "CBanNatives.h"
#include "CVehicleNatives.h"
#include "CHttpNatives.h"

// General
#include "CBanManager.h"
#include "CModuleManager.h"
#include "CResourceManager.h"
#include "CWhoWas.h"

// Network
#include "CPedManager.h"
#include "CNetworkVehicle.h"
#include "CVehicleManager.h"
#include "CNetworkPlayer.h"
#include "CPlayerManager.h"
#include "CBlipManager.h"
#include "CNetworkRPC.h"
#include "CNetworkModule.h"
#include "CMasterList.h"
#include "CQuery.h"
#include "CWebServer.h"
#include "CClientScriptingManager.h"
#include "CWebRequest.h"

// Project
#include "CCore.h"