// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <C_SlotManager.h>

/** C_EntityActivator (VTable=0x01E55C00) */
class C_EntityActivator : public C_TickedModule
{
public:
	/** C_EntityActivator::C_SDSNotifyHandler (VTable=0x01E55C20) */
	class C_SDSNotifyHandler : public C_SlotManager::C_Events
	{
	public:
		virtual void vfn_0001_B8FE5756() = 0;
		virtual void vfn_0002_B8FE5756() = 0;
		virtual void vfn_0003_B8FE5756() = 0;
	};

	virtual void vfn_0001_8C1CC972() = 0;
	virtual void vfn_0002_8C1CC972() = 0;
	virtual void vfn_0003_8C1CC972() = 0;
	virtual void vfn_0004_8C1CC972() = 0;
	virtual void vfn_0005_8C1CC972() = 0;
	virtual void vfn_0006_8C1CC972() = 0;
};
