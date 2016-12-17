/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project : Client
* File : COffsets.cpp
* Developers : AaronLad <aaron@m2 - multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "COffsets.h"
#include "CPatcher.h"
#include "CPatches.h"

unsigned int COffsets::FUNC_CIE_CreateObject = 0;
unsigned int COffsets::FUNC_CIE_Assign_Model = 0;
unsigned int COffsets::FUNC_CIE_SetHashName = 0;
unsigned int COffsets::FUNC_CIE_SetFlag = 0;
unsigned int COffsets::FUNC_CIE_SetModel = 0;
unsigned int COffsets::FUNC_CIE_UnkFunc = 0;
unsigned int COffsets::FUNC_CIE_GetTrafficHuman = 0;
unsigned int COffsets::FUNC_CIE_SetTrafficHuman = 0;
unsigned int COffsets::FUNC_CIE_GetHashedString = 0;
unsigned int COffsets::FUNC_CIE_LoadModel = 0;
unsigned int COffsets::FUNC_CIE_CCarSetup = 0;
unsigned int COffsets::FUNC_CIE_SetCarParams = 0;

unsigned int COffsets::VAR_CGame = 0;
unsigned int COffsets::FUNC_CGame__OnGameInit = 0;
unsigned int COffsets::FUNC_CGame__OnGameEvent = 0;
unsigned int COffsets::FUNC_CGame__OnGameLoad = 0;
unsigned int COffsets::VAR_CGame__ScriptEngine = 0;
unsigned int COffsets::FUNC_CGame__Allocate = 0;
unsigned int COffsets::FUNC_CGame__Free = 0;
unsigned int COffsets::FUNC_CGame__AllocateBuffer = 0;

unsigned int COffsets::VAR_LocalPlayer = 0;
unsigned int COffsets::VAR_GuiMainSdsFolder = 0;
unsigned int COffsets::VAR_GuiMainSds = 0;
unsigned int COffsets::VAR_GuiSds = 0;
unsigned int COffsets::VAR_TablesSds = 0;
unsigned int COffsets::VAR_SDSConfigFile = 0;
unsigned int COffsets::VAR_LoadingScreen = 0;
unsigned int COffsets::VAR_StreamBin = 0;
unsigned int COffsets::FUNC_M2malloc = 0;
unsigned int COffsets::FUNC_OnGameProcessStart = 0;

unsigned int COffsets::VAR_CCore = 0;
unsigned int COffsets::FUNC_CCore__AllocateModel = 0;

unsigned int COffsets::FUNC_CDoor__Open = 0;
unsigned int COffsets::FUNC_CDoor__Kick = 0;
unsigned int COffsets::FUNC_CDoor__Close = 0;
unsigned int COffsets::FUNC_CDoor__Lock = 0;
unsigned int COffsets::FUNC_CDoor__Unlock = 0;
unsigned int COffsets::FUNC_CDoor__EnableAction = 0;
unsigned int COffsets::FUNC_CDoor__DisableAction = 0;

unsigned int COffsets::FUNC_CModelMgr__Load = 0;
unsigned int COffsets::FUNC_CModelMgr__Free = 0;
unsigned int COffsets::FUNC_CModelMgr__Construct = 0;
unsigned int COffsets::FUNC_CModelMgr__ChangeModel = 0;

unsigned int COffsets::VAR_CSlotManager = 0;

unsigned int COffsets::VAR_CObjectTable = 0;

unsigned int COffsets::FUNC_fnv64 = 0;
unsigned int COffsets::FUNC_fnv32 = 0;

unsigned int COffsets::VAR_CWindow = 0;

unsigned int COffsets::VAR_CSDSManager = 0;
unsigned int COffsets::FUNC_CSDSManager__ActivateStreamMapLine = 0;

unsigned int COffsets::FUNC_CEntMgr__ProcessEntities = 0;

unsigned int COffsets::VAR_CWrapperList = 0;
unsigned int COffsets::FUNC_CWrapperList__GetEntityByName = 0;

unsigned int COffsets::VAR_CCamera = 0;
unsigned int COffsets::VAR_CCamera__State = 0;
unsigned int COffsets::FUNC_CCamera__GetGameCameraFromIndex = 0;

unsigned int COffsets::FUNC_CLua__pcall = 0;
unsigned int COffsets::FUNC_CLua__loadbuffer = 0;
unsigned int COffsets::FUNC_CLua__settop = 0;
unsigned int COffsets::FUNC_CLua__gettop = 0;
unsigned int COffsets::FUNC_CLua__setfield = 0;
unsigned int COffsets::FUNC_CLua__tolstring = 0;
unsigned int COffsets::FUNC_CLua__pushcclosure = 0;
unsigned int COffsets::FUNC_CLua__tointeger = 0;
unsigned int COffsets::FUNC_CLua__newthread = 0;
unsigned int COffsets::FUNC_CLua__getfield = 0;
unsigned int COffsets::FUNC_CLua__type_t = 0;
unsigned int COffsets::FUNC_CLua__typename_t;
unsigned int COffsets::FUNC_CLua__pushnil = 0;
unsigned int COffsets::FUNC_CLua__next = 0;
unsigned int COffsets::FUNC_CLua__pushvalue = 0;
unsigned int COffsets::FUNC_CLua__isuserdata = 0;
unsigned int COffsets::FUNC_CLua__touserdata = 0;

