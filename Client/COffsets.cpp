/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : COffsets.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

unsigned int COffsets::VAR_CGame = 0;
unsigned int COffsets::FUNC_CGame__OnGameInit = 0;
unsigned int COffsets::FUNC_CGame__OnGameEvent = 0;
unsigned int COffsets::VAR_CGame__ScriptEngine = 0;
unsigned int COffsets::FUNC_CGame__Allocate = 0;
unsigned int COffsets::FUNC_CGame__Free = 0;
unsigned int COffsets::FUNC_CGame__AllocateBuffer = 0;

unsigned int COffsets::VAR_LocalPlayer = 0;
unsigned int COffsets::VAR_GuiMainSdsFolder = 0;
unsigned int COffsets::VAR_GuiMainSds = 0;
unsigned int COffsets::VAR_GuiSds = 0;
unsigned int COffsets::VAR_TablesSds = 0;
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

unsigned int COffsets::FUNC_CHumanInventory__ProcessShot = 0;
unsigned int COffsets::FUNC_CHuman__InventorySelect = 0;
unsigned int COffsets::FUNC_CHuman__InventoryAddWeapon = 0;
unsigned int COffsets::FUNC_CHuman__InventoryRemoveWeapon = 0;
unsigned int COffsets::FUNC_CHuman__ToggleStealthKill = 0;
unsigned int COffsets::FUNC_CHuman__EnablePushByPlayer = 0;
unsigned int COffsets::FUNC_CHuman__SetReactions = 0;
unsigned int COffsets::FUNC_CHuman__MoveVec = 0;
unsigned int COffsets::FUNC_CHuman__ShootAt = 0;
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
unsigned int COffsets::FUNC_CVehicle__Repair = 0;
unsigned int COffsets::FUNC_CVehicle__GetEngineDamage = 0;
unsigned int COffsets::FUNC_CVehicle__SetEngineDamage = 0;
unsigned int COffsets::FUNC_CVehicle__SetColors = 0;
unsigned int COffsets::FUNC_CVehicle__Explode = 0;
unsigned int COffsets::FUNC_CVehicle__SetSpeedLimited = 0;
unsigned int COffsets::FUNC_CVehicle__SetOnPart = 0;
unsigned int COffsets::FUNC_CVehicle__SetOffPart = 0;

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

unsigned int COffsets::VAR_CMap = 0;
unsigned int COffsets::FUNC_CMap__CreateEntityIcon = 0;
unsigned int COffsets::FUNC_CMap__DeleteEntityIcon = 0;

unsigned int COffsets::VAR_CNavigation = 0;
unsigned int COffsets::FUNC_CNavigation__RegisterIconEntity = 0;
unsigned int COffsets::FUNC_CNavigation__RegisterIconPos = 0;
unsigned int COffsets::FUNC_CNavigation__UnregisterIconPos = 0;
unsigned int COffsets::FUNC_CNavigation__UnregisterIconEntity = 0;

unsigned int COffsets::VAR_CPhysFS;

void COffsets::Initialise( BYTE byteVersion, unsigned int uiBase )
{
	switch( byteVersion )
	{
	case 0: // temp - v1 cracked
		{
			// Initialise pointers
			VAR_CGame						= (uiBase + 0x1AACD64);
			VAR_CGame__ScriptEngine			= (uiBase + 0x1AADBC8);
			VAR_CCamera						= (uiBase + 0x1AA99FC);
			VAR_CWindow						= (uiBase + 0x1AACD80);
			VAR_CWrapperList				= (uiBase + 0x1BAE088);
			VAR_VehiclePool					= (uiBase + 0x1AA29C0);
			VAR_CSDSManager					= (uiBase + 0x1AACD9C);
			VAR_LocalPlayer					= (uiBase + 0x1AACDAC);

			// 0x1AACD70 = VAR_CMiniMap

			break;
		}

		// Steam - unsupported
	case 1: // temp - steam
		{
			// Initialise pointers
			VAR_CGame						= (uiBase + 0x1ABFE14);
			VAR_CGame__ScriptEngine			= (uiBase + 0x1AC0C78);
			VAR_CCamera						= (uiBase + 0x1ABCAAC);
			VAR_CWindow						= (uiBase + 0x1ABFE30);
			VAR_CWrapperList				= (uiBase + 0x1CC14A8);
			VAR_VehiclePool					= (uiBase + 0x0);
			VAR_CSDSManager					= (uiBase + 0x0);
			VAR_LocalPlayer					= (uiBase + 0x1ABFE5C);

			break;
		}
	}

	// Initialise common addresses
	byteVersion == 0 ? CommonInitialise( uiBase ) : CommonInitialise__Steam( uiBase );
}

