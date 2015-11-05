// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/sys/core/C_SynchronizationJob.h>

namespace ue
{

namespace sys
{

namespace core
{

/** ue::sys::core::C_SyncManager (VTable=0xFFFFFFFF) */
class C_SyncManager
{
public:
	/** ue::sys::core::C_SyncManager::C_GrabCallback (VTable=0x01E879B0) */
	class C_GrabCallback : public ue::sys::core::C_SynchronizationJob::I_GrabCallback
	{
	public:
		virtual void vfn_0001_98CC3A19() = 0;
		virtual void vfn_0002_98CC3A19() = 0;
		virtual void vfn_0003_98CC3A19() = 0;
	};
};

} // namespace core

} // namespace sys

} // namespace ue
