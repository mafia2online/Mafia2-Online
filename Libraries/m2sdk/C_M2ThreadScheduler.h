// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/sys/core/I_CoreCallbacks.h>
#include <ue/sys/threads/I_ThreadProcedure.h>
#include <ue/sys/render/I_RenderIdleCallback.h>
#include <ue/sys/threads/C_MTScheduler.h>

/** C_M2ThreadScheduler (VTable=0x01E55D80) */
class C_M2ThreadScheduler : public ue::sys::core::I_CoreCallbacks, public ue::sys::threads::I_ThreadProcedure, public ue::sys::render::I_RenderIdleCallback
{
public:
	/** C_M2ThreadScheduler::C_Worker (VTable=0x01E55D40) */
	class C_Worker : public ue::sys::threads::I_ThreadProcedure, public ue::sys::threads::C_MTScheduler::I_JobCallback
	{
	public:
		virtual void vfn_0001_E737E8CE() = 0;
		virtual void vfn_0002_E737E8CE() = 0;
		virtual void vfn_0003_E737E8CE() = 0;
		virtual void vfn_0004_E737E8CE() = 0;
		virtual void vfn_0005_E737E8CE() = 0;
		virtual void vfn_0006_E737E8CE() = 0;
	};

	virtual void vfn_0001_A601759B() = 0;
	virtual void vfn_0002_A601759B() = 0;
	virtual void vfn_0003_A601759B() = 0;
	virtual void vfn_0004_A601759B() = 0;
	virtual void vfn_0005_A601759B() = 0;
	virtual void vfn_0006_A601759B() = 0;
	virtual void vfn_0007_A601759B() = 0;
	virtual void vfn_0008_A601759B() = 0;
	virtual void vfn_0009_A601759B() = 0;
	virtual void vfn_0010_A601759B() = 0;
	virtual void vfn_0011_A601759B() = 0;
	virtual void vfn_0012_A601759B() = 0;
	virtual void vfn_0013_A601759B() = 0;
	virtual void vfn_0014_A601759B() = 0;
	virtual void vfn_0015_A601759B() = 0;
	virtual void vfn_0016_A601759B() = 0;
	virtual void vfn_0017_A601759B() = 0;
	virtual void vfn_0018_A601759B() = 0;
};
