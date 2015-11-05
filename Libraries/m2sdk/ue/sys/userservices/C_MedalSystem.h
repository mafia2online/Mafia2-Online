// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <ue/sys/threads/I_ThreadProcedure.h>
#include <ue/sys/userservices/I_SysNotificationCallback.h>

namespace ue
{

namespace sys
{

namespace userservices
{

/** ue::sys::userservices::C_MedalSystem (VTable=0x01E94F40) */
class C_MedalSystem : public C_TickedModule, public ue::sys::threads::I_ThreadProcedure, public I_SysNotificationCallback
{
public:
	virtual void vfn_0001_1F2C5D2A() = 0;
	virtual void vfn_0002_1F2C5D2A() = 0;
	virtual void vfn_0003_1F2C5D2A() = 0;
	virtual void vfn_0004_1F2C5D2A() = 0;
	virtual void vfn_0005_1F2C5D2A() = 0;
	virtual void vfn_0006_1F2C5D2A() = 0;
	virtual void vfn_0007_1F2C5D2A() = 0;
	virtual void vfn_0008_1F2C5D2A() = 0;
};

} // namespace userservices

} // namespace sys

} // namespace ue
