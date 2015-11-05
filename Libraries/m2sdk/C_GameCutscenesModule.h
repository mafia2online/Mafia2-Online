// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <I_EntityListener.h>
#include <ue/game/params/I_SaveLoadCallBack.h>
#include <ue/game/cutscenes/I_ModuleNotifyHandler.h>

/** C_GameCutscenesModule (VTable=0x01E7F980) */
class C_GameCutscenesModule : public C_TickedModule
{
public:
	/** C_GameCutscenesModule::C_EntityListener (VTable=0x01E7F9A0) */
	class C_EntityListener : public I_EntityListener
	{
	public:
		virtual void vfn_0001_745730C5() = 0;
		virtual void vfn_0002_745730C5() = 0;
		virtual void vfn_0003_745730C5() = 0;
	};

	/** C_GameCutscenesModule::C_SaveLoadCallback (VTable=0x01E7F9B8) */
	class C_SaveLoadCallback : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_37587FF7() = 0;
		virtual void vfn_0002_37587FF7() = 0;
		virtual void vfn_0003_37587FF7() = 0;
		virtual void vfn_0004_37587FF7() = 0;
	};

	/** C_GameCutscenesModule::C_Handler (VTable=0x01E7F968) */
	class C_Handler : public ue::game::cutscenes::I_ModuleNotifyHandler
	{
	public:
		virtual void vfn_0001_A923CD82() = 0;
		virtual void vfn_0002_A923CD82() = 0;
		virtual void vfn_0003_A923CD82() = 0;
	};

	virtual void vfn_0001_BD8AD479() = 0;
	virtual void vfn_0002_BD8AD479() = 0;
	virtual void vfn_0003_BD8AD479() = 0;
	virtual void vfn_0004_BD8AD479() = 0;
	virtual void vfn_0005_BD8AD479() = 0;
	virtual void vfn_0006_BD8AD479() = 0;
};