unsigned int COffsets::FUNC_CHumanInventory__ProcessShot = 0;
unsigned int COffsets::FUNC_CHumanInventory__DoReload = 0;
unsigned int COffsets::FUNC_CHuman__InventorySelect = 0;
unsigned int COffsets::FUNC_CHuman__InventoryAddWeapon = 0;
unsigned int COffsets::FUNC_CHuman__InventoryRemoveWeapon = 0;
unsigned int COffsets::FUNC_CHuman__InventoryTellAmmoInSelectedWeapon = 0;
unsigned int COffsets::FUNC_CHuman__ToggleStealthKill = 0;
unsigned int COffsets::FUNC_CHuman__EnablePushByPlayer = 0;
unsigned int COffsets::FUNC_CHuman__SetReactions = 0;
unsigned int COffsets::FUNC_CHuman__MoveVec = 0;
unsigned int COffsets::FUNC_CHuman__ShootAt = 0;
unsigned int COffsets::FUNC_CHuman__ShootAtEffect = 0;
unsigned int COffsets::FUNC_CHuman__AimAt = 0;
unsigned int COffsets::FUNC_CHuman__LookAt = 0;
unsigned int COffsets::FUNC_CHuman__ShowHat = 0;
unsigned int COffsets::FUNC_CHuman__DropHat = 0;
unsigned int COffsets::FUNC_CHuman__SetHealth = 0;
unsigned int COffsets::FUNC_CHuman__LockControls = 0;
unsigned int COffsets::FUNC_CHuman__AreControlsLocked = 0;
unsigned int COffsets::FUNC_CHuman__InventoryHaveItem = 0;
unsigned int COffsets::FUNC_CHuman__GetRealHealth = 0;
unsigned int COffsets::FUNC_CHuman__InventoryAddMoney = 0;
unsigned int COffsets::FUNC_CHuman__InventoryRemoveMoney = 0;
unsigned int COffsets::FUNC_CHuman__InventoryGetMoney = 0;
unsigned int COffsets::FUNC_CHuman__InventoryDoReload = 0;
unsigned int COffsets::FUNC_CHuman__SetControlStyle = 0;
unsigned int COffsets::FUNC_CHuman__GetControlStyle = 0;
unsigned int COffsets::FUNC_CHuman__HaveThrowingWeaponInHand = 0;
unsigned int COffsets::FUNC_CHuman__IsOnScreen = 0;
unsigned int COffsets::FUNC_CHuman__AnimationReset = 0;
unsigned int COffsets::FUNC_CHuman__GetInOutCar = 0;
unsigned int COffsets::FUNC_CHuman__TakeDamage = 0;
unsigned int COffsets::FUNC_CHuman__SetAnimStyle = 0;
unsigned int COffsets::FUNC_CHuman__SetModelToHand = 0;
unsigned int COffsets::FUNC_CHuman__SetModelToMouth = 0;
unsigned int COffsets::FUNC_CHuman__SetStealthMove = 0;
unsigned int COffsets::FUNC_CHuman__SetPhysState = 0;

unsigned int COffsets::VAR_VehiclePool = 0;
unsigned int COffsets::FUNC_CVehicle__OpenHood = 0;
unsigned int COffsets::FUNC_CVehicle__CloseHood = 0;
unsigned int COffsets::FUNC_CVehicle__OpenTrunk = 0;
unsigned int COffsets::FUNC_CVehicle__CloseTrunk = 0;
unsigned int COffsets::FUNC_CVehicle__SetDirtLevel = 0;
unsigned int COffsets::FUNC_CVehicle__SetEngineOn = 0;
unsigned int COffsets::FUNC_CVehicle__SetPainting = 0;
unsigned int COffsets::FUNC_CVehicle__SetActualTuningTable = 0;
unsigned int COffsets::FUNC_CVehicle__SetSPZText = 0;
unsigned int COffsets::FUNC_CVehicle__SetSpeedFloat = 0;
unsigned int COffsets::FUNC_CVehicle__SetBeaconLightOn = 0;
unsigned int COffsets::FUNC_CVehicle__SetHandbrake = 0;
unsigned int COffsets::FUNC_CVehicle__OpenSeatWindow = 0;
unsigned int COffsets::FUNC_CVehicle__IsWindowOpen = 0;
unsigned int COffsets::FUNC_CVehicle__Repair = 0;
unsigned int COffsets::FUNC_CVehicle__GetEngineDamage = 0;
unsigned int COffsets::FUNC_CVehicle__SetEngineDamage = 0;
unsigned int COffsets::FUNC_CVehicle__SetColors = 0;
unsigned int COffsets::FUNC_CVehicle__Explode = 0;
unsigned int COffsets::FUNC_CVehicle__SetSpeedLimited = 0;
unsigned int COffsets::FUNC_CVehicle__SetOnPart = 0;
unsigned int COffsets::FUNC_CVehicle__SetOffPart = 0;
unsigned int COffsets::FUNC_CVehicle__SetSpeedVec = 0;
unsigned int COffsets::FUNC_CVehicle__AddSteer = 0;
unsigned int COffsets::FUNC_CVehicle__SetPower = 0;
unsigned int COffsets::FUNC_CVehicle__SetBrake = 0;
unsigned int COffsets::FUNC_CVehicle__FixCarPos = 0;
unsigned int COffsets::FUNC_CVehicle__SetWheelMatrixAtIndex = 0;
unsigned int COffsets::FUNC_CVehicle__GetWheelsMaterial = 0;
unsigned int COffsets::FUNC_CVehicle__SetLightState = 0;
unsigned int COffsets::FUNC_CVehicle__PlayerStartEnter = 0;
unsigned int COffsets::FUNC_CVehicle__StopAllSounds = 0;
unsigned int COffsets::FUNC_CVehicle__SetVehicleColor = 0;
unsigned int COffsets::FUNC_CVehicle__GetVehicleColor = 0;
unsigned int COffsets::FUNC_CVehicle__SetDynamic = 0;
unsigned int COffsets::FUNC_CVehicle__AddVehicleFlags = 0;
unsigned int COffsets::FUNC_CVehicle__ClearVehicleFlags = 0;

unsigned int COffsets::FUNC_CVehicle__MarkForSale = 0;
unsigned int COffsets::FUNC_CVehicle__SetIndicatorLightOn = 0;
unsigned int COffsets::FUNC_CVehicle__SetTaxiLightOn = 0;
unsigned int COffsets::FUNC_CVehicle__OpenDoor = 0;
unsigned int COffsets::FUNC_CVehicle__SetDoorFree = 0;

unsigned int COffsets::FUNC_CCar__Spawn = 0;
unsigned int COffsets::FUNC_CCar__GetDamage = 0;
unsigned int COffsets::FUNC_CCar__SetCarDamage = 0;
unsigned int COffsets::FUNC_CCar__SetTransparency = 0;
unsigned int COffsets::FUNC_CCar__OpenDoors = 0;
unsigned int COffsets::FUNC_CCar__LockThrowFromCar = 0;
unsigned int COffsets::FUNC_CCar__SetGear = 0;
unsigned int COffsets::FUNC_CCar__SetGearBoxAutomat = 0;
unsigned int COffsets::FUNC_CCar__ResetRigidBody = 0;

