// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/sys/threads/I_LWJobManager.h>

namespace ue
{

namespace sys
{

namespace core
{

/** ue::sys::core::C_SynchronizationJob (VTable=0x01E84F88) */
class C_SynchronizationJob : public ue::sys::threads::I_LWJobManager::I_Job
{
public:
	/** ue::sys::core::C_SynchronizationJob::I_GrabCallback (VTable=0xFFFFFFFF) */
	class I_GrabCallback
	{
	public:
	};

	virtual void vfn_0001_275979EE() = 0;
	virtual void vfn_0002_275979EE() = 0;
	virtual void vfn_0003_275979EE() = 0;
	virtual void vfn_0004_275979EE() = 0;
	virtual void vfn_0005_275979EE() = 0;
};

} // namespace core

} // namespace sys

} // namespace ue
