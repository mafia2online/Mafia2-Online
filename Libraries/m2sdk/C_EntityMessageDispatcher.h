// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <ue/game/params/I_SaveLoadCallBack.h>

/** C_EntityMessageDispatcher (VTable=0x01E63140) */
class C_EntityMessageDispatcher : public C_TickedModule
{
public:
	/** C_EntityMessageDispatcher::C_SaveLoadCallback (VTable=0x01E63160) */
	class C_SaveLoadCallback : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_51A5050A() = 0;
		virtual void vfn_0002_51A5050A() = 0;
		virtual void vfn_0003_51A5050A() = 0;
		virtual void vfn_0004_51A5050A() = 0;
	};

	virtual void vfn_0001_424A847C() = 0;
	virtual void vfn_0002_424A847C() = 0;
	virtual void vfn_0003_424A847C() = 0;
	virtual void vfn_0004_424A847C() = 0;
	virtual void vfn_0005_424A847C() = 0;
	virtual void vfn_0006_424A847C() = 0;
};