void COffsets::CommonInitialise( unsigned int uiBase )
{
	// Game
	FUNC_CGame__OnGameInit					= (uiBase + 0x410440); // Steam: 0x4105F0
	FUNC_CGame__OnGameEvent					= (uiBase + 0x1173A00); // Steam: 0x117BCA0
	FUNC_CGame__Allocate					= (uiBase + 0x401730); // Steam: 0x401830
	FUNC_CGame__Free						= (uiBase + 0x4018F0);
	FUNC_CGame__AllocateBuffer				= (uiBase + 0x4016F0);

	// Misc
	VAR_GuiMainSdsFolder					= (uiBase + 0x18E1D04);
	VAR_GuiMainSds							= (uiBase + 0x18E4DA0);
	VAR_GuiSds								= (uiBase + 0x18E4D30);
	VAR_TablesSds							= (uiBase + 0x18E4CFC);
	VAR_LoadingScreen						= (uiBase + 0x1949B58);
	VAR_StreamBin							= (uiBase + 0x18E3A44);
	FUNC_M2malloc							= (uiBase + 0x401730);
	FUNC_OnGameProcessStart					= (uiBase + 0x401A80);

	// Core
	VAR_CCore								= (uiBase + 0x1AAF6C8);
	FUNC_CCore__AllocateModel				= (uiBase + 0x14DFBB0);

	// Door
	FUNC_CDoor__Open						= (uiBase + 0x4DBBA0);
	FUNC_CDoor__Kick						= (uiBase + 0x4E5600);
	FUNC_CDoor__Close						= (uiBase + 0x4DBC50);
	FUNC_CDoor__Lock						= (uiBase + 0x4E1700);
	FUNC_CDoor__Unlock						= (uiBase + 0x4E1730);
	FUNC_CDoor__EnableAction				= (uiBase + 0x449FF0);
	FUNC_CDoor__DisableAction				= (uiBase + 0x44A060);

	// Model Manager
	FUNC_CModelMgr__Load					= (uiBase + 0x5BB590);
	FUNC_CModelMgr__Free					= (uiBase + 0x59F0F0);
	FUNC_CModelMgr__Construct				= (uiBase + 0x5A64E0);
	FUNC_CModelMgr__ChangeModel				= (uiBase + 0x5BB7E0);

	// Slot Manager
	VAR_CSlotManager						= (uiBase + 0x1AACD8C);

	// Obj Manager
	VAR_CObjectTable						= (uiBase + 0x1AACD70);

	// Hash
	FUNC_fnv64								= (uiBase + 0x41C660);
	FUNC_fnv32								= (uiBase + 0x41C6B0);

	// SDS
	FUNC_CSDSManager__ActivateStreamMapLine	= (uiBase + 0x5BF630);

	// Entity manager
	FUNC_CEntMgr__ProcessEntities			= (uiBase + 0x973B90); // Steam: 0x985460

	// Wrapper lists
	FUNC_CWrapperList__GetEntityByName		= (uiBase + 0xD8E3B0); // Steam: 0xD955A0

	// Camera
	FUNC_CCamera__GetGameCameraFromIndex	= (uiBase + 0x1079D40); // Steam: 0x10822C0
	
	// Lua
	FUNC_CLua__pcall						= (uiBase + 0x5C3870); // Steam: 0x5D3F80
	FUNC_CLua__loadbuffer					= (uiBase + 0x5C54C0); // Steam: 0x5D5BD0
	FUNC_CLua__settop						= (uiBase + 0x5C2110); // Steam: 0x5D2820
	FUNC_CLua__gettop						= (uiBase + 0x5C20F0); // Steam: 0x5D2800
	
	// Don't need these?
	FUNC_CLua__setfield						= (uiBase + CPatcher::FindPattern( (BYTE *)"\x55\x8B\xEC\x83\xEC\x20\x8B\x45\x0C\x50\x8B\x4D\x08\x51\xE8\xCD", "xxxxxxxxxxxxxxxx" ));
	FUNC_CLua__tolstring					= (uiBase + CPatcher::FindPattern( (BYTE *)"\x55\x8B\xEC\x51\x8B\x45\x0C\x50\x8B\x4D\x08\x51\xE8\x8F\xF8\xFF", "xxxxxxxxxxxxxxxx" ));
	FUNC_CLua__pushcclosure					= (uiBase + CPatcher::FindPattern( (BYTE *)"\x55\x8B\xEC\x83\xEC\x10\x8B\x45\x08\x8B\x48\x10\x8B\x55\x08\x8B", "xxxxxxxxxxxxxxxx" ));
	FUNC_CLua__tointeger					= (uiBase + CPatcher::FindPattern( (BYTE *)"\x55\x8B\xEC\x83\xEC\x14\x8B\x45\x0C\x50\x8B\x4D\x08\x51\xE8\x3D", "xxxxxxxxxxxxxxxx" ));
	FUNC_CLua__newthread					= (uiBase + CPatcher::FindPattern( (BYTE *)"\x55\x8B\xEC\x83\xEC\x08\x8B\x45\x08\x8B\x48\x10\x8B\x55\x08\x8B", "xxxxxxxxxxxxxxxx" ));

	// Human
	FUNC_CHumanInventory__ProcessShot		= (uiBase + 0x977FE0);
	FUNC_CHuman__ShowHat					= (uiBase + 0x90C880); // Steam: 0x91DCC0
	FUNC_CHuman__DropHat					= (uiBase + 0x92A450); // Steam: 0x93B890
	FUNC_CHuman__SetHealth					= (uiBase + 0x92A480); // Steam: 0x93B8C0
	FUNC_CHuman__SetReactions				= (uiBase + 0x90CEA0); // Steam: 0x91E2E0
	FUNC_CHuman__InventorySelect            = (uiBase + 0x983A30); // Steam: 0x9953A0
	FUNC_CHuman__InventoryAddWeapon			= (uiBase + 0x949C50); // Steam: 0x95B030
	FUNC_CHuman__InventoryRemoveWeapon		= (uiBase + 0x925960); // Steam: 0x936D90
	FUNC_CHuman__ToggleStealthKill          = (uiBase + 0x92A640); // Steam: 0x93BA80
	FUNC_CHuman__EnablePushByPlayer         = (uiBase + 0x92A6C0); // Steam: 0x93BB00
	FUNC_CHuman__MoveVec					= (uiBase + 0x97F740); // Steam: 0x9910B0
	FUNC_CHuman__ShootAt					= (uiBase + 0x982420); // Steam: 0x993C30
	FUNC_CHuman__AimAt						= (uiBase + 0x982190); // Steam: 0x993B00
	FUNC_CHuman__LookAt						= (uiBase + 0x981860); // Steam: 0x9931D0
	FUNC_CHuman__LockControls				= (uiBase + 0x94B9C0);
	FUNC_CHuman__AreControlsLocked			= (uiBase + 0x90CA00); // Steam: 0x91DE40
	FUNC_CHuman__InventoryHaveItem			= (uiBase + 0x9D3000); // Steam: 0x9E3280
	FUNC_CHuman__GetRealHealth				= (uiBase + 0x90CAF0);
	FUNC_CHuman__InventoryAddMoney			= (uiBase + 0x949C80); // Steam: 0x95B060
	FUNC_CHuman__InventoryRemoveMoney		= (uiBase + 0x93BE60); // Steam: 0x94D250
	FUNC_CHuman__InventoryGetMoney			= (uiBase + 0x931B80); // Steam: 0x942FB0
	FUNC_CHuman__InventoryDoReload			= (uiBase + 0x958140); // Steam: 0x969510
	FUNC_CHuman__SetControlStyle			= (uiBase + 0x94B9C0);
	FUNC_CHuman__GetControlStyle			= (uiBase + 0x90CA00); // Steam: 0x91DE40 (SAME AS CHuman__AreControlsLocked)
	FUNC_CHuman__HaveThrowingWeaponInHand	= (uiBase + 0x420040); // Steam: 0x420230
	FUNC_CHuman__IsOnScreen					= (uiBase + 0xD74FE0); // Steam: 0xD7C150
	FUNC_CHuman__AnimationReset				= (uiBase + 0x98B960); // Steam: 0x99D340
	FUNC_CHuman__GetInOutCar				= (uiBase + 0x98C800); // Steam: 0x99E1E0

	// Vehicle
	FUNC_CVehicle__OpenHood					= (uiBase + 0x9AC050);
	FUNC_CVehicle__CloseHood				= (uiBase + 0x9AC0D0);
	FUNC_CVehicle__OpenTrunk				= (uiBase + 0x9ABF50);
	FUNC_CVehicle__CloseTrunk				= (uiBase + 0x9ABFD0);
	FUNC_CVehicle__SetDirtLevel				= (uiBase + 0x9BC1B0);
	FUNC_CVehicle__SetEngineOn				= (uiBase + 0x1259FB0);
	FUNC_CVehicle__SetPainting				= (uiBase + 0x446B70);
	FUNC_CVehicle__SetActualTuningTable		= (uiBase + 0x9CA0D0);
	FUNC_CVehicle__SetSPZText				= (uiBase + 0x11EE750);
	FUNC_CVehicle__SetSpeedFloat			= (uiBase + 0x98E7F0);
	FUNC_CVehicle__SetBeaconLightOn			= (uiBase + 0x1203DD0);
	FUNC_CVehicle__SetHandbrake				= (uiBase + 0x11FB6D0);
	FUNC_CVehicle__OpenSeatWindow			= (uiBase + 0x990AA0);
	FUNC_CVehicle__Explode					= (uiBase + 0xA032A0);
	FUNC_CVehicle__Repair					= (uiBase + 0x4DA7B0);
	FUNC_CVehicle__SetEngineDamage			= (uiBase + 0x9A9E50);
	FUNC_CVehicle__GetEngineDamage			= (uiBase + 0x9A9DC0);
	FUNC_CVehicle__SetSpeedLimited			= (uiBase + 0x467BB0);
	FUNC_CVehicle__SetOnPart				= (uiBase + 0x125A600);
	FUNC_CVehicle__SetOffPart				= (uiBase + 0x125A530);
	FUNC_CVehicle__SetColors				= (uiBase + 0x9AB010);

	// Sync Objects
	FUNC_CSyncObject__Reactivate			= (uiBase + 0x11DE7D0);
	
	// Hud
	VAR_CHud								= (uiBase + 0x1BA71F8);
	VAR_CHudFader							= (uiBase + 0x1AA3448);
	FUNC_CHud__FaderFadeIn					= (uiBase + 0x87FFA0);
	FUNC_CHud__FaderFadeOut					= (uiBase + 0x87FDF0);
	FUNC_CHud__Toggle						= (uiBase + 0x893B30);
	FUNC_CHudComponent__Show				= (uiBase + 0x87A0B0);
	FUNC_CHud__ScoreShow					= (uiBase + 0x87B760);
	FUNC_CHud__RadarShow					= (uiBase + 0x8823F0);
	FUNC_CHud__SubtitlesShow				= (uiBase + 0x8824B0);

	// Map (Unused)
	VAR_CMap								= (uiBase + 0x1AACD70);
	FUNC_CMap__CreateEntityIcon				= (uiBase + 0x83B420);
	FUNC_CMap__DeleteEntityIcon				= (uiBase + 0x83B540);

	// Navigation
	VAR_CNavigation							= (uiBase + 0x1AA3398);
	FUNC_CNavigation__RegisterIconEntity	= (uiBase + 0x4D82E0);
	FUNC_CNavigation__RegisterIconPos		= (uiBase + 0x4D8270);
	FUNC_CNavigation__UnregisterIconPos		= (uiBase + 0x4C97C0);
	FUNC_CNavigation__UnregisterIconEntity	= (uiBase + 0x4C98F0);

	// PhysFS
	VAR_CPhysFS								= (uiBase + 0x1AC94F0);
}

void COffsets::CommonInitialise__Steam( unsigned int uiBase )
{
}