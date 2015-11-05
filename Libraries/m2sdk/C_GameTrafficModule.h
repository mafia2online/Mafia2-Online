// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <ue/game/params/I_SaveLoadCallBack.h>

/** C_GameTrafficModule (VTable=0x01E7AA60) */
class C_GameTrafficModule : public C_TickedModule
{
public:
	/** C_GameTrafficModule::C_SaveLoadCallbackTraffic (VTable=0x01E7BA78) */
	class C_SaveLoadCallbackTraffic : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_780F2441() = 0;
		virtual void vfn_0002_780F2441() = 0;
		virtual void vfn_0003_780F2441() = 0;
		virtual void vfn_0004_780F2441() = 0;
	};

	virtual void vfn_0001_317B691B() = 0;
	virtual void vfn_0002_317B691B() = 0;
	virtual void vfn_0003_317B691B() = 0;
	virtual void vfn_0004_317B691B() = 0;
	virtual void vfn_0005_317B691B() = 0;
	virtual void vfn_0006_317B691B() = 0;
};
