// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <C_SlotManager.h>

namespace ue
{

namespace at
{

/** ue::at::C_GameATManagerTickedModule (VTable=0x01E512E0) */
class C_GameATManagerTickedModule : public C_TickedModule
{
public:
	/** ue::at::C_GameATManagerTickedModule::C_SDSEvents (VTable=0x01E52720) */
	class C_SDSEvents : public C_SlotManager::C_Events
	{
	public:
		virtual void vfn_0001_DCE38594() = 0;
		virtual void vfn_0002_DCE38594() = 0;
		virtual void vfn_0003_DCE38594() = 0;
	};

	virtual void vfn_0001_D1233479() = 0;
	virtual void vfn_0002_D1233479() = 0;
	virtual void vfn_0003_D1233479() = 0;
	virtual void vfn_0004_D1233479() = 0;
	virtual void vfn_0005_D1233479() = 0;
	virtual void vfn_0006_D1233479() = 0;
};

} // namespace at

} // namespace ue
