// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/game/params/C_StateMachine.h>
#include <mafia/gui/menu/helpers/I_GamaCorrectionSettingCallback.h>
#include <ue/sys/userservices/I_SysNotificationCallback.h>
#include <mafia/gui/I_GuiListDialogCallback.h>

namespace ue
{

namespace game
{

namespace params
{

/** ue::game::params::C_StateMachinePlatformSpec (VTable=0x01E7A3E0) */
class C_StateMachinePlatformSpec : public C_StateMachine
{
public:
	/** ue::game::params::C_StateMachinePlatformSpec::C_LoadProfileCorrupted (VTable=0x01E7A660) */
	class C_LoadProfileCorrupted : public ue::game::params::C_StateMachine::I_InteractiveState
	{
	public:
		virtual void vfn_0001_2D21BF1B() = 0;
		virtual void vfn_0002_2D21BF1B() = 0;
		virtual void vfn_0003_2D21BF1B() = 0;
		virtual void vfn_0004_2D21BF1B() = 0;
		virtual void vfn_0005_2D21BF1B() = 0;
		virtual void vfn_0006_2D21BF1B() = 0;
		virtual void vfn_0007_2D21BF1B() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_CreateProfileGammaSettings (VTable=0x01E7A720) */
	class C_CreateProfileGammaSettings : public ue::game::params::C_StateMachine::I_State, public mafia::gui::menu::helpers::I_GamaCorrectionSettingCallback
	{
	public:
		virtual void vfn_0001_A9FD93DC() = 0;
		virtual void vfn_0002_A9FD93DC() = 0;
		virtual void vfn_0003_A9FD93DC() = 0;
		virtual void vfn_0004_A9FD93DC() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_CreateProfileEnterName (VTable=0x01E7A180) */
	class C_CreateProfileEnterName : public ue::game::params::C_StateMachine::I_InteractiveState
	{
	public:
		virtual void vfn_0001_B2F99470() = 0;
		virtual void vfn_0002_B2F99470() = 0;
		virtual void vfn_0003_B2F99470() = 0;
		virtual void vfn_0004_B2F99470() = 0;
		virtual void vfn_0005_B2F99470() = 0;
		virtual void vfn_0006_B2F99470() = 0;
		virtual void vfn_0007_B2F99470() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_CheckProfileOtherError (VTable=0x01E7A2A0) */
	class C_CheckProfileOtherError : public ue::game::params::C_StateMachine::I_InteractiveState
	{
	public:
		virtual void vfn_0001_0FDA950E() = 0;
		virtual void vfn_0002_0FDA950E() = 0;
		virtual void vfn_0003_0FDA950E() = 0;
		virtual void vfn_0004_0FDA950E() = 0;
		virtual void vfn_0005_0FDA950E() = 0;
		virtual void vfn_0006_0FDA950E() = 0;
		virtual void vfn_0007_0FDA950E() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_CheckProfileSignInGWLGuide (VTable=0x01E7A300) */
	class C_CheckProfileSignInGWLGuide : public ue::game::params::C_StateMachine::I_State, public ue::sys::userservices::I_SysNotificationCallback
	{
	public:
		virtual void vfn_0001_FEED13DC() = 0;
		virtual void vfn_0002_FEED13DC() = 0;
		virtual void vfn_0003_FEED13DC() = 0;
		virtual void vfn_0004_FEED13DC() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_SaveProfileOrSaveGameOtherError (VTable=0x01E7A4E0) */
	class C_SaveProfileOrSaveGameOtherError : public ue::game::params::C_StateMachine::I_InteractiveState
	{
	public:
		virtual void vfn_0001_7A6860A8() = 0;
		virtual void vfn_0002_7A6860A8() = 0;
		virtual void vfn_0003_7A6860A8() = 0;
		virtual void vfn_0004_7A6860A8() = 0;
		virtual void vfn_0005_7A6860A8() = 0;
		virtual void vfn_0006_7A6860A8() = 0;
		virtual void vfn_0007_7A6860A8() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_LoadGameError (VTable=0x01E7A520) */
	class C_LoadGameError : public ue::game::params::C_StateMachine::I_InteractiveState
	{
	public:
		virtual void vfn_0001_CF523FE5() = 0;
		virtual void vfn_0002_CF523FE5() = 0;
		virtual void vfn_0003_CF523FE5() = 0;
		virtual void vfn_0004_CF523FE5() = 0;
		virtual void vfn_0005_CF523FE5() = 0;
		virtual void vfn_0006_CF523FE5() = 0;
		virtual void vfn_0007_CF523FE5() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_LoadProfileGameUpdateNeeded (VTable=0x01E7A560) */
	class C_LoadProfileGameUpdateNeeded : public ue::game::params::C_StateMachine::I_InteractiveState
	{
	public:
		virtual void vfn_0001_071E6393() = 0;
		virtual void vfn_0002_071E6393() = 0;
		virtual void vfn_0003_071E6393() = 0;
		virtual void vfn_0004_071E6393() = 0;
		virtual void vfn_0005_071E6393() = 0;
		virtual void vfn_0006_071E6393() = 0;
		virtual void vfn_0007_071E6393() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_LoadGameDLCMissingMessage (VTable=0x01E7A5A0) */
	class C_LoadGameDLCMissingMessage : public ue::game::params::C_StateMachine::I_InteractiveState
	{
	public:
		virtual void vfn_0001_07CD0353() = 0;
		virtual void vfn_0002_07CD0353() = 0;
		virtual void vfn_0003_07CD0353() = 0;
		virtual void vfn_0004_07CD0353() = 0;
		virtual void vfn_0005_07CD0353() = 0;
		virtual void vfn_0006_07CD0353() = 0;
		virtual void vfn_0007_07CD0353() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_LoadGameFailed (VTable=0x01E7A5E0) */
	class C_LoadGameFailed : public ue::game::params::C_StateMachine::I_State
	{
	public:
		virtual void vfn_0001_2BA8F9B0() = 0;
		virtual void vfn_0002_2BA8F9B0() = 0;
		virtual void vfn_0003_2BA8F9B0() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_CancelLoadGame (VTable=0x01E7A5F8) */
	class C_CancelLoadGame : public ue::game::params::C_StateMachine::I_State
	{
	public:
		virtual void vfn_0001_F879BE52() = 0;
		virtual void vfn_0002_F879BE52() = 0;
		virtual void vfn_0003_F879BE52() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_LoadProfileWarningOverwrite (VTable=0x01E7A620) */
	class C_LoadProfileWarningOverwrite : public ue::game::params::C_StateMachine::I_InteractiveState
	{
	public:
		virtual void vfn_0001_B5B28600() = 0;
		virtual void vfn_0002_B5B28600() = 0;
		virtual void vfn_0003_B5B28600() = 0;
		virtual void vfn_0004_B5B28600() = 0;
		virtual void vfn_0005_B5B28600() = 0;
		virtual void vfn_0006_B5B28600() = 0;
		virtual void vfn_0007_B5B28600() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_LoadProfileCanceled (VTable=0x01E7A6E0) */
	class C_LoadProfileCanceled : public ue::game::params::C_StateMachine::I_InteractiveState
	{
	public:
		virtual void vfn_0001_4D38D20D() = 0;
		virtual void vfn_0002_4D38D20D() = 0;
		virtual void vfn_0003_4D38D20D() = 0;
		virtual void vfn_0004_4D38D20D() = 0;
		virtual void vfn_0005_4D38D20D() = 0;
		virtual void vfn_0006_4D38D20D() = 0;
		virtual void vfn_0007_4D38D20D() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_LoadProfileOrLoadGameOtherError (VTable=0x01E7A6A0) */
	class C_LoadProfileOrLoadGameOtherError : public ue::game::params::C_StateMachine::I_InteractiveState
	{
	public:
		virtual void vfn_0001_8DA5CDC2() = 0;
		virtual void vfn_0002_8DA5CDC2() = 0;
		virtual void vfn_0003_8DA5CDC2() = 0;
		virtual void vfn_0004_8DA5CDC2() = 0;
		virtual void vfn_0005_8DA5CDC2() = 0;
		virtual void vfn_0006_8DA5CDC2() = 0;
		virtual void vfn_0007_8DA5CDC2() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_CreateProfileTestStorage (VTable=0x01E7A158) */
	class C_CreateProfileTestStorage : public ue::game::params::C_StateMachine::I_State
	{
	public:
		virtual void vfn_0001_E8CB8BC4() = 0;
		virtual void vfn_0002_E8CB8BC4() = 0;
		virtual void vfn_0003_E8CB8BC4() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_DeleteProfileWarning (VTable=0x01E79FC0) */
	class C_DeleteProfileWarning : public ue::game::params::C_StateMachine::I_InteractiveState
	{
	public:
		virtual void vfn_0001_46454253() = 0;
		virtual void vfn_0002_46454253() = 0;
		virtual void vfn_0003_46454253() = 0;
		virtual void vfn_0004_46454253() = 0;
		virtual void vfn_0005_46454253() = 0;
		virtual void vfn_0006_46454253() = 0;
		virtual void vfn_0007_46454253() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_DeleteProfileConfirmation (VTable=0x01E7A000) */
	class C_DeleteProfileConfirmation : public ue::game::params::C_StateMachine::I_InteractiveState
	{
	public:
		virtual void vfn_0001_90813A3D() = 0;
		virtual void vfn_0002_90813A3D() = 0;
		virtual void vfn_0003_90813A3D() = 0;
		virtual void vfn_0004_90813A3D() = 0;
		virtual void vfn_0005_90813A3D() = 0;
		virtual void vfn_0006_90813A3D() = 0;
		virtual void vfn_0007_90813A3D() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_SaveProfileOrSaveGameNoSpace (VTable=0x01E7A040) */
	class C_SaveProfileOrSaveGameNoSpace : public ue::game::params::C_StateMachine::I_InteractiveState
	{
	public:
		virtual void vfn_0001_95C3DE5A() = 0;
		virtual void vfn_0002_95C3DE5A() = 0;
		virtual void vfn_0003_95C3DE5A() = 0;
		virtual void vfn_0004_95C3DE5A() = 0;
		virtual void vfn_0005_95C3DE5A() = 0;
		virtual void vfn_0006_95C3DE5A() = 0;
		virtual void vfn_0007_95C3DE5A() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_SaveProfileOrSaveGame (VTable=0x01E7A080) */
	class C_SaveProfileOrSaveGame : public ue::game::params::C_StateMachine::I_State
	{
	public:
		virtual void vfn_0001_998A2CCA() = 0;
		virtual void vfn_0002_998A2CCA() = 0;
		virtual void vfn_0003_998A2CCA() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_LoadProfileOrLoadGame (VTable=0x01E7A098) */
	class C_LoadProfileOrLoadGame : public ue::game::params::C_StateMachine::I_State
	{
	public:
		virtual void vfn_0001_3551F39B() = 0;
		virtual void vfn_0002_3551F39B() = 0;
		virtual void vfn_0003_3551F39B() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_CreateProfileOtherError (VTable=0x01E7A0C0) */
	class C_CreateProfileOtherError : public ue::game::params::C_StateMachine::I_InteractiveState
	{
	public:
		virtual void vfn_0001_64C3E9F6() = 0;
		virtual void vfn_0002_64C3E9F6() = 0;
		virtual void vfn_0003_64C3E9F6() = 0;
		virtual void vfn_0004_64C3E9F6() = 0;
		virtual void vfn_0005_64C3E9F6() = 0;
		virtual void vfn_0006_64C3E9F6() = 0;
		virtual void vfn_0007_64C3E9F6() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_CreateProfileNoSpace (VTable=0x01E7A100) */
	class C_CreateProfileNoSpace : public ue::game::params::C_StateMachine::I_InteractiveState
	{
	public:
		virtual void vfn_0001_B1AE9106() = 0;
		virtual void vfn_0002_B1AE9106() = 0;
		virtual void vfn_0003_B1AE9106() = 0;
		virtual void vfn_0004_B1AE9106() = 0;
		virtual void vfn_0005_B1AE9106() = 0;
		virtual void vfn_0006_B1AE9106() = 0;
		virtual void vfn_0007_B1AE9106() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_CreateProfileCall (VTable=0x01E7A140) */
	class C_CreateProfileCall : public ue::game::params::C_StateMachine::I_State
	{
	public:
		virtual void vfn_0001_41C7A563() = 0;
		virtual void vfn_0002_41C7A563() = 0;
		virtual void vfn_0003_41C7A563() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_CreateProfile (VTable=0x01E7A1C0) */
	class C_CreateProfile : public ue::game::params::C_StateMachine::I_State
	{
	public:
		virtual void vfn_0001_32012304() = 0;
		virtual void vfn_0002_32012304() = 0;
		virtual void vfn_0003_32012304() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_SelectProfile (VTable=0x01E7A1E0) */
	class C_SelectProfile : public ue::game::params::C_StateMachine::I_InteractiveState, public mafia::gui::I_GuiListDialogCallback
	{
	public:
		virtual void vfn_0001_6102B048() = 0;
		virtual void vfn_0002_6102B048() = 0;
		virtual void vfn_0003_6102B048() = 0;
		virtual void vfn_0004_6102B048() = 0;
		virtual void vfn_0005_6102B048() = 0;
		virtual void vfn_0006_6102B048() = 0;
		virtual void vfn_0007_6102B048() = 0;
		virtual void vfn_0008_6102B048() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_EndDisableSaveLoad (VTable=0x01E7A238) */
	class C_EndDisableSaveLoad : public ue::game::params::C_StateMachine::I_State
	{
	public:
		virtual void vfn_0001_FB23E32E() = 0;
		virtual void vfn_0002_FB23E32E() = 0;
		virtual void vfn_0003_FB23E32E() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_CheckProfileCanceled (VTable=0x01E7A260) */
	class C_CheckProfileCanceled : public ue::game::params::C_StateMachine::I_InteractiveState
	{
	public:
		virtual void vfn_0001_E9DFC92B() = 0;
		virtual void vfn_0002_E9DFC92B() = 0;
		virtual void vfn_0003_E9DFC92B() = 0;
		virtual void vfn_0004_E9DFC92B() = 0;
		virtual void vfn_0005_E9DFC92B() = 0;
		virtual void vfn_0006_E9DFC92B() = 0;
		virtual void vfn_0007_E9DFC92B() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_CheckProfileCall (VTable=0x01E7A2E0) */
	class C_CheckProfileCall : public ue::game::params::C_StateMachine::I_State
	{
	public:
		virtual void vfn_0001_0BC872CC() = 0;
		virtual void vfn_0002_0BC872CC() = 0;
		virtual void vfn_0003_0BC872CC() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_CheckProfileSignInWarning (VTable=0x01E7A340) */
	class C_CheckProfileSignInWarning : public ue::game::params::C_StateMachine::I_InteractiveState
	{
	public:
		virtual void vfn_0001_5900C437() = 0;
		virtual void vfn_0002_5900C437() = 0;
		virtual void vfn_0003_5900C437() = 0;
		virtual void vfn_0004_5900C437() = 0;
		virtual void vfn_0005_5900C437() = 0;
		virtual void vfn_0006_5900C437() = 0;
		virtual void vfn_0007_5900C437() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_CheckProfileSignIn (VTable=0x01E7A380) */
	class C_CheckProfileSignIn : public ue::game::params::C_StateMachine::I_State
	{
	public:
		virtual void vfn_0001_3B71A2AC() = 0;
		virtual void vfn_0002_3B71A2AC() = 0;
		virtual void vfn_0003_3B71A2AC() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_CheckProfile (VTable=0x01E7A398) */
	class C_CheckProfile : public ue::game::params::C_StateMachine::I_State
	{
	public:
		virtual void vfn_0001_A4F7122F() = 0;
		virtual void vfn_0002_A4F7122F() = 0;
		virtual void vfn_0003_A4F7122F() = 0;
	};

