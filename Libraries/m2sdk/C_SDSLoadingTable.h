// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <ue/game/params/I_SaveLoadCallBack.h>
#include <C_SDSLoadingTable.h>
#include <C_StreamMapa.h>
#include <C_SlotManager.h>
#include <C_CarManager.h>

/** C_SDSLoadingTable (VTable=0x01E78D80) */
class C_SDSLoadingTable : public C_TickedModule
{
public:
	/** C_SDSLoadingTable::C_SaveLoadCallback (VTable=0x01E795F8) */
	class C_SaveLoadCallback : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_9CE68992() = 0;
		virtual void vfn_0002_9CE68992() = 0;
		virtual void vfn_0003_9CE68992() = 0;
		virtual void vfn_0004_9CE68992() = 0;
	};

	/** C_SDSLoadingTable::S_Command (VTable=0xFFFFFFFF) */
	struct S_Command
	{
	public:
		/** C_SDSLoadingTable::S_Command::S_AdditionalDataBase (VTable=0x01E79648) */
		struct S_AdditionalDataBase
		{
		public:
			virtual void vfn_0001_65DC1A43() = 0;
			virtual void vfn_0002_65DC1A43() = 0;
		};

		/** C_SDSLoadingTable::S_Command::S_AdditionalDataText (VTable=0x01E79620) */
		struct S_AdditionalDataText : public S_AdditionalDataBase
		{
		public:
			virtual void vfn_0001_1D5F7AB6() = 0;
			virtual void vfn_0002_1D5F7AB6() = 0;
		};
	};

	/** C_SDSLoadingTable::C_Enumerator (VTable=0x01E78DA0) */
	class C_Enumerator : public C_StreamMapa::I_StreamMapaCommandEnumerator
	{
	public:
		virtual void vfn_0001_38692C9A() = 0;
		virtual void vfn_0002_38692C9A() = 0;
		virtual void vfn_0003_38692C9A() = 0;
		virtual void vfn_0004_38692C9A() = 0;
		virtual void vfn_0005_38692C9A() = 0;
		virtual void vfn_0006_38692C9A() = 0;
		virtual void vfn_0007_38692C9A() = 0;
		virtual void vfn_0008_38692C9A() = 0;
		virtual void vfn_0009_38692C9A() = 0;
		virtual void vfn_0010_38692C9A() = 0;
		virtual void vfn_0011_38692C9A() = 0;
		virtual void vfn_0012_38692C9A() = 0;
		virtual void vfn_0013_38692C9A() = 0;
		virtual void vfn_0014_38692C9A() = 0;
		virtual void vfn_0015_38692C9A() = 0;
		virtual void vfn_0016_38692C9A() = 0;
		virtual void vfn_0017_38692C9A() = 0;
		virtual void vfn_0018_38692C9A() = 0;
		virtual void vfn_0019_38692C9A() = 0;
		virtual void vfn_0020_38692C9A() = 0;
		virtual void vfn_0021_38692C9A() = 0;
		virtual void vfn_0022_38692C9A() = 0;
	};

	/** C_SDSLoadingTable::C_SDSEvents (VTable=0x01E78E00) */
	class C_SDSEvents : public C_SlotManager::C_Events
	{
	public:
		virtual void vfn_0001_BCADD9A4() = 0;
		virtual void vfn_0002_BCADD9A4() = 0;
		virtual void vfn_0003_BCADD9A4() = 0;
	};

	/** C_SDSLoadingTable::C_CarModelStreamMap (VTable=0x01E79580) */
	class C_CarModelStreamMap : public C_CarManager::C_CarModel
	{
	public:
		virtual void vfn_0001_7A55CD09() = 0;
		virtual void vfn_0002_7A55CD09() = 0;
		virtual void vfn_0003_7A55CD09() = 0;
		virtual void vfn_0004_7A55CD09() = 0;
		virtual void vfn_0005_7A55CD09() = 0;
		virtual void vfn_0006_7A55CD09() = 0;
		virtual void vfn_0007_7A55CD09() = 0;
		virtual void vfn_0008_7A55CD09() = 0;
	};

	/** C_SDSLoadingTable::C_SDSWrapper (VTable=0x01E795C0) */
	class C_SDSWrapper : public C_SlotManager::C_SlotWrapper
	{
	public:
		virtual void vfn_0001_B513C882() = 0;
		virtual void vfn_0002_B513C882() = 0;
		virtual void vfn_0003_B513C882() = 0;
		virtual void vfn_0004_B513C882() = 0;
		virtual void vfn_0005_B513C882() = 0;
		virtual void vfn_0006_B513C882() = 0;
		virtual void vfn_0007_B513C882() = 0;
		virtual void vfn_0008_B513C882() = 0;
		virtual void vfn_0009_B513C882() = 0;
	};

	virtual void vfn_0001_B47BA526() = 0;
	virtual void vfn_0002_B47BA526() = 0;
	virtual void vfn_0003_B47BA526() = 0;
	virtual void vfn_0004_B47BA526() = 0;
	virtual void vfn_0005_B47BA526() = 0;
	virtual void vfn_0006_B47BA526() = 0;
};
