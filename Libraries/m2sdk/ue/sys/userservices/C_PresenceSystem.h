// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <ue/sys/userservices/I_SysNotificationCallback.h>

namespace ue
{

namespace sys
{

namespace userservices
{

/** ue::sys::userservices::C_PresenceSystem (VTable=0x01E94F00) */
class C_PresenceSystem : public C_TickedModule, public I_SysNotificationCallback
{
public:
	virtual void vfn_0001_12E54060() = 0;
	virtual void vfn_0002_12E54060() = 0;
	virtual void vfn_0003_12E54060() = 0;
	virtual void vfn_0004_12E54060() = 0;
	virtual void vfn_0005_12E54060() = 0;
	virtual void vfn_0006_12E54060() = 0;
	virtual void vfn_0007_12E54060() = 0;
};

} // namespace userservices

} // namespace sys

} // namespace ue
