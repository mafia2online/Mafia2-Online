// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <ue/game/params/I_SaveLoadCallBack.h>

/** C_StreamingPosManager (VTable=0x01E7F680) */
class C_StreamingPosManager : public C_TickedModule
{
public:
	/** C_StreamingPosManager::C_SaveLoadCallback (VTable=0x01E7F660) */
	class C_SaveLoadCallback : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_52B3FCFB() = 0;
		virtual void vfn_0002_52B3FCFB() = 0;
		virtual void vfn_0003_52B3FCFB() = 0;
		virtual void vfn_0004_52B3FCFB() = 0;
	};

	virtual void vfn_0001_286CC05D() = 0;
	virtual void vfn_0002_286CC05D() = 0;
	virtual void vfn_0003_286CC05D() = 0;
	virtual void vfn_0004_286CC05D() = 0;
	virtual void vfn_0005_286CC05D() = 0;
	virtual void vfn_0006_286CC05D() = 0;
};
