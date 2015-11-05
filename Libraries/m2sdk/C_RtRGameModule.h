// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <ue/game/params/I_SaveLoadCallBack.h>

/** C_RtRGameModule (VTable=0x01E798E0) */
class C_RtRGameModule : public C_TickedModule
{
public:
	/** C_RtRGameModule::C_SaveLoadCallback (VTable=0x01E798B0) */
	class C_SaveLoadCallback : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_843EB3EF() = 0;
		virtual void vfn_0002_843EB3EF() = 0;
		virtual void vfn_0003_843EB3EF() = 0;
		virtual void vfn_0004_843EB3EF() = 0;
	};

	virtual void vfn_0001_89B73F43() = 0;
	virtual void vfn_0002_89B73F43() = 0;
	virtual void vfn_0003_89B73F43() = 0;
	virtual void vfn_0004_89B73F43() = 0;
	virtual void vfn_0005_89B73F43() = 0;
	virtual void vfn_0006_89B73F43() = 0;
};
