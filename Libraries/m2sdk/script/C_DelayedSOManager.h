// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <ue/game/params/I_SaveLoadCallBack.h>

namespace script
{

/** script::C_DelayedSOManager (VTable=0x01E7D2A0) */
class C_DelayedSOManager : public C_TickedModule
{
public:
	/** script::C_DelayedSOManager::C_SaveLoadCallback (VTable=0x01E7E1F0) */
	class C_SaveLoadCallback : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_52957C35() = 0;
		virtual void vfn_0002_52957C35() = 0;
		virtual void vfn_0003_52957C35() = 0;
		virtual void vfn_0004_52957C35() = 0;
	};

	virtual void vfn_0001_2F7D9EDD() = 0;
	virtual void vfn_0002_2F7D9EDD() = 0;
	virtual void vfn_0003_2F7D9EDD() = 0;
	virtual void vfn_0004_2F7D9EDD() = 0;
	virtual void vfn_0005_2F7D9EDD() = 0;
	virtual void vfn_0006_2F7D9EDD() = 0;
	virtual void vfn_0007_2F7D9EDD() = 0;
	virtual void vfn_0008_2F7D9EDD() = 0;
};

} // namespace script
