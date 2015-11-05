// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <ue/game/params/I_SaveLoadCallBack.h>

/** C_SystemInitDone (VTable=0x01E797A0) */
class C_SystemInitDone : public C_TickedModule
{
public:
	/** C_SystemInitDone::C_SaveLoadCallback (VTable=0x01E797D0) */
	class C_SaveLoadCallback : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_AA179932() = 0;
		virtual void vfn_0002_AA179932() = 0;
		virtual void vfn_0003_AA179932() = 0;
		virtual void vfn_0004_AA179932() = 0;
	};

	virtual void vfn_0001_6B8FF849() = 0;
	virtual void vfn_0002_6B8FF849() = 0;
	virtual void vfn_0003_6B8FF849() = 0;
	virtual void vfn_0004_6B8FF849() = 0;
	virtual void vfn_0005_6B8FF849() = 0;
	virtual void vfn_0006_6B8FF849() = 0;
};
