// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <mafia/gui/utils/C_MapSlotWatcher.h>
#include <mafia/gui/utils/C_SlotCache.h>
#include <I_EntityListener.h>
#include <ue/game/params/I_SaveLoadCallBack.h>

namespace mafia
{

namespace gui
{

namespace navigation
{

/** mafia::gui::navigation::C_Navigation (VTable=0x01E7EC20) */
class C_Navigation : public mafia::gui::utils::C_MapSlotWatcher, public mafia::gui::utils::C_SlotCache::I_SlotCacheCallback, public I_EntityListener
{
public:
	/** mafia::gui::navigation::C_Navigation::C_SaveLoadCallback (VTable=0x01E7EC00) */
	class C_SaveLoadCallback : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_D52FB37A() = 0;
		virtual void vfn_0002_D52FB37A() = 0;
		virtual void vfn_0003_D52FB37A() = 0;
		virtual void vfn_0004_D52FB37A() = 0;
	};

	virtual void vfn_0001_D45B4841() = 0;
	virtual void vfn_0002_D45B4841() = 0;
	virtual void vfn_0003_D45B4841() = 0;
	virtual void vfn_0004_D45B4841() = 0;
	virtual void vfn_0005_D45B4841() = 0;
	virtual void vfn_0006_D45B4841() = 0;
	virtual void vfn_0007_D45B4841() = 0;
	virtual void vfn_0008_D45B4841() = 0;
	virtual void vfn_0009_D45B4841() = 0;
	virtual void vfn_0010_D45B4841() = 0;
	virtual void vfn_0011_D45B4841() = 0;
	virtual void vfn_0012_D45B4841() = 0;
	virtual void vfn_0013_D45B4841() = 0;
};

} // namespace navigation

} // namespace gui

} // namespace mafia
