// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <ue/game/params/I_SaveLoadCallBack.h>
#include <C_SlotManager.h>

/** C_CityShops (VTable=0x01E7F7A0) */
class C_CityShops : public C_TickedModule
{
public:
	/** C_CityShops::C_SaveLoadCallback (VTable=0x01E7F780) */
	class C_SaveLoadCallback : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_28BC9DD0() = 0;
		virtual void vfn_0002_28BC9DD0() = 0;
		virtual void vfn_0003_28BC9DD0() = 0;
		virtual void vfn_0004_28BC9DD0() = 0;
	};

	/** C_CityShops::C_SDSEvents (VTable=0x01E7F7C0) */
	class C_SDSEvents : public C_SlotManager::C_Events
	{
	public:
		virtual void vfn_0001_66D8C0D4() = 0;
		virtual void vfn_0002_66D8C0D4() = 0;
		virtual void vfn_0003_66D8C0D4() = 0;
	};

	/** C_CityShops::C_SDSWrapper (VTable=0x01E7F7E0) */
	class C_SDSWrapper : public C_SlotManager::C_SlotWrapper
	{
	public:
		virtual void vfn_0001_D2593A86() = 0;
		virtual void vfn_0002_D2593A86() = 0;
		virtual void vfn_0003_D2593A86() = 0;
		virtual void vfn_0004_D2593A86() = 0;
		virtual void vfn_0005_D2593A86() = 0;
		virtual void vfn_0006_D2593A86() = 0;
		virtual void vfn_0007_D2593A86() = 0;
		virtual void vfn_0008_D2593A86() = 0;
		virtual void vfn_0009_D2593A86() = 0;
	};

	virtual void vfn_0001_9D10E8CC() = 0;
	virtual void vfn_0002_9D10E8CC() = 0;
	virtual void vfn_0003_9D10E8CC() = 0;
	virtual void vfn_0004_9D10E8CC() = 0;
	virtual void vfn_0005_9D10E8CC() = 0;
	virtual void vfn_0006_9D10E8CC() = 0;
};
