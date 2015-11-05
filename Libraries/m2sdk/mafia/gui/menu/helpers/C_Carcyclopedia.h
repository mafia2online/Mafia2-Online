// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <mafia/gui/menu/C_SysGuiCallbackHelper.h>
#include <mafia/gui/menu/C_HelperInputProvider.h>
#include <ue/sys/physics/C_SimulateHandler.h>
#include <C_CarManager.h>

namespace mafia
{

namespace gui
{

namespace menu
{

namespace helpers
{

/** mafia::gui::menu::helpers::C_Carcyclopedia (VTable=0x01E7EF60) */
class C_Carcyclopedia : public mafia::gui::menu::C_SysGuiCallbackHelper, public mafia::gui::menu::C_HelperInputProvider, public ue::sys::physics::C_SimulateHandler
{
public:
	/** mafia::gui::menu::helpers::C_Carcyclopedia::C_SdsCar (VTable=0x01E81BE0) */
	class C_SdsCar : public C_CarManager::C_CarModel
	{
	public:
		virtual void vfn_0001_E5D76FB2() = 0;
		virtual void vfn_0002_E5D76FB2() = 0;
		virtual void vfn_0003_E5D76FB2() = 0;
		virtual void vfn_0004_E5D76FB2() = 0;
		virtual void vfn_0005_E5D76FB2() = 0;
		virtual void vfn_0006_E5D76FB2() = 0;
		virtual void vfn_0007_E5D76FB2() = 0;
		virtual void vfn_0008_E5D76FB2() = 0;
	};

	virtual void vfn_0001_BA0D5465() = 0;
	virtual void vfn_0002_BA0D5465() = 0;
	virtual void vfn_0003_BA0D5465() = 0;
	virtual void vfn_0004_BA0D5465() = 0;
	virtual void vfn_0005_BA0D5465() = 0;
	virtual void vfn_0006_BA0D5465() = 0;
	virtual void vfn_0007_BA0D5465() = 0;
	virtual void vfn_0008_BA0D5465() = 0;
	virtual void vfn_0009_BA0D5465() = 0;
	virtual void vfn_0010_BA0D5465() = 0;
	virtual void vfn_0011_BA0D5465() = 0;
	virtual void vfn_0012_BA0D5465() = 0;
	virtual void vfn_0013_BA0D5465() = 0;
	virtual void vfn_0014_BA0D5465() = 0;
};

} // namespace helpers

} // namespace menu

} // namespace gui

} // namespace mafia
