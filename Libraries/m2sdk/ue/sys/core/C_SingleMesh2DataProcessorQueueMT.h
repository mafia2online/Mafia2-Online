// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/sys/core/C_SingleMesh2DataProcessorQueueST.h>
#include <ue/sys/threads/I_LWJobManager.h>

namespace ue
{

namespace sys
{

namespace core
{

/** ue::sys::core::C_SingleMesh2DataProcessorQueueMT (VTable=0x01E85A60) */
class C_SingleMesh2DataProcessorQueueMT : public C_SingleMesh2DataProcessorQueueST
{
public:
	/** ue::sys::core::C_SingleMesh2DataProcessorQueueMT::C_WorkerJob (VTable=0x01E877A8) */
	class C_WorkerJob : public ue::sys::threads::I_LWJobManager::I_Job
	{
	public:
		virtual void vfn_0001_04EA691F() = 0;
		virtual void vfn_0002_04EA691F() = 0;
		virtual void vfn_0003_04EA691F() = 0;
		virtual void vfn_0004_04EA691F() = 0;
		virtual void vfn_0005_04EA691F() = 0;
	};

	virtual void vfn_0001_D24DED80() = 0;
	virtual void vfn_0002_D24DED80() = 0;
	virtual void vfn_0003_D24DED80() = 0;
	virtual void vfn_0004_D24DED80() = 0;
	virtual void vfn_0005_D24DED80() = 0;
	virtual void vfn_0006_D24DED80() = 0;
	virtual void vfn_0007_D24DED80() = 0;
};

} // namespace core

} // namespace sys

} // namespace ue
