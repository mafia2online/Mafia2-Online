// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <mafia/gui/menu/C_SysGuiCallbackHelper.h>
#include <mafia/gui/menu/C_HelperInputProvider.h>
#include <mafia/gui/utils/C_MapSlotWatcher.h>
#include <ue/sys/render2d/I_2dRenderContextHandler.h>
#include <I_EntityListener.h>
#include <ue/game/params/I_SaveLoadCallBack.h>

namespace mafia
{

namespace gui
{

namespace menu
{

namespace helpers
{

/** mafia::gui::menu::helpers::C_Map (VTable=0x01E7F020) */
class C_Map : public mafia::gui::menu::C_SysGuiCallbackHelper, public mafia::gui::menu::C_HelperInputProvider, public mafia::gui::utils::C_MapSlotWatcher, public ue::sys::render2d::I_2dRenderContextHandler, public I_EntityListener
{
public:
	/** mafia::gui::menu::helpers::C_Map::C_SaveLoadCallback (VTable=0x01E7EFF0) */
	class C_SaveLoadCallback : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_DA27B26D() = 0;
		virtual void vfn_0002_DA27B26D() = 0;
		virtual void vfn_0003_DA27B26D() = 0;
		virtual void vfn_0004_DA27B26D() = 0;
	};

	virtual void vfn_0001_BFD00810() = 0;
	virtual void vfn_0002_BFD00810() = 0;
	virtual void vfn_0003_BFD00810() = 0;
	virtual void vfn_0004_BFD00810() = 0;
	virtual void vfn_0005_BFD00810() = 0;
	virtual void vfn_0006_BFD00810() = 0;
	virtual void vfn_0007_BFD00810() = 0;
	virtual void vfn_0008_BFD00810() = 0;
	virtual void vfn_0009_BFD00810() = 0;
	virtual void vfn_0010_BFD00810() = 0;
	virtual void vfn_0011_BFD00810() = 0;
	virtual void vfn_0012_BFD00810() = 0;
	virtual void vfn_0013_BFD00810() = 0;
	virtual void vfn_0014_BFD00810() = 0;
	virtual void vfn_0015_BFD00810() = 0;
	virtual void vfn_0016_BFD00810() = 0;
};

} // namespace helpers

} // namespace menu

} // namespace gui

} // namespace mafia
