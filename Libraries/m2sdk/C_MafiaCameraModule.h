// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <ue/game/params/I_SaveLoadCallBack.h>
#include <ue/game/cutscenes/I_ModuleNotifyHandler.h>

/** C_MafiaCameraModule (VTable=0x01E7AAA0) */
class C_MafiaCameraModule : public C_TickedModule
{
public:
	/** C_MafiaCameraModule::C_SaveLoadCallback (VTable=0x01E7AA80) */
	class C_SaveLoadCallback : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_80BF1357() = 0;
		virtual void vfn_0002_80BF1357() = 0;
		virtual void vfn_0003_80BF1357() = 0;
		virtual void vfn_0004_80BF1357() = 0;
	};

	/** C_MafiaCameraModule::C_CutScenesNotificationsListener (VTable=0x01E7BAC8) */
	class C_CutScenesNotificationsListener : public ue::game::cutscenes::I_ModuleNotifyHandler
	{
	public:
		virtual void vfn_0001_4883211C() = 0;
		virtual void vfn_0002_4883211C() = 0;
		virtual void vfn_0003_4883211C() = 0;
	};

	virtual void vfn_0001_0186AA40() = 0;
	virtual void vfn_0002_0186AA40() = 0;
	virtual void vfn_0003_0186AA40() = 0;
	virtual void vfn_0004_0186AA40() = 0;
	virtual void vfn_0005_0186AA40() = 0;
	virtual void vfn_0006_0186AA40() = 0;
	virtual void vfn_0007_0186AA40() = 0;
	virtual void vfn_0008_0186AA40() = 0;
};