unsigned int COffsets::FUNC_CCar__LockTrunks = 0;
unsigned int COffsets::FUNC_CCar__UnlockTrunks = 0;

unsigned int COffsets::FUNC_CSyncObject__Reactivate = 0;

unsigned int COffsets::VAR_CHud = 0;
unsigned int COffsets::VAR_CHudFader = 0;
unsigned int COffsets::FUNC_CHud__FaderFadeIn = 0;
unsigned int COffsets::FUNC_CHud__FaderFadeOut = 0;
unsigned int COffsets::FUNC_CHud__Toggle = 0;
unsigned int COffsets::FUNC_CHudComponent__Show = 0;
unsigned int COffsets::FUNC_CHud__ScoreShow = 0;
unsigned int COffsets::FUNC_CHud__RadarShow = 0;
unsigned int COffsets::FUNC_CHud__SubtitlesShow = 0;
unsigned int COffsets::FUNC_CHud__SpeedoShow = 0;

unsigned int COffsets::VAR_CMap = 0;
unsigned int COffsets::FUNC_CMap__CreateEntityIcon = 0;
unsigned int COffsets::FUNC_CMap__DeleteEntityIcon = 0;

unsigned int COffsets::VAR_CNavigation = 0;
unsigned int COffsets::FUNC_CNavigation__RegisterIconEntity = 0;
unsigned int COffsets::FUNC_CNavigation__RegisterIconPos = 0;
unsigned int COffsets::FUNC_CNavigation__UnregisterIconPos = 0;
unsigned int COffsets::FUNC_CNavigation__UnregisterIconEntity = 0;
unsigned int COffsets::FUNC_CNavigation__GetIconFromId = 0;
unsigned int COffsets::FUNC_CNavigation__GetIconFromEntity = 0;

unsigned int COffsets::VAR_CPhysFS;

unsigned int COffsets::FUNC_CEntity__Create = 0;
unsigned int COffsets::FUNC_CEntity__Activate = 0;
unsigned int COffsets::FUNC_CEntity__Deactivate = 0;
unsigned int COffsets::FUNC_CEntity__Delete = 0;

// Video settings
unsigned int COffsets::FUNC_C_SystemRenderingModule__SetFullScreenEnabled = 0;
unsigned int COffsets::FUNC_C_SystemRenderingModule__SetVerticalSyncEnabled = 0;
unsigned int COffsets::FUNC_SetScreenResolution = 0;
unsigned int COffsets::FUNC_SetMultisampleAntiAliasing = 0;
unsigned int COffsets::FUNC_SetAnisotropicFiltering = 0;
unsigned int COffsets::FUNC_EnableAmbientOcclusion = 0;

// Audio settings
unsigned int COffsets::FUNC_SetAudioQuality = 0;
unsigned int COffsets::FUNC_SetSFXVolume = 0;
unsigned int COffsets::FUNC_SetVoicesVolume = 0;
unsigned int COffsets::FUNC_SetMusicVolume = 0;
unsigned int COffsets::FUNC_SetRadioVolume = 0;

// Player
unsigned int COffsets::FUNC_CPlayer_ProcessKeyboard = 0;

// Text database
unsigned int COffsets::VAR_CTextDatabase = 0;
unsigned int COffsets::FUNC_CTextDatabase__Clear = 0;
unsigned int COffsets::FUNC_CTextDatabase__Done = 0;
unsigned int COffsets::FUNC_CTextDatabase__GetString = 0;
unsigned int COffsets::FUNC_CTextDatabase__Init = 0;
unsigned int COffsets::FUNC_CTextDatabase__LoadIconsMap = 0;
unsigned int COffsets::FUNC_CTextDatabase__ResetHistory = 0;

void COffsets::Initialise(BYTE byteVersion, unsigned int uiBase)
{
	switch (byteVersion)
	{
	case 0: // temp - v1 cracked
	{
		// Initialise pointers
		VAR_CGame = (uiBase + 0x1AACD64);
		VAR_CGame__ScriptEngine = (uiBase + 0x1AADBC8);
		VAR_CCamera = (uiBase + 0x1AA99FC);
		VAR_CWindow = (uiBase + 0x1AACD80);
		VAR_CWrapperList = (uiBase + 0x1BAE088);
		VAR_VehiclePool = (uiBase + 0x1AA29C0);
		VAR_CSDSManager = (uiBase + 0x1AACD9C);
		VAR_LocalPlayer = (uiBase + 0x1AACDAC);
		VAR_CTextDatabase = (uiBase + 0x1AADC18);

		// 0x1AACD70 = VAR_CMiniMap

		break;
	}

	// Steam - unsupported
	case 1: // temp - steam
	{
		// Initialise pointers
		VAR_CGame = (uiBase + 0x1ABFE14);
		VAR_CGame__ScriptEngine = (uiBase + 0x1AC0C78);
		VAR_CCamera = (uiBase + 0x1ABCAAC);
		VAR_CWindow = (uiBase + 0x1ABFE30);
		VAR_CWrapperList = (uiBase + 0x1CC14A8);
		VAR_VehiclePool = (uiBase + 0x0);
		VAR_CSDSManager = (uiBase + 0x0);
		VAR_LocalPlayer = (uiBase + 0x1ABFE5C);

		break;
	}
	}

	// Initialise common addresses
	byteVersion == 0 ? CommonInitialise(uiBase) : CommonInitialise__Steam(uiBase);
}

