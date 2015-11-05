// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <C_SlotManager.h>
#include <C_CarManager.h>

/** C_TrafficStreaming (VTable=0x01E55C60) */
class C_TrafficStreaming : public C_TickedModule
{
public:
	/** C_TrafficStreaming::C_FreeSlotCallBack (VTable=0x01E51A70) */
	class C_FreeSlotCallBack
	{
	public:
		virtual void vfn_0001_63FCF300() = 0;
		virtual void vfn_0002_63FCF300() = 0;
		virtual void vfn_0003_63FCF300() = 0;
	};

	/** C_TrafficStreaming::C_Model (VTable=0x01E55C98) */
	class C_Model
	{
	public:
		virtual void vfn_0001_3214AF80() = 0;
		virtual void vfn_0002_3214AF80() = 0;
		virtual void vfn_0003_3214AF80() = 0;
		virtual void vfn_0004_3214AF80() = 0;
		virtual void vfn_0005_3214AF80() = 0;
	};

	/** C_TrafficStreaming::C_SDSWrapper (VTable=0x01E55CC0) */
	class C_SDSWrapper : public C_SlotManager::C_SlotWrapper
	{
	public:
		virtual void vfn_0001_F34950DC() = 0;
		virtual void vfn_0002_F34950DC() = 0;
		virtual void vfn_0003_F34950DC() = 0;
		virtual void vfn_0004_F34950DC() = 0;
		virtual void vfn_0005_F34950DC() = 0;
		virtual void vfn_0006_F34950DC() = 0;
		virtual void vfn_0007_F34950DC() = 0;
		virtual void vfn_0008_F34950DC() = 0;
		virtual void vfn_0009_F34950DC() = 0;
	};

	/** C_TrafficStreaming::C_SDSEnvents (VTable=0x01E55C80) */
	class C_SDSEnvents : public C_SlotManager::C_Events
	{
	public:
		virtual void vfn_0001_2BE56142() = 0;
		virtual void vfn_0002_2BE56142() = 0;
		virtual void vfn_0003_2BE56142() = 0;
	};

	/** C_TrafficStreaming::C_StreamCar (VTable=0x01E55D00) */
	class C_StreamCar : public C_CarManager::C_CarModel
	{
	public:
		virtual void vfn_0001_C546A1B2() = 0;
		virtual void vfn_0002_C546A1B2() = 0;
		virtual void vfn_0003_C546A1B2() = 0;
		virtual void vfn_0004_C546A1B2() = 0;
		virtual void vfn_0005_C546A1B2() = 0;
		virtual void vfn_0006_C546A1B2() = 0;
		virtual void vfn_0007_C546A1B2() = 0;
		virtual void vfn_0008_C546A1B2() = 0;
	};

	virtual void vfn_0001_3B03E9C7() = 0;
	virtual void vfn_0002_3B03E9C7() = 0;
	virtual void vfn_0003_3B03E9C7() = 0;
	virtual void vfn_0004_3B03E9C7() = 0;
	virtual void vfn_0005_3B03E9C7() = 0;
	virtual void vfn_0006_3B03E9C7() = 0;
};
