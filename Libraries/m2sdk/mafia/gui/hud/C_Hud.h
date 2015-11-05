// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/game/cutscenes/I_ModuleNotifyHandler.h>
#include <ue/game/params/C_DifficultyManager.h>
#include <ue/game/params/I_SaveLoadCallBack.h>

namespace mafia
{

namespace gui
{

namespace hud
{

/** mafia::gui::hud::C_Hud (VTable=0xFFFFFFFF) */
class C_Hud
{
public:
	/** mafia::gui::hud::C_Hud::C_HudModuleNotifyHandler (VTable=0x01E6F3B8) */
	class C_HudModuleNotifyHandler : public ue::game::cutscenes::I_ModuleNotifyHandler
	{
	public:
		virtual void vfn_0001_43619EC0() = 0;
		virtual void vfn_0002_43619EC0() = 0;
		virtual void vfn_0003_43619EC0() = 0;
	};

	/** mafia::gui::hud::C_Hud::C_RetryHandler (VTable=0x01E6F380) */
	class C_RetryHandler : public ue::game::params::C_DifficultyManager::I_NotifyHandler
	{
	public:
		virtual void vfn_0001_D9E7FC34() = 0;
		virtual void vfn_0002_D9E7FC34() = 0;
		virtual void vfn_0003_D9E7FC34() = 0;
		virtual void vfn_0004_D9E7FC34() = 0;
		virtual void vfn_0005_D9E7FC34() = 0;
		virtual void vfn_0006_D9E7FC34() = 0;
	};

	/** mafia::gui::hud::C_Hud::C_SaveLoadCallback (VTable=0x01E6F3A0) */
	class C_SaveLoadCallback : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_1D134E20() = 0;
		virtual void vfn_0002_1D134E20() = 0;
		virtual void vfn_0003_1D134E20() = 0;
		virtual void vfn_0004_1D134E20() = 0;
	};
};

} // namespace hud

} // namespace gui

} // namespace mafia
