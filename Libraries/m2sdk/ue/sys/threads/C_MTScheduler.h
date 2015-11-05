// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/sys/threads/I_Scheduler.h>

namespace ue
{

namespace sys
{

namespace threads
{

/** ue::sys::threads::C_MTScheduler (VTable=0x01E944E0) */
class C_MTScheduler : public I_Scheduler
{
public:
	/** ue::sys::threads::C_MTScheduler::I_JobCallback (VTable=0xFFFFFFFF) */
	class I_JobCallback
	{
	public:
	};

	virtual void vfn_0001_AB488920() = 0;
	virtual void vfn_0002_AB488920() = 0;
	virtual void vfn_0003_AB488920() = 0;
};

} // namespace threads

} // namespace sys

} // namespace ue
