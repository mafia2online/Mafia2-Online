// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/sys/threads/I_JobManagerWorker.h>
#include <ue/sys/threads/I_ThreadProcedure.h>

namespace ue
{

namespace sys
{

namespace threads
{

/** ue::sys::threads::C_JobWorker (VTable=0x01E946C0) */
class C_JobWorker : public I_JobManagerWorker, public I_ThreadProcedure
{
public:
	virtual void vfn_0001_C3E0DCF4() = 0;
	virtual void vfn_0002_C3E0DCF4() = 0;
	virtual void vfn_0003_C3E0DCF4() = 0;
	virtual void vfn_0004_C3E0DCF4() = 0;
	virtual void vfn_0005_C3E0DCF4() = 0;
	virtual void vfn_0006_C3E0DCF4() = 0;
};

} // namespace threads

} // namespace sys

} // namespace ue
