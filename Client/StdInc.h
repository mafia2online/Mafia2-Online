/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : StdInc.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#define DIRECTINPUT_VERSION		0x0800

// OS Dependant includes
#include <WindowsIncludes.h>
#include <windowsx.h>

// Core includes
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <detours.h>
#include <stdio.h>
#include <aclapi.h>
#include <process.h>
#include <map>
#include <list>
#include <queue>
#include <string>
#include <atomic> // C++11
#include <thread> // C++11

// Shared Library
#include <CCommon.h>
#include <CRC.h>
#include <SharedUtility.h>
#include <CLogFile.h>
#include <CString.h>
#include <CPatcher.h>
#include <CColor.h>
#include <Math/CVector3.h>
#include <Math/CMaths.h>
#include <Math/CMatrix.h>
#include <CSync.h>
#include <CNetworkRPC.h>
#include <CSettings.h>
#include <md5/md5.h>
#include <CZlib.h>
#include <Scripting/CScriptingManager.h>
#include <Scripting/CSquirrel.h>
#include <Scripting/CSquirrelArguments.h>
#include <Scripting/CSquirrelCommon.h>
#include <CEvents.h>
#include <CCommands.h>
#include <CExceptionHandler.h>
#include <CXML.h>
#include <CWPMHook.h>
#include <Network/CNetworkStats.h>
#include <Game/CGame.h>
#include <CrashRpt.h>

// RakNet includes
#include <RakPeerInterface.h>
#include <BitStream.h>
#include <MessageIdentifiers.h>
#include <RPC4Plugin.h>

// Lua
#include <lua.h>

// zlib
#include <zlib.h>

// Libpng
#include <png.h>

// Direct X Hook
#include "CDirect3D9Proxy.h"
#include "CDirect3DDevice9Proxy.h"
#include "CDirect3D9Hook.h"

// Dirext Input Hook
#include "CDirectInput8Proxy.h"
#include "CDirectInputDevice8Proxy.h"
#include "CDirectInput8Hook.h"

// Window hook
#include "CWindowSubclass.h"

// Mouse
#include "CMouseHandler.h"

// Game
#include "CKeyBinds.h"
#include "CGameFiles.h"
#include "COffsets.h"
#include "CPatches.h"
#include "CStreamer.h"

// Engine
#include "CM2PhysFS.h"
#include "CM2VideoSettings.h"
#include "CM2SyncObject.h"
#include "CM2ScriptMachine.h"
#include "CM2EntityMessage.h"
#include "CM2Camera.h"
#include "CM2Hud.h"
#include "CIE.h"
#include "CLua.h"
#include "CM2Enums.h"

// Engine new
#include "engine/CM2Object.h"
#include "engine/CM2SDSTables.h"
#include "engine/CM2BaseEntity.h"
#include "engine/CM2Entity.h"
#include "engine/CM2Vehicle.h"
#include "engine/CM2Ped.h"
#include "engine/CM2PlayerPed.h"
#include "engine/CM2Navigation.h"
#include "engine/CM2SlotManager.h"
#include "engine/CM2ObjectTable.h"
#include "engine/CM2Core.h"
#include "engine/CM2Door.h"

// Model Mgr
#include "CModelManager.h"

// GUI
#include "CGUIEvent.h"
#include "CGUICallback.h"
#include "CGUI.h"

// GUI Impl
#include "gui_impl/CGUI_Impl.h"
#include "gui_impl/CGUIFont_Impl.h"
#include "gui_impl/CGUIWindow_Impl.h"
#include "gui_impl/CGUIProgressBar_Impl.h"
#include "gui_impl/CGUITexture_Impl.h"
#include "gui_impl/CGUIStaticImage_Impl.h"
#include "gui_impl/CGUIButton_Impl.h"
#include "gui_impl/CGUICheckBox_Impl.h"
#include "gui_impl/CGUIComboBox_Impl.h"
#include "gui_impl/CGUIEdit_Impl.h"
#include "gui_impl/CGUILabel_Impl.h"
#include "gui_impl/CGUIRadioButton_Impl.h"
#include "gui_impl/CGUITabPanel_Impl.h"
#include "gui_impl/CGUIListItem_Impl.h"
#include "gui_impl/CGUIGridList_Impl.h"
#include "gui_impl/CGUIScrollBar_Impl.h"
#include "gui_impl/CGUIScrollPane_Impl.h"
#include "gui_impl/CGUIMessageBox_Impl.h"

// GUI
#include "CServerPassword.h"
#include "CServerQuery.h"
#include "CMenuSettings.h"
#include "CDownloadProgress.h"
#include "CServerBrowser.h"
#include "CMainMenu.h"

// Graphics
#include "CFont.h"
#include "CGraphics.h"
#include "CChat.h"
#include "CFPSCounter.h"
#include "CScreenShot.h"
#include "CLoadingScreen.h"

// Network
#include "CNetworkModelManager.h"
#include "CPed.h"
#include "CPedManager.h"
#include "CNetworkVehicle.h"
#include "CVehicleManager.h"
#include "CNetworkPlayer.h"
#include "CLocalPlayer.h"
#include "CRemotePlayer.h"
#include "CPlayerManager.h"
#include "CBlipManager.h"
#include "CFileTransfer.h"
#include "CFileTransferManager.h"
#include "CNetworkRPC.h"
#include "CScriptingRPC.h"
#include "CNetworkModule.h"
#include "CMasterList.h"

// Shared scripting natives
#include <Scripting/Natives/CEventNatives.h>
#include <Scripting/Natives/CSystemNatives.h>
#include <Scripting/Natives/CAreaNatives.h>
#include <Scripting/Natives/CTimerNatives.h>
#include <Scripting/Natives/CBlipNatives.h>
#include <Scripting/Natives/CUtilNatives.h>
#include <Scripting/Natives/CXMLNatives.h>
#include <Scripting/Natives/CPedNatives.h>
#include <Scripting/Natives/CSharedPlayerNatives.h>
#include <Scripting/Natives/CSharedVehicleNatives.h>
#include <Scripting/Natives/CHashNatives.h>

// Client scripting natives
#include "CGraphicsNatives.h"
#include "CPlayerNatives.h"
#include "CGUINatives.h"
#include "CGameNatives.h"
#include "CCameraNatives.h"
#include "CClientNatives.h"

// Clientscripts
#include <Timers/CTimer.h>
#include <Timers/CTimerManager.h>
#include "CClientScriptGUIManager.h"
#include "CClientScriptingManager.h"

// Project
#include "CMafia.h"
#include "CCore.h"