void COffsets::CommonInitialise(unsigned int uiBase)
{
	// CIE
	FUNC_CIE_CreateObject	=	(0x0115F840);
	FUNC_CIE_Assign_Model	=	(0x014E2970);
	FUNC_CIE_SetHashName	=	(0x014AEA40);
	FUNC_CIE_SetFlag		=	(0x014AEAC0);
	FUNC_CIE_SetModel		=	(0x0115CB10);
	FUNC_CIE_UnkFunc		=	(0x0098DA20);
	FUNC_CIE_GetTrafficHuman	=	(0x00A2FD60);
	FUNC_CIE_SetTrafficHuman	=	(0x0093E6A0);
	FUNC_CIE_GetHashedString	=	(0x180F3E0);
	FUNC_CIE_LoadModel		=	(0x124F2F0);
	FUNC_CIE_CCarSetup		=	(0x00A04B40);
	FUNC_CIE_SetCarParams	=	(0x009906B0);

	// Game
	FUNC_CGame__OnGameInit = (uiBase + 0x410440); // Steam: 0x4105F0
	FUNC_CGame__OnGameEvent = (uiBase + 0x1173A00); // Steam: 0x117BCA0
	FUNC_CGame__OnGameLoad = (uiBase + 0x587A20); // Steam: 0x0597C10
	FUNC_CGame__Allocate = (uiBase + 0x401730); // Steam: 0x401830
	FUNC_CGame__Free = (uiBase + 0x4018F0);
	FUNC_CGame__AllocateBuffer = (uiBase + 0x4016F0);

	// Misc
	VAR_GuiMainSdsFolder = (uiBase + 0x18E1D04); // Steam : 0x18F5084
	VAR_GuiMainSds = (uiBase + 0x18E4DA0); // Steam : 0x18F8958
	VAR_GuiSds = (uiBase + 0x18E4D30); // Steam : 0x18F88E8
	VAR_TablesSds = (uiBase + 0x18E4CFC); // Steam : 0x18F88B4
	VAR_LoadingScreen = (uiBase + 0x1949B58); // Steam : 0x195D7A0
	VAR_StreamBin = (uiBase + 0x18E3A44); // Steam : 0x18F7364
	VAR_SDSConfigFile = (uiBase + 0x18F76A4);
	FUNC_M2malloc = (uiBase + 0x401730);
	FUNC_OnGameProcessStart = (uiBase + 0x401A80);

	// Core
	VAR_CCore = (uiBase + 0x1AAF6C8);
	FUNC_CCore__AllocateModel = (uiBase + 0x14DFBB0);

	// Door
	FUNC_CDoor__Open = (uiBase + 0x4DBBA0);
	FUNC_CDoor__Kick = (uiBase + 0x4E5600);
	FUNC_CDoor__Close = (uiBase + 0x4DBC50);
	FUNC_CDoor__Lock = (uiBase + 0x4E1700);
	FUNC_CDoor__Unlock = (uiBase + 0x4E1730);
	FUNC_CDoor__EnableAction = (uiBase + 0x449FF0);
	FUNC_CDoor__DisableAction = (uiBase + 0x44A060);

	// Model Manager
	FUNC_CModelMgr__Load = (uiBase + 0x5BB590);
	FUNC_CModelMgr__Free = (uiBase + 0x59F0F0);
	FUNC_CModelMgr__Construct = (uiBase + 0x5A64E0);
	FUNC_CModelMgr__ChangeModel = (uiBase + 0x5BB7E0);

	// Slot Manager
	VAR_CSlotManager = (uiBase + 0x1AACD8C);

	// Obj Manager
	VAR_CObjectTable = (uiBase + 0x1AACD70);

	// Hash
	FUNC_fnv64 = (uiBase + 0x41C660);
	FUNC_fnv32 = (uiBase + 0x41C6B0);

	// SDS
	FUNC_CSDSManager__ActivateStreamMapLine = (uiBase + 0x5BF630);

	// Entity manager
	FUNC_CEntMgr__ProcessEntities = (uiBase + 0x973B90); // Steam: 0x985460

	// Wrapper lists
	FUNC_CWrapperList__GetEntityByName = (uiBase + 0xD8E3B0); // Steam: 0xD955A0

	// Camera
	FUNC_CCamera__GetGameCameraFromIndex = (uiBase + 0x1079D40); // Steam: 0x10822C0
	VAR_CCamera__State = (uiBase + 0x1BAF07C);

	// Lua
	FUNC_CLua__pcall = (uiBase + 0x5C3870); // Steam: 0x5D3F80
	FUNC_CLua__loadbuffer = (uiBase + 0x5C54C0); // Steam: 0x5D5BD0
	FUNC_CLua__settop = (uiBase + 0x5C2110); // Steam: 0x5D2820
	FUNC_CLua__gettop = (uiBase + 0x5C20F0); // Steam: 0x5D2800
	FUNC_CLua__getfield = (uiBase + 0x005C3070);
	FUNC_CLua__type_t = (uiBase + 0x005C2530);
	FUNC_CLua__typename_t = (uiBase + 0x005C2570);
	FUNC_CLua__pushnil = (uiBase + 0x005C2C10);
	FUNC_CLua__next = (uiBase + 0x005C3C60);
	FUNC_CLua__pushvalue = (uiBase + 0x005C24E0);
	FUNC_CLua__isuserdata = (uiBase + 0x005C2690);
	FUNC_CLua__touserdata = (uiBase + 0x005C2AF0);

	// Don't need these?
	FUNC_CLua__setfield = (uiBase + CPatcher::FindPattern((BYTE *)"\x55\x8B\xEC\x83\xEC\x20\x8B\x45\x0C\x50\x8B\x4D\x08\x51\xE8\xCD", "xxxxxxxxxxxxxxxx"));
	FUNC_CLua__tolstring = (uiBase + CPatcher::FindPattern((BYTE *)"\x55\x8B\xEC\x51\x8B\x45\x0C\x50\x8B\x4D\x08\x51\xE8\x8F\xF8\xFF", "xxxxxxxxxxxxxxxx"));
	FUNC_CLua__pushcclosure = (uiBase + CPatcher::FindPattern((BYTE *)"\x55\x8B\xEC\x83\xEC\x10\x8B\x45\x08\x8B\x48\x10\x8B\x55\x08\x8B", "xxxxxxxxxxxxxxxx"));
	FUNC_CLua__tointeger = (uiBase + CPatcher::FindPattern((BYTE *)"\x55\x8B\xEC\x83\xEC\x14\x8B\x45\x0C\x50\x8B\x4D\x08\x51\xE8\x3D", "xxxxxxxxxxxxxxxx"));
	FUNC_CLua__newthread = (uiBase + CPatcher::FindPattern((BYTE *)"\x55\x8B\xEC\x83\xEC\x08\x8B\x45\x08\x8B\x48\x10\x8B\x55\x08\x8B", "xxxxxxxxxxxxxxxx"));

	// Human
	FUNC_CHumanInventory__ProcessShot = (uiBase + 0x977FE0); // Steam: 0x0977FE0
	FUNC_CHumanInventory__DoReload = (uiBase + 0x958140); // Steam: 0x0969510
	FUNC_CHuman__ShowHat = (uiBase + 0x90C880); // Steam: 0x91DCC0
	FUNC_CHuman__DropHat = (uiBase + 0x92A450); // Steam: 0x93B890
	FUNC_CHuman__SetHealth = (uiBase + 0x92A480); // Steam: 0x93B8C0
	FUNC_CHuman__SetReactions = (uiBase + 0x90CEA0); // Steam: 0x91E2E0
	FUNC_CHuman__InventorySelect = (uiBase + 0x983A30); // Steam: 0x9953A0
	FUNC_CHuman__InventoryAddWeapon = (uiBase + 0x949C50); // Steam: 0x95B030
	FUNC_CHuman__InventoryRemoveWeapon = (uiBase + 0x925960); // Steam: 0x936D90
	FUNC_CHuman__InventoryTellAmmoInSelectedWeapon = (uiBase + 0x0931950);
	FUNC_CHuman__ToggleStealthKill = (uiBase + 0x92A640); // Steam: 0x93BA80
	FUNC_CHuman__EnablePushByPlayer = (uiBase + 0x92A6C0); // Steam: 0x93BB00
	FUNC_CHuman__MoveVec = (uiBase + 0x97F740); // Steam: 0x9910B0
	FUNC_CHuman__ShootAt = (uiBase + 0x09822C0); // Steam: 0x993C30
	FUNC_CHuman__ShootAtEffect = (uiBase + 0x982420); // Steam: 0x993C30
	FUNC_CHuman__AimAt = (uiBase + 0x982190); // Steam: 0x993B00
	FUNC_CHuman__LookAt = (uiBase + 0x981860); // Steam: 0x9931D0
	FUNC_CHuman__LockControls = (uiBase + 0x94B9C0); // Steam: 0x095CD80
	FUNC_CHuman__AreControlsLocked = (uiBase + 0x90CA00); // Steam: 0x91DE40
	FUNC_CHuman__InventoryHaveItem = (uiBase + 0x9D3000); // Steam: 0x9E3280
	FUNC_CHuman__GetRealHealth = (uiBase + 0x90CAF0);
	FUNC_CHuman__InventoryAddMoney = (uiBase + 0x949C80); // Steam: 0x95B060
	FUNC_CHuman__InventoryRemoveMoney = (uiBase + 0x93BE60); // Steam: 0x94D250
	FUNC_CHuman__InventoryGetMoney = (uiBase + 0x931B80); // Steam: 0x942FB0
	FUNC_CHuman__InventoryDoReload = (uiBase + 0x958140); // Steam: 0x969510
	FUNC_CHuman__SetControlStyle = (uiBase + 0x94B9C0);
	FUNC_CHuman__GetControlStyle = (uiBase + 0x90CA00); // Steam: 0x91DE40 (SAME AS CHuman__AreControlsLocked)
	FUNC_CHuman__HaveThrowingWeaponInHand = (uiBase + 0x420040); // Steam: 0x420230
	FUNC_CHuman__IsOnScreen = (uiBase + 0xD74FE0); // Steam: 0xD7C150
	FUNC_CHuman__AnimationReset = (uiBase + 0x98B960); // Steam: 0x99D340
	FUNC_CHuman__GetInOutCar = (uiBase + 0x98C800); // Steam: 0x99E1E0
	FUNC_CHuman__TakeDamage = (uiBase + 0x97EE60);
	FUNC_CHuman__SetAnimStyle = (uiBase + 0x956720);
	FUNC_CHuman__SetModelToHand = (uiBase + 0x90C860);
	FUNC_CHuman__SetModelToMouth = (uiBase + 0x042D940);
	FUNC_CHuman__SetStealthMove = (uiBase + 0x090CCE0);
	FUNC_CHuman__SetPhysState = (uiBase + 0x092A460);

	// Vehicle
	FUNC_CVehicle__OpenHood = (uiBase + 0x9AC050); // Steam: 0x09BCFB0
	FUNC_CVehicle__CloseHood = (uiBase + 0x9AC0D0); // Steam: 0x09BD030
	FUNC_CVehicle__OpenTrunk = (uiBase + 0x9ABF50); // Steam: 0x09BCEB0
	FUNC_CVehicle__CloseTrunk = (uiBase + 0x9ABFD0); // Steam: 0x09BCF30
	FUNC_CVehicle__SetDirtLevel = (uiBase + 0x9BC1B0);
	FUNC_CVehicle__SetEngineOn = (uiBase + 0x1259FB0);
	FUNC_CVehicle__SetPainting = (uiBase + 0x446B70); // Steam: 0x04472A0
	FUNC_CVehicle__SetActualTuningTable = (uiBase + 0x9CA0D0); // Steam: 0x09DA440
	FUNC_CVehicle__SetSPZText = (uiBase + 0x11EE750);
	FUNC_CVehicle__SetSpeedFloat = (uiBase + 0x98E7F0);
	FUNC_CVehicle__SetBeaconLightOn = (uiBase + 0x1203DD0);
	FUNC_CVehicle__SetHandbrake = (uiBase + 0x11FB6D0);
	FUNC_CVehicle__OpenSeatWindow = (uiBase + 0x990AA0);
	FUNC_CVehicle__IsWindowOpen = (uiBase + 0x990AF0);
	FUNC_CVehicle__Explode = (uiBase + 0xA032A0);
	FUNC_CVehicle__Repair = (uiBase + 0x4DA7B0); // Steam: 0x04E9890
	FUNC_CVehicle__SetEngineDamage = (uiBase + 0x9A9E50);
	FUNC_CVehicle__GetEngineDamage = (uiBase + 0x9A9DC0); // Steam: 0x09BAD20
	FUNC_CVehicle__SetSpeedLimited = (uiBase + 0x467BB0); // Steam: 0x046B7D0
	FUNC_CVehicle__SetOnPart = (uiBase + 0x125A600);
	FUNC_CVehicle__SetOffPart = (uiBase + 0x125A530);
	FUNC_CVehicle__SetColors = (uiBase + 0x9AB010);
	FUNC_CVehicle__SetSpeedVec = (uiBase + 0x120E910);
	FUNC_CVehicle__AddSteer = (uiBase + 0x11FB4E0);
	FUNC_CVehicle__SetPower = (uiBase + 0x120E720);
	FUNC_CVehicle__SetBrake = (uiBase + 0x120E7D0);
	FUNC_CVehicle__FixCarPos = (uiBase + 0x480580);
	FUNC_CVehicle__SetWheelMatrixAtIndex = (uiBase + 0x9BE450);
	FUNC_CVehicle__GetWheelsMaterial = (uiBase + 0x990CF0);
	FUNC_CVehicle__SetLightState = (uiBase + 0x11EED00);
	FUNC_CVehicle__PlayerStartEnter = (uiBase + 0x953B20);

	FUNC_CVehicle__MarkForSale = (uiBase + 0x0D649E0);
	FUNC_CVehicle__SetIndicatorLightOn = (uiBase + 0x1203D80);
	FUNC_CVehicle__SetTaxiLightOn = (uiBase + 0x1203DF0);
	FUNC_CVehicle__OpenDoor = (uiBase + 0x1258050);
	FUNC_CVehicle__SetDoorFree = (uiBase + 0x1258630);
	FUNC_CVehicle__SetVehicleColor = (uiBase + 0x11EE9A0);
	FUNC_CVehicle__GetVehicleColor = (uiBase + 0x11EEA00);
	FUNC_CVehicle__SetDynamic = (uiBase + 0x0120DBB0);
	FUNC_CVehicle__StopAllSounds = (uiBase + 0x0122B690);
	FUNC_CVehicle__AddVehicleFlags = (uiBase + 0x01259F40);
	FUNC_CVehicle__ClearVehicleFlags = (uiBase + 0x0120E340);

	FUNC_CCar__Spawn = (uiBase + 0x447820);
	FUNC_CCar__GetDamage = (uiBase + 0x09A9A70);
	FUNC_CCar__SetCarDamage = (uiBase + 0x1264790);
	FUNC_CCar__SetTransparency = (uiBase + 0x09BC2F0);
	FUNC_CCar__OpenDoors = (uiBase + 0x09AAD10);
	FUNC_CCar__LockThrowFromCar = (uiBase + 0x990BA0);
	FUNC_CCar__LockTrunks = (uiBase + 0x0D63EB0);
	FUNC_CCar__UnlockTrunks = (uiBase + 0x09BE550);
	FUNC_CCar__SetGear = (uiBase + 0x11EA5D0);
	FUNC_CCar__SetGearBoxAutomat = (uiBase + 0x125A180);
	FUNC_CCar__ResetRigidBody = (uiBase + 0x00468EB0);

	// Sync Objects
	FUNC_CSyncObject__Reactivate = (uiBase + 0x11DE7D0);

	// Hud
	VAR_CHud = (uiBase + 0x1BA71F8);
	VAR_CHudFader = (uiBase + 0x1AA3448);
	FUNC_CHud__FaderFadeIn = (uiBase + 0x87FFA0);
	FUNC_CHud__FaderFadeOut = (uiBase + 0x87FDF0);
	FUNC_CHud__Toggle = (uiBase + 0x893B30);
	FUNC_CHudComponent__Show = (uiBase + 0x87A0B0);
	FUNC_CHud__ScoreShow = (uiBase + 0x87B760);
	FUNC_CHud__RadarShow = (uiBase + 0x8823F0);
	FUNC_CHud__SubtitlesShow = (uiBase + 0x8824B0);
	FUNC_CHud__SpeedoShow = (uiBase + 0x8818D0);

	// Map (Unused)
	VAR_CMap = (uiBase + 0x1AACD70);
	FUNC_CMap__CreateEntityIcon = (uiBase + 0x83B420);
	FUNC_CMap__DeleteEntityIcon = (uiBase + 0x83B540);

	// Navigation
	VAR_CNavigation = (uiBase + 0x1AA3398);
	FUNC_CNavigation__RegisterIconEntity = (uiBase + 0x4D82E0);
	FUNC_CNavigation__RegisterIconPos = (uiBase + 0x4D8270);
	FUNC_CNavigation__UnregisterIconPos = (uiBase + 0x4C97C0);
	FUNC_CNavigation__UnregisterIconEntity = (uiBase + 0x4C98F0);
	FUNC_CNavigation__GetIconFromId = (uiBase + 0x4963E0);
	FUNC_CNavigation__GetIconFromEntity = (uiBase + 0x4965C0);

	// PhysFS
	VAR_CPhysFS = (uiBase + 0x1AC94F0);

	// Entity
	FUNC_CEntity__Create = (uiBase + 0x992E50);
	FUNC_CEntity__Activate = (uiBase + 0x1189E20);
	FUNC_CEntity__Deactivate = (uiBase + 0x1189E70);
	FUNC_CEntity__Delete = (uiBase + 0x1195580);

	// Video settings
	FUNC_C_SystemRenderingModule__SetFullScreenEnabled = 0xB65CB0;
	FUNC_C_SystemRenderingModule__SetVerticalSyncEnabled = 0xB65F80;
	FUNC_SetScreenResolution = 0x8366A0;
	FUNC_SetMultisampleAntiAliasing = 0x836820;
	FUNC_SetAnisotropicFiltering = 0x8368B0;
	FUNC_EnableAmbientOcclusion = 0x8366F0;

	// Audio settings
	FUNC_SetAudioQuality = 0x836540;
	FUNC_SetSFXVolume = 0x8363F0;
	FUNC_SetVoicesVolume = 0x836450;
	FUNC_SetMusicVolume = 0x8364B0;
	FUNC_SetRadioVolume = 0x836510;

	// Player
	FUNC_CPlayer_ProcessKeyboard = 0x42A9E0;

	// TextDatabase
	FUNC_CTextDatabase__Clear = (uiBase + 0x11E5700);
	FUNC_CTextDatabase__Done = (uiBase + 0x11E7950);
	FUNC_CTextDatabase__GetString = (uiBase + 0x11E6AD0);
	FUNC_CTextDatabase__Init = (uiBase + 0x11E0F10);
	FUNC_CTextDatabase__LoadIconsMap = (uiBase + 0x11E6950);
	FUNC_CTextDatabase__ResetHistory = (uiBase + 0x11E7310);
}

