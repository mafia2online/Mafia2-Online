// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <C_SlotManager.h>
#include <ue/game/params/I_SaveLoadCallBack.h>

/** C_SDSLoadUnloadNotify (VTable=0x01E78CA0) */
class C_SDSLoadUnloadNotify : public C_TickedModule
{
public:
	/** C_SDSLoadUnloadNotify::C_Events (VTable=0x01E79558) */
	class C_Events : public C_SlotManager::C_Events
	{
	public:
		virtual void vfn_0001_8A21839A() = 0;
		virtual void vfn_0002_8A21839A() = 0;
		virtual void vfn_0003_8A21839A() = 0;
	};

	/** C_SDSLoadUnloadNotify::C_SaveLoadCallback (VTable=0x01E78CC0) */
	class C_SaveLoadCallback : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_D9FFB87D() = 0;
		virtual void vfn_0002_D9FFB87D() = 0;
		virtual void vfn_0003_D9FFB87D() = 0;
		virtual void vfn_0004_D9FFB87D() = 0;
	};

	virtual void vfn_0001_3AA69B2A() = 0;
	virtual void vfn_0002_3AA69B2A() = 0;
	virtual void vfn_0003_3AA69B2A() = 0;
	virtual void vfn_0004_3AA69B2A() = 0;
	virtual void vfn_0005_3AA69B2A() = 0;
	virtual void vfn_0006_3AA69B2A() = 0;
};
