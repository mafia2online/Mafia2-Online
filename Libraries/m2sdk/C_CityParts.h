// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <C_SlotManager.h>

/** C_CityParts (VTable=0x01E7F580) */
class C_CityParts : public C_TickedModule
{
public:
	/** C_CityParts::C_SDSWrapperCity (VTable=0x01E7F5C0) */
	class C_SDSWrapperCity : public C_SlotManager::C_SlotWrapper
	{
	public:
		virtual void vfn_0001_FD389378() = 0;
		virtual void vfn_0002_FD389378() = 0;
		virtual void vfn_0003_FD389378() = 0;
		virtual void vfn_0004_FD389378() = 0;
		virtual void vfn_0005_FD389378() = 0;
		virtual void vfn_0006_FD389378() = 0;
		virtual void vfn_0007_FD389378() = 0;
		virtual void vfn_0008_FD389378() = 0;
		virtual void vfn_0009_FD389378() = 0;
	};

	/** C_CityParts::C_SDSEvents (VTable=0x01E7F5A0) */
	class C_SDSEvents : public C_SlotManager::C_Events
	{
	public:
		virtual void vfn_0001_12E7435B() = 0;
		virtual void vfn_0002_12E7435B() = 0;
		virtual void vfn_0003_12E7435B() = 0;
	};

	virtual void vfn_0001_DC75F0C2() = 0;
	virtual void vfn_0002_DC75F0C2() = 0;
	virtual void vfn_0003_DC75F0C2() = 0;
	virtual void vfn_0004_DC75F0C2() = 0;
	virtual void vfn_0005_DC75F0C2() = 0;
	virtual void vfn_0006_DC75F0C2() = 0;
};