	/** ue::game::params::C_StateMachinePlatformSpec::C_SendQuitGameRequest (VTable=0x01E7A3B0) */
	class C_SendQuitGameRequest : public ue::game::params::C_StateMachine::I_State
	{
	public:
		virtual void vfn_0001_94D161CF() = 0;
		virtual void vfn_0002_94D161CF() = 0;
		virtual void vfn_0003_94D161CF() = 0;
	};

	virtual void vfn_0001_0755E442() = 0;
	virtual void vfn_0002_0755E442() = 0;
	virtual void vfn_0003_0755E442() = 0;
	virtual void vfn_0004_0755E442() = 0;
	virtual void vfn_0005_0755E442() = 0;
	virtual void vfn_0006_0755E442() = 0;
	virtual void vfn_0007_0755E442() = 0;
	virtual void vfn_0008_0755E442() = 0;
	virtual void vfn_0009_0755E442() = 0;
	virtual void vfn_0010_0755E442() = 0;
	virtual void vfn_0011_0755E442() = 0;
	virtual void vfn_0012_0755E442() = 0;
	virtual void vfn_0013_0755E442() = 0;
	virtual void vfn_0014_0755E442() = 0;
	virtual void vfn_0015_0755E442() = 0;
	virtual void vfn_0016_0755E442() = 0;
	virtual void vfn_0017_0755E442() = 0;
	virtual void vfn_0018_0755E442() = 0;
	virtual void vfn_0019_0755E442() = 0;
	virtual void vfn_0020_0755E442() = 0;
	virtual void vfn_0021_0755E442() = 0;
	virtual void vfn_0022_0755E442() = 0;
	virtual void vfn_0023_0755E442() = 0;
	virtual void vfn_0024_0755E442() = 0;
	virtual void vfn_0025_0755E442() = 0;
	virtual void vfn_0026_0755E442() = 0;
	virtual void vfn_0027_0755E442() = 0;
	virtual void vfn_0028_0755E442() = 0;
};

} // namespace params

} // namespace game

} // namespace ue
