// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <NxUserScheduler.h>
#include <ue/sys/threads/I_LWJobManager.h>

namespace ue
{

namespace sys
{

namespace physics
{

/** ue::sys::physics::C_MTScheduler (VTable=0x01E8D078) */
class C_MTScheduler : public NxUserScheduler
{
public:
	/** ue::sys::physics::C_MTScheduler::C_Job (VTable=0x01E8E678) */
	class C_Job : public ue::sys::threads::I_LWJobManager::I_Job
	{
	public:
		virtual void vfn_0001_01AAA091() = 0;
		virtual void vfn_0002_01AAA091() = 0;
		virtual void vfn_0003_01AAA091() = 0;
		virtual void vfn_0004_01AAA091() = 0;
		virtual void vfn_0005_01AAA091() = 0;
	};

	virtual void vfn_0001_8E6E2CDF() = 0;
	virtual void vfn_0002_8E6E2CDF() = 0;
	virtual void vfn_0003_8E6E2CDF() = 0;
	virtual void vfn_0004_8E6E2CDF() = 0;
	virtual void vfn_0005_8E6E2CDF() = 0;
};

} // namespace physics

} // namespace sys

} // namespace ue
