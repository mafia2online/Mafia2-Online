// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <ue/game/params/I_SaveLoadCallBack.h>
#include <I_EntityListener.h>

/** C_EntityListTickedModule (VTable=0x01E79820) */
class C_EntityListTickedModule : public C_TickedModule
{
public:
	/** C_EntityListTickedModule::C_SaveLoadCallback (VTable=0x01E79850) */
	class C_SaveLoadCallback : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_FD3B37D7() = 0;
		virtual void vfn_0002_FD3B37D7() = 0;
		virtual void vfn_0003_FD3B37D7() = 0;
		virtual void vfn_0004_FD3B37D7() = 0;
	};

	/** C_EntityListTickedModule::C_EntityListener (VTable=0x01E79170) */
	class C_EntityListener : public I_EntityListener
	{
	public:
		virtual void vfn_0001_780B29FB() = 0;
		virtual void vfn_0002_780B29FB() = 0;
		virtual void vfn_0003_780B29FB() = 0;
	};

	virtual void vfn_0001_3EEF3B3E() = 0;
	virtual void vfn_0002_3EEF3B3E() = 0;
	virtual void vfn_0003_3EEF3B3E() = 0;
	virtual void vfn_0004_3EEF3B3E() = 0;
	virtual void vfn_0005_3EEF3B3E() = 0;
	virtual void vfn_0006_3EEF3B3E() = 0;
};