void COffsets::CommonInitialise__Steam(unsigned int uiBase)
{
	// Game
	FUNC_CGame__OnGameInit = (uiBase + 0x4105F0);
	FUNC_CGame__OnGameEvent = (uiBase + 0x117BCA0);
	FUNC_CGame__OnGameLoad = (uiBase + 0x0597C10);
	FUNC_CGame__Allocate = (uiBase + 0x401830);
	FUNC_CGame__Free = (uiBase + 0x4019F0);
	FUNC_CGame__AllocateBuffer = (uiBase + 0x4017F0);

	// Misc
	VAR_GuiMainSdsFolder = (uiBase + 0x18F5084);
	VAR_GuiMainSds = (uiBase + 0x18F8958);
	VAR_GuiSds = (uiBase + 0x18F88E8);
	VAR_TablesSds = (uiBase + 0x18F88B4);
	VAR_LoadingScreen = (uiBase + 0x195D7A0);
	VAR_StreamBin = (uiBase + 0x18F7364);
	VAR_SDSConfigFile = (uiBase + 0x190B2A4);
	FUNC_M2malloc = (uiBase + 0x401830);
	FUNC_OnGameProcessStart = (uiBase + 0x401B80);

	// Core
	VAR_CCore = (uiBase + 0x1AC277);
	FUNC_CCore__AllocateModel = (uiBase + 0x14EBFB0);

	// Door
	/*FUNC_CDoor__Open = (uiBase + 0x4DBBA0);
	FUNC_CDoor__Kick = (uiBase + 0x4E5600);
	FUNC_CDoor__Close = (uiBase + 0x4DBC50);
	FUNC_CDoor__Lock = (uiBase + 0x4E1700);
	FUNC_CDoor__Unlock = (uiBase + 0x4E1730);
	FUNC_CDoor__EnableAction = (uiBase + 0x449FF0);
	FUNC_CDoor__DisableAction = (uiBase + 0x44A060);*/

	// Model Manager
	FUNC_CModelMgr__Load = (uiBase + 0x5CBBE0);
	FUNC_CModelMgr__Free = (uiBase + 0x5AF310);
	FUNC_CModelMgr__Construct = (uiBase + 0x5B6680);
	FUNC_CModelMgr__ChangeModel = (uiBase + 0x5CBE30);

	// Slot Manager
	VAR_CSlotManager = (uiBase + 0x1ABFE3C);

	// Obj Manager
	VAR_CObjectTable = (uiBase + 0x1ABFE20);

	// Hash
	FUNC_fnv64 = (uiBase + 0x41C830);
	FUNC_fnv32 = (uiBase + 0x41C880);

	// SDS
	FUNC_CSDSManager__ActivateStreamMapLine = (uiBase + 0x5CFCD0);

	// Entity manager
	FUNC_CEntMgr__ProcessEntities = (uiBase + 0x985460);

	// Wrapper lists
	FUNC_CWrapperList__GetEntityByName = (uiBase + 0xD955A0);

	// Camera
	FUNC_CCamera__GetGameCameraFromIndex = (uiBase + 0x10822C0);

	// Lua
	FUNC_CLua__pcall = (uiBase + 0x5D3F80);
	FUNC_CLua__loadbuffer = (uiBase + 0x5D5BD0);
	FUNC_CLua__settop = (uiBase + 0x5D2820);
	FUNC_CLua__gettop = (uiBase + 0x5D2800);

	// Don't need these?
	FUNC_CLua__setfield = (uiBase + CPatcher::FindPattern((BYTE *)"\x55\x8B\xEC\x83\xEC\x20\x8B\x45\x0C\x50\x8B\x4D\x08\x51\xE8\xCD", "xxxxxxxxxxxxxxxx"));
	FUNC_CLua__tolstring = (uiBase + CPatcher::FindPattern((BYTE *)"\x55\x8B\xEC\x51\x8B\x45\x0C\x50\x8B\x4D\x08\x51\xE8\x8F\xF8\xFF", "xxxxxxxxxxxxxxxx"));
	FUNC_CLua__pushcclosure = (uiBase + CPatcher::FindPattern((BYTE *)"\x55\x8B\xEC\x83\xEC\x10\x8B\x45\x08\x8B\x48\x10\x8B\x55\x08\x8B", "xxxxxxxxxxxxxxxx"));
	FUNC_CLua__tointeger = (uiBase + CPatcher::FindPattern((BYTE *)"\x55\x8B\xEC\x83\xEC\x14\x8B\x45\x0C\x50\x8B\x4D\x08\x51\xE8\x3D", "xxxxxxxxxxxxxxxx"));
	FUNC_CLua__newthread = (uiBase + CPatcher::FindPattern((BYTE *)"\x55\x8B\xEC\x83\xEC\x08\x8B\x45\x08\x8B\x48\x10\x8B\x55\x08\x8B", "xxxxxxxxxxxxxxxx"));

	// Human
	FUNC_CHumanInventory__ProcessShot = (uiBase + 0x0977FE0);
	FUNC_CHumanInventory__DoReload = (uiBase + 0x0969510);
	FUNC_CHuman__ShowHat = (uiBase + 0x91DCC0);
	FUNC_CHuman__DropHat = (uiBase + 0x93B890);
	FUNC_CHuman__SetHealth = (uiBase + 0x93B8C0);
	FUNC_CHuman__SetReactions = (uiBase + 0x91E2E0);
	FUNC_CHuman__InventorySelect = (uiBase + 0x9953A0);
	FUNC_CHuman__InventoryAddWeapon = (uiBase + 0x95B030);
	FUNC_CHuman__InventoryRemoveWeapon = (uiBase + 0x936D90);
	FUNC_CHuman__ToggleStealthKill = (uiBase + 0x93BA80);
	FUNC_CHuman__EnablePushByPlayer = (uiBase + 0x93BB00);
	FUNC_CHuman__MoveVec = (uiBase + 0x9910B0);
	FUNC_CHuman__ShootAt = (uiBase + 0x993C30);
	FUNC_CHuman__AimAt = (uiBase + 0x993B00);
	FUNC_CHuman__LookAt = (uiBase + 0x9931D0);
	FUNC_CHuman__LockControls = (uiBase + 0x095CD80);
	FUNC_CHuman__AreControlsLocked = (uiBase + 0x91DE40);
	FUNC_CHuman__InventoryHaveItem = (uiBase + 0x9E3280);
	FUNC_CHuman__GetRealHealth = (uiBase + 0x91DF30);
	FUNC_CHuman__InventoryAddMoney = (uiBase + 0x95B060);
	FUNC_CHuman__InventoryRemoveMoney = (uiBase + 0x94D250);
	FUNC_CHuman__InventoryGetMoney = (uiBase + 0x942FB0);
	FUNC_CHuman__InventoryDoReload = (uiBase + 0x969510);
	//FUNC_CHuman__SetControlStyle = (uiBase + 0x94B9C0);
	FUNC_CHuman__GetControlStyle = (uiBase + 0x91DE40); // (SAME AS CHuman__AreControlsLocked)
	FUNC_CHuman__HaveThrowingWeaponInHand = (uiBase + 0x420230);
	FUNC_CHuman__IsOnScreen = (uiBase + 0xD7C150);
	FUNC_CHuman__AnimationReset = (uiBase + 0x99D340);
	FUNC_CHuman__GetInOutCar = (uiBase + 0x99E1E0);
	FUNC_CHuman__TakeDamage = (uiBase + 0x09907D0);

	// Vehicle
	FUNC_CVehicle__OpenHood = (uiBase + 0x09BCFB0);
	FUNC_CVehicle__CloseHood = (uiBase + 0x09BD030);
	FUNC_CVehicle__OpenTrunk = (uiBase + 0x09BCEB0);
	FUNC_CVehicle__CloseTrunk = (uiBase + 0x09BCF30);
	FUNC_CVehicle__SetDirtLevel = (uiBase + 0x9CCB80);
	FUNC_CVehicle__SetEngineOn = (uiBase + 0x1263850);
	FUNC_CVehicle__SetPainting = (uiBase + 0x04472A0);
	FUNC_CVehicle__SetActualTuningTable = (uiBase + 0x09DA440);
	FUNC_CVehicle__SetSPZText = (uiBase + 0x11F75D0);
	FUNC_CVehicle__SetSpeedFloat = (uiBase + 0x9A01A0);
	FUNC_CVehicle__SetBeaconLightOn = (uiBase + 0x120CC10);
	FUNC_CVehicle__SetHandbrake = (uiBase + 0x1204540);
	FUNC_CVehicle__OpenSeatWindow = (uiBase + 0x9A2230);
	FUNC_CVehicle__Explode = (uiBase + 0xA13950);
	FUNC_CVehicle__Repair = (uiBase + 0x04E9890);
	//FUNC_CVehicle__SetEngineDamage = (uiBase + 0x9A9E50);
	FUNC_CVehicle__GetEngineDamage = (uiBase + 0x09BAD20);
	FUNC_CVehicle__SetSpeedLimited = (uiBase + 0x046B7D0);
	/*FUNC_CVehicle__SetOnPart = (uiBase + 0x125A600);
	FUNC_CVehicle__SetOffPart = (uiBase + 0x125A530);*/
	FUNC_CVehicle__SetColors = (uiBase + 0x9BBF70);
	FUNC_CVehicle__PlayerStartEnter = (uiBase + 0x0964EF0);

	// Sync Objects
	FUNC_CSyncObject__Reactivate = (uiBase + 0x11E7590);

	// Hud
	VAR_CHud = (uiBase + 0x1CBA618);
	VAR_CHudFader = (uiBase + 0x1AB64F0);
	FUNC_CHud__FaderFadeIn = (uiBase + 0x08910C0);
	FUNC_CHud__FaderFadeOut = (uiBase + 0x890F10);
	FUNC_CHud__Toggle = (uiBase + 0x8A4CE0);
	FUNC_CHudComponent__Show = (uiBase + 0x88B1D0);
	FUNC_CHud__ScoreShow = (uiBase + 0x88C880);
	FUNC_CHud__RadarShow = (uiBase + 0x893510);
	FUNC_CHud__SubtitlesShow = (uiBase + 0x8935D0);

	// Map (Unused)
	/*VAR_CMap = (uiBase + 0x1AACD70);
	FUNC_CMap__CreateEntityIcon = (uiBase + 0x83B420);
	FUNC_CMap__DeleteEntityIcon = (uiBase + 0x83B540);*/

	// Navigation
	VAR_CNavigation = (uiBase + 0x1AB6440);
	FUNC_CNavigation__RegisterIconEntity = (uiBase + 0x4E7350);
	FUNC_CNavigation__RegisterIconPos = (uiBase + 0x4E72E0);
	FUNC_CNavigation__UnregisterIconPos = (uiBase + 0x4D7770);
	FUNC_CNavigation__UnregisterIconEntity = (uiBase + 0x4D78A0);

	// PhysFS
	VAR_CPhysFS = (uiBase + 0x1BDC918);

	// Entity
	FUNC_CEntity__Activate = (uiBase + 0x1192170);
	FUNC_CEntity__Deactivate = (uiBase + 0x11921C0);
	FUNC_CEntity__Delete = (uiBase + 0x119DDD0);

	// Player
	FUNC_CPlayer_ProcessKeyboard = 0x42ABE0;
}