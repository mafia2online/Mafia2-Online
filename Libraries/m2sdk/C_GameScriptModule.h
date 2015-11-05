// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <ue/game/params/I_SaveLoadCallBack.h>
#include <ue/game/script/C_ScriptMachineManager.h>

/** C_GameScriptModule (VTable=0x01E7A960) */
class C_GameScriptModule : public C_TickedModule
{
public:
	/** C_GameScriptModule::C_SaveLoadCallback (VTable=0x01E7A998) */
	class C_SaveLoadCallback : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_5CA566D7() = 0;
		virtual void vfn_0002_5CA566D7() = 0;
		virtual void vfn_0003_5CA566D7() = 0;
		virtual void vfn_0004_5CA566D7() = 0;
	};

	/** C_GameScriptModule::C_ScriptMachineIdProvider (VTable=0x01E7A980) */
	class C_ScriptMachineIdProvider : public ue::game::script::C_ScriptMachineManager::I_ScriptMachineIdProvider
	{
	public:
		virtual void vfn_0001_BF997EA9() = 0;
		virtual void vfn_0002_BF997EA9() = 0;
		virtual void vfn_0003_BF997EA9() = 0;
		virtual void vfn_0004_BF997EA9() = 0;
	};

	virtual void vfn_0001_9412E874() = 0;
	virtual void vfn_0002_9412E874() = 0;
	virtual void vfn_0003_9412E874() = 0;
	virtual void vfn_0004_9412E874() = 0;
	virtual void vfn_0005_9412E874() = 0;
	virtual void vfn_0006_9412E874() = 0;
};
