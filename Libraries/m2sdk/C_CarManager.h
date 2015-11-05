// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <C_SlotManager.h>

/** C_CarManager (VTable=0x01E78CE0) */
class C_CarManager : public C_TickedModule
{
public:
	/** C_CarManager::C_OwnershipMigration (VTable=0x01E78D50) */
	class C_OwnershipMigration
	{
	public:
		virtual void vfn_0001_82C4470A() = 0;
		virtual void vfn_0002_82C4470A() = 0;
		virtual void vfn_0003_82C4470A() = 0;
		virtual void vfn_0004_82C4470A() = 0;
	};

	/** C_CarManager::C_CarModel (VTable=0x01E56160) */
	class C_CarModel
	{
	public:
		virtual void vfn_0001_C9B7C381() = 0;
		virtual void vfn_0002_C9B7C381() = 0;
		virtual void vfn_0003_C9B7C381() = 0;
		virtual void vfn_0004_C9B7C381() = 0;
		virtual void vfn_0005_C9B7C381() = 0;
		virtual void vfn_0006_C9B7C381() = 0;
		virtual void vfn_0007_C9B7C381() = 0;
		virtual void vfn_0008_C9B7C381() = 0;
	};

	/** C_CarManager::C_SDSWrapper (VTable=0x01E78D20) */
	class C_SDSWrapper : public C_SlotManager::C_SlotWrapper
	{
	public:
		virtual void vfn_0001_BAD99410() = 0;
		virtual void vfn_0002_BAD99410() = 0;
		virtual void vfn_0003_BAD99410() = 0;
		virtual void vfn_0004_BAD99410() = 0;
		virtual void vfn_0005_BAD99410() = 0;
		virtual void vfn_0006_BAD99410() = 0;
		virtual void vfn_0007_BAD99410() = 0;
		virtual void vfn_0008_BAD99410() = 0;
		virtual void vfn_0009_BAD99410() = 0;
	};

	/** C_CarManager::C_SDSEnvents (VTable=0x01E78D00) */
	class C_SDSEnvents : public C_SlotManager::C_Events
	{
	public:
		virtual void vfn_0001_30D6AA9E() = 0;
		virtual void vfn_0002_30D6AA9E() = 0;
		virtual void vfn_0003_30D6AA9E() = 0;
	};

	virtual void vfn_0001_941A5438() = 0;
	virtual void vfn_0002_941A5438() = 0;
	virtual void vfn_0003_941A5438() = 0;
	virtual void vfn_0004_941A5438() = 0;
	virtual void vfn_0005_941A5438() = 0;
	virtual void vfn_0006_941A5438() = 0;
};
