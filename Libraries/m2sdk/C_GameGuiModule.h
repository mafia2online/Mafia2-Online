// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <mafia/gui/I_MenuManager.h>
#include <mafia/gui/utils/C_SlotCache.h>
#include <ue/game/params/I_SaveLoadCallBack.h>

/** C_GameGuiModule (VTable=0x01E7F8E0) */
class C_GameGuiModule : public C_TickedModule, public mafia::gui::I_MenuManager, public mafia::gui::utils::C_SlotCache::I_SlotCacheCallback
{
public:
	/** C_GameGuiModule::C_SaveLoadCallback (VTable=0x01E7F950) */
	class C_SaveLoadCallback : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_7F15DF26() = 0;
		virtual void vfn_0002_7F15DF26() = 0;
		virtual void vfn_0003_7F15DF26() = 0;
		virtual void vfn_0004_7F15DF26() = 0;
	};

	virtual void vfn_0001_DCACE760() = 0;
	virtual void vfn_0002_DCACE760() = 0;
	virtual void vfn_0003_DCACE760() = 0;
	virtual void vfn_0004_DCACE760() = 0;
	virtual void vfn_0005_DCACE760() = 0;
	virtual void vfn_0006_DCACE760() = 0;
	virtual void vfn_0007_DCACE760() = 0;
	virtual void vfn_0008_DCACE760() = 0;
	virtual void vfn_0009_DCACE760() = 0;
	virtual void vfn_0010_DCACE760() = 0;
	virtual void vfn_0011_DCACE760() = 0;
	virtual void vfn_0012_DCACE760() = 0;
};
