/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : COffsets.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class COffsets
{

public:

	// CIE
	static	unsigned int		FUNC_CIE_CreateObject;
	static	unsigned int		FUNC_CIE_Assign_Model;
	static	unsigned int		FUNC_CIE_SetHashName;
	static	unsigned int		FUNC_CIE_SetFlag;
	static	unsigned int		FUNC_CIE_SetModel;
	static	unsigned int		FUNC_CIE_UnkFunc;
	static	unsigned int		FUNC_CIE_GetTrafficHuman;
	static	unsigned int		FUNC_CIE_SetTrafficHuman;
	static	unsigned int		FUNC_CIE_GetHashedString;
	static	unsigned int		FUNC_CIE_LoadModel;
	static	unsigned int		FUNC_CIE_CCarSetup;
	static	unsigned int		FUNC_CIE_SetCarParams;

	// Game
	static	unsigned int		VAR_CGame;
	static	unsigned int		FUNC_CGame__OnGameInit;
	static	unsigned int		FUNC_CGame__OnGameEvent;
	static	unsigned int		FUNC_CGame__OnGameLoad;
	static	unsigned int		VAR_CGame__ScriptEngine;
	static	unsigned int		FUNC_CGame__Allocate;
	static	unsigned int		FUNC_CGame__Free;
	static	unsigned int		FUNC_CGame__AllocateBuffer;

	// Misc
	static	unsigned int		VAR_LocalPlayer;
	static	unsigned int		VAR_GuiMainSdsFolder;
	static	unsigned int		VAR_GuiMainSds;
	static	unsigned int		VAR_GuiSds;
	static	unsigned int		VAR_TablesSds;
	static	unsigned int		VAR_LoadingScreen;
	static	unsigned int		VAR_StreamBin;
	static	unsigned int		VAR_SDSConfigFile;
	static	unsigned int		FUNC_M2malloc;
	static	unsigned int		FUNC_OnGameProcessStart;

	// Core
	static	unsigned int		VAR_CCore;
	static	unsigned int		FUNC_CCore__AllocateModel;

	// Door
	static	unsigned int		FUNC_CDoor__Open;
	static	unsigned int		FUNC_CDoor__Kick;
	static	unsigned int		FUNC_CDoor__Close;
	static	unsigned int		FUNC_CDoor__Lock;
	static	unsigned int		FUNC_CDoor__Unlock;
	static	unsigned int		FUNC_CDoor__EnableAction;
	static	unsigned int		FUNC_CDoor__DisableAction;

	// Model Mgr
	static	unsigned int		FUNC_CModelMgr__Load;
	static	unsigned int		FUNC_CModelMgr__Free;
	static	unsigned int		FUNC_CModelMgr__Construct;
	static	unsigned int		FUNC_CModelMgr__ChangeModel;

	// Slot Mgr
	static	unsigned int		VAR_CSlotManager;

	// Obj Mgr
	static	unsigned int		VAR_CObjectTable;

	// Hash
	static	unsigned int		FUNC_fnv64;
	static	unsigned int		FUNC_fnv32;

	// Window
	static	unsigned int		VAR_CWindow;

	// Sds
	static	unsigned int		VAR_CSDSManager;
	static	unsigned int		FUNC_CSDSManager__ActivateStreamMapLine;

	// Entity Manager
	static	unsigned int		FUNC_CEntMgr__ProcessEntities;

	// Entity
	static	unsigned int		FUNC_CEntity__Create;
	static	unsigned int		FUNC_CEntity__Activate;
	static	unsigned int		FUNC_CEntity__Deactivate;
	static	unsigned int		FUNC_CEntity__Delete;

	// Entity wrapper
	static	unsigned int		VAR_CWrapperList;
	static	unsigned int		FUNC_CWrapperList__GetEntityByName;

	// Camera
	static	unsigned int		VAR_CCamera;
	static	unsigned int		VAR_CCamera__State;
	static	unsigned int		FUNC_CCamera__GetGameCameraFromIndex;

	// Lua
	static	unsigned int		FUNC_CLua__pcall;
	static	unsigned int		FUNC_CLua__loadbuffer;
	static	unsigned int		FUNC_CLua__settop;
	static	unsigned int		FUNC_CLua__gettop;
	static	unsigned int		FUNC_CLua__setfield;
	static	unsigned int		FUNC_CLua__tolstring;
	static	unsigned int		FUNC_CLua__pushcclosure;
	static	unsigned int		FUNC_CLua__tointeger;
	static	unsigned int		FUNC_CLua__newthread;
	static	unsigned int		FUNC_CLua__getfield;
	static	unsigned int		FUNC_CLua__type_t;
	static	unsigned int		FUNC_CLua__typename_t;
	static	unsigned int		FUNC_CLua__pushnil;
	static	unsigned int		FUNC_CLua__next;
	static	unsigned int		FUNC_CLua__pushvalue;
	static	unsigned int		FUNC_CLua__isuserdata;
	static	unsigned int		FUNC_CLua__touserdata;

	// Human
	static	unsigned int		FUNC_CHumanInventory__ProcessShot;
	static	unsigned int		FUNC_CHumanInventory__DoReload;
	static	unsigned int		FUNC_CHuman__InventorySelect;
	static	unsigned int		FUNC_CHuman__InventoryAddWeapon;
	static  unsigned int		FUNC_CHuman__InventoryRemoveWeapon;
	static	unsigned int		FUNC_CHuman__InventoryTellAmmoInSelectedWeapon;
	static	unsigned int		FUNC_CHuman__ToggleStealthKill;
	static	unsigned int		FUNC_CHuman__EnablePushByPlayer;
	static	unsigned int		FUNC_CHuman__SetReactions;
	static	unsigned int		FUNC_CHuman__MoveVec;
	static	unsigned int		FUNC_CHuman__ShootAt;
	static	unsigned int		FUNC_CHuman__ShootAtEffect;
	static	unsigned int		FUNC_CHuman__AimAt;
	static	unsigned int		FUNC_CHuman__LookAt;
	static	unsigned int		FUNC_CHuman__ShowHat;
	static  unsigned int		FUNC_CHuman__DropHat;
	static	unsigned int		FUNC_CHuman__SetHealth;
	static	unsigned int		FUNC_CHuman__LockControls;
	static	unsigned int		FUNC_CHuman__AreControlsLocked;
	static	unsigned int		FUNC_CHuman__InventoryHaveItem;
	static	unsigned int		FUNC_CHuman__GetRealHealth;
	static  unsigned int		FUNC_CHuman__InventoryAddMoney;
	static  unsigned int		FUNC_CHuman__InventoryRemoveMoney;
	static	unsigned int		FUNC_CHuman__InventoryGetMoney;
	static	unsigned int		FUNC_CHuman__InventoryDoReload;
	static	unsigned int		FUNC_CHuman__GetControlStyle;
	static  unsigned int		FUNC_CHuman__SetControlStyle;
	static	unsigned int		FUNC_CHuman__HaveThrowingWeaponInHand;
	static	unsigned int		FUNC_CHuman__IsOnScreen;
	static	unsigned int		FUNC_CHuman__AnimationReset;
	static	unsigned int		FUNC_CHuman__GetInOutCar;
	static	unsigned int		FUNC_CHuman__TakeDamage;
	static	unsigned int		FUNC_CHuman__SetAnimStyle;
	static	unsigned int		FUNC_CHuman__SetModelToHand;
	static	unsigned int		FUNC_CHuman__SetModelToMouth;
	static	unsigned int		FUNC_CHuman__SetPhysState;
	static	unsigned int		FUNC_CHuman__SetStealthMove;

	// Vehicle
	static	unsigned int		VAR_VehiclePool;
	static	unsigned int		FUNC_CVehicle__OpenHood;
	static	unsigned int		FUNC_CVehicle__CloseHood;
	static	unsigned int		FUNC_CVehicle__OpenTrunk;
	static	unsigned int		FUNC_CVehicle__CloseTrunk;
	static	unsigned int		FUNC_CVehicle__SetDirtLevel;
	static	unsigned int		FUNC_CVehicle__SetEngineOn;
	static	unsigned int		FUNC_CVehicle__SetPainting;
	static	unsigned int		FUNC_CVehicle__SetActualTuningTable;
	static	unsigned int		FUNC_CVehicle__SetSPZText;
	static	unsigned int		FUNC_CVehicle__SetSpeedFloat;
	static	unsigned int		FUNC_CVehicle__SetBeaconLightOn;
	static	unsigned int		FUNC_CVehicle__SetHandbrake;
	static	unsigned int		FUNC_CVehicle__OpenSeatWindow;
	static	unsigned int		FUNC_CVehicle__IsWindowOpen;
	static  unsigned int		FUNC_CVehicle__Repair;
	static  unsigned int		FUNC_CVehicle__GetEngineDamage;
	static  unsigned int		FUNC_CVehicle__SetEngineDamage;
	static	unsigned int		FUNC_CVehicle__SetColors;
	static  unsigned int		FUNC_CVehicle__Explode;
	static	unsigned int		FUNC_CVehicle__SetSpeedLimited;
	static	unsigned int		FUNC_CVehicle__SetOnPart;
	static	unsigned int		FUNC_CVehicle__SetOffPart;
	static	unsigned int		FUNC_CVehicle__SetSpeedVec;
	static	unsigned int		FUNC_CVehicle__AddSteer;
	static	unsigned int		FUNC_CVehicle__SetPower;
	static	unsigned int		FUNC_CVehicle__SetBrake;
	static	unsigned int		FUNC_CVehicle__FixCarPos;
	static	unsigned int		FUNC_CVehicle__SetWheelMatrixAtIndex;
	static	unsigned int		FUNC_CVehicle__GetWheelsMaterial;
	static	unsigned int		FUNC_CVehicle__SetLightState;
	static	unsigned int		FUNC_CVehicle__PlayerStartEnter;

	static	unsigned int		FUNC_CVehicle__MarkForSale;
	static	unsigned int		FUNC_CVehicle__SetIndicatorLightOn;
	static	unsigned int		FUNC_CVehicle__SetTaxiLightOn;
	static	unsigned int		FUNC_CVehicle__OpenDoor;
	static	unsigned int		FUNC_CVehicle__SetDoorFree;
	static	unsigned int		FUNC_CVehicle__SetVehicleColor;
	static	unsigned int		FUNC_CVehicle__GetVehicleColor;
	static	unsigned int		FUNC_CVehicle__SetDynamic;
	static	unsigned int		FUNC_CVehicle__StopAllSounds;
	static	unsigned int		FUNC_CVehicle__AddVehicleFlags;
	static	unsigned int		FUNC_CVehicle__ClearVehicleFlags;

	static	unsigned int		FUNC_CCar__Spawn;
	static	unsigned int		FUNC_CCar__GetDamage;
	static	unsigned int		FUNC_CCar__SetCarDamage;
	static	unsigned int		FUNC_CCar__SetTransparency;
	static	unsigned int		FUNC_CCar__OpenDoors;
	static	unsigned int		FUNC_CCar__LockThrowFromCar;
	static	unsigned int		FUNC_CCar__LockTrunks;
	static	unsigned int		FUNC_CCar__UnlockTrunks;
	static	unsigned int		FUNC_CCar__SetGear;
	static	unsigned int		FUNC_CCar__SetGearBoxAutomat;
	static	unsigned int		FUNC_CCar__ResetRigidBody;

	// Sync Objects
	static	unsigned int		FUNC_CSyncObject__Reactivate;

	// Hud
	static	unsigned int		VAR_CHud;
	static	unsigned int		VAR_CHudFader;
	static  unsigned int		FUNC_CHud__FaderFadeIn;
	static	unsigned int		FUNC_CHud__FaderFadeOut;
	static	unsigned int		FUNC_CHud__Toggle;
	static	unsigned int		FUNC_CHudComponent__Show;
	static	unsigned int		FUNC_CHud__ScoreShow;
	static	unsigned int		FUNC_CHud__RadarShow;
	static	unsigned int		FUNC_CHud__SubtitlesShow;
	static	unsigned int		FUNC_CHud__SpeedoShow;

	// Map
	static  unsigned int		VAR_CMap;
	static  unsigned int		FUNC_CMap__CreateEntityIcon;
	static  unsigned int		FUNC_CMap__DeleteEntityIcon;

	// Navigation
	static	unsigned int		VAR_CNavigation;
	static	unsigned int		FUNC_CNavigation__RegisterIconEntity;
	static	unsigned int		FUNC_CNavigation__RegisterIconPos;
	static	unsigned int		FUNC_CNavigation__UnregisterIconPos;
	static	unsigned int		FUNC_CNavigation__UnregisterIconEntity;
	static	unsigned int		FUNC_CNavigation__GetIconFromId;
	static	unsigned int		FUNC_CNavigation__GetIconFromEntity;

	// Physfs
	static	unsigned int		VAR_CPhysFS;

	static	void				Initialise(BYTE gameVersion, unsigned int uiBase);
	static	void				CommonInitialise(unsigned int uiBase);
	static	void				CommonInitialise__Steam(unsigned int uiBase);

	// Video settings
	static	unsigned int		FUNC_C_SystemRenderingModule__SetFullScreenEnabled;
	static	unsigned int		FUNC_C_SystemRenderingModule__SetVerticalSyncEnabled;
	static	unsigned int		FUNC_SetScreenResolution;
	static	unsigned int		FUNC_SetMultisampleAntiAliasing;
	static	unsigned int		FUNC_SetAnisotropicFiltering;
	static	unsigned int		FUNC_EnableAmbientOcclusion;

	// Audio settings
	static	unsigned int		FUNC_SetAudioQuality;
	static	unsigned int		FUNC_SetSFXVolume;
	static	unsigned int		FUNC_SetVoicesVolume;
	static	unsigned int		FUNC_SetMusicVolume;
	static	unsigned int		FUNC_SetRadioVolume;

	// Player
	static	unsigned int		FUNC_CPlayer_ProcessKeyboard;

	// Text Datbase
	static	unsigned int		VAR_CTextDatabase;
	static	unsigned int		FUNC_CTextDatabase__Clear;
	static	unsigned int		FUNC_CTextDatabase__Done;
	static	unsigned int		FUNC_CTextDatabase__GetString;
	static	unsigned int		FUNC_CTextDatabase__Init;
	static	unsigned int		FUNC_CTextDatabase__LoadIconsMap;
	static	unsigned int		FUNC_CTextDatabase__ResetHistory;
};