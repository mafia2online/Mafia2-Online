// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/sys/threads/I_JobManager.h>

namespace ue
{

namespace sys
{

namespace threads
{

/** ue::sys::threads::C_JobManager (VTable=0x01E946A0) */
class C_JobManager : public I_JobManager
{
public:
	/** ue::sys::threads::C_JobManager::C_JobExecution (VTable=0x01E94660) */
	class C_JobExecution : public ue::sys::threads::I_JobManager::I_JobExecution
	{
	public:
		virtual void vfn_0001_55807C27() = 0;
		virtual void vfn_0002_55807C27() = 0;
		virtual void vfn_0003_55807C27() = 0;
		virtual void vfn_0004_55807C27() = 0;
		virtual void vfn_0005_55807C27() = 0;
		virtual void vfn_0006_55807C27() = 0;
		virtual void vfn_0007_55807C27() = 0;
		virtual void vfn_0008_55807C27() = 0;
		virtual void vfn_0009_55807C27() = 0;
		virtual void vfn_0010_55807C27() = 0;
	};

	virtual void vfn_0001_55B799D3() = 0;
	virtual void vfn_0002_55B799D3() = 0;
	virtual void vfn_0003_55B799D3() = 0;
	virtual void vfn_0004_55B799D3() = 0;
	virtual void vfn_0005_55B799D3() = 0;
	virtual void vfn_0006_55B799D3() = 0;
};

} // namespace threads

} // namespace sys

} // namespace ue